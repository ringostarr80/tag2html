/***************************************************************************
                          main.cpp  -  description
                             -------------------
    program              : tag2html
    version              : 0.2.0
    begin                : Mon Mär  3 22:34:16 CET 2003
    copyright            : (C) 2003 by ringostarr / sfo
    email                : ringostarr <ringo19@gmx.de>
                           sfo <sfo@zapo.net>
    resource             : http://tag2html.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <vector>
#include <stdlib.h>

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegheader.h>
#include <taglib/mpegproperties.h>

#include "xml.h"

using namespace std;

void writeXmlFile(std::list<TagLib::FileRef>* fileRefs, bool outputXSL, bool outputXSD)
{
	XMLPlatformUtils::Initialize();
	DOMImplementation* domImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));
	DOMDocument* doc = domImplementation->createDocument(0, XMLString::transcode("tag-info"), 0);

	if (outputXSL) {
		DOMProcessingInstruction* pi = doc->createProcessingInstruction(XMLString::transcode("xml-stylesheet"), XMLString::transcode("type=\"text/xsl\" href=\"index.xsl\""));
		doc->insertBefore(pi, doc->getDocumentElement());
	}
	if (outputXSD) {
		doc->getDocumentElement()->setAttribute(XMLString::transcode("xmlns:xsi"), XMLString::transcode("http://www.w3.org/2001/XMLSchema-instance"));
		doc->getDocumentElement()->setAttribute(XMLString::transcode("xsi:noNamespaceSchemalLocation"), XMLString::transcode("index.xsd"));
	}

	for (auto currentFileRef = fileRefs->begin(); currentFileRef != fileRefs->end(); currentFileRef++) {
		DOMElement* track = doc->createElement(XMLString::transcode("track"));
		if (currentFileRef->tag()->track() < 1000) {
			char trackNumber[3];
			sprintf(trackNumber, "%u", currentFileRef->tag()->track());
			track->setAttribute(XMLString::transcode("number"), XMLString::transcode(trackNumber));
		}

		DOMElement* artist = doc->createElement(XMLString::transcode("artist"));
		artist->setTextContent(XMLString::transcode(currentFileRef->tag()->artist().toCString(true)));
		track->appendChild(artist);

		DOMElement* title = doc->createElement(XMLString::transcode("title"));
		title->setTextContent(XMLString::transcode(currentFileRef->tag()->title().toCString(true)));
		if (currentFileRef->audioProperties()->length() < 3600) {
			char trackLength[12];
			struct tm* timeinfo = new tm();

			int seconds = currentFileRef->audioProperties()->length();
			if (seconds >= 60) {
				int minutes = seconds / 60;
				if (minutes >= 60) {
					timeinfo->tm_hour = minutes / 60;
					timeinfo->tm_min = minutes / 60;
				} else {
					timeinfo->tm_min = seconds / 60;
				}
			}
			timeinfo->tm_sec = seconds % 60;
			strftime(trackLength, 12, "%H:%M:%S", timeinfo);

			title->setAttribute(XMLString::transcode("length"), XMLString::transcode(trackLength));
		}
		track->appendChild(title);

		DOMElement* album = doc->createElement(XMLString::transcode("album"));
		album->setTextContent(XMLString::transcode(currentFileRef->tag()->album().toCString(true)));
		char year[4];
		sprintf(year, "%u", currentFileRef->tag()->year());
		album->setAttribute(XMLString::transcode("year"), XMLString::transcode(year));
		album->setAttribute(XMLString::transcode("genre"), XMLString::transcode(currentFileRef->tag()->genre().toCString(true)));
		track->appendChild(album);

		DOMElement* comment = doc->createElement(XMLString::transcode("comment"));
		comment->setTextContent(XMLString::transcode(currentFileRef->tag()->comment().toCString(true)));
		track->appendChild(comment);

		DOMElement* filename = doc->createElement(XMLString::transcode("filename"));
		filename->setTextContent(XMLString::transcode(currentFileRef->file()->name()));
		track->appendChild(filename);

		TagLib::MPEG::File f(currentFileRef->file()->name());

		DOMElement* headerInfo = doc->createElement(XMLString::transcode("header-info"));

		const char* isCopyrighted = (f.audioProperties()->isCopyrighted()) ? "1" : "0";
		const char* isOriginal = (f.audioProperties()->isOriginal()) ? "1" : "0";
		const char* protectionEnabled = (f.audioProperties()->protectionEnabled()) ? "1" : "0";
		headerInfo->setAttribute(XMLString::transcode("is_copyrighted"), XMLString::transcode(isCopyrighted));
		headerInfo->setAttribute(XMLString::transcode("is_original"), XMLString::transcode(isOriginal));
		headerInfo->setAttribute(XMLString::transcode("protection_enabled"), XMLString::transcode(protectionEnabled));

		DOMElement* bitrate = doc->createElement(XMLString::transcode("bitrate"));
		char bitrateString[3];
		sprintf(bitrateString, "%u", currentFileRef->audioProperties()->bitrate());
		bitrate->setAttribute(XMLString::transcode("kbits"), XMLString::transcode(bitrateString));
		headerInfo->appendChild(bitrate);

		DOMElement* sampleRate = doc->createElement(XMLString::transcode("samplerate"));
		char sampleRateString[5];
		sprintf(sampleRateString, "%u", currentFileRef->audioProperties()->sampleRate());
		sampleRate->setAttribute(XMLString::transcode("hz"), XMLString::transcode(sampleRateString));
		headerInfo->appendChild(sampleRate);

		DOMElement* channels = doc->createElement(XMLString::transcode("channels"));
		char channelsString[1];
		sprintf(channelsString, "%u", f.audioProperties()->channels());
		channels->setTextContent(XMLString::transcode(channelsString));
		const char* channelMode;
		switch (f.audioProperties()->channelMode()) {
			case TagLib::MPEG::Header::ChannelMode::DualChannel:
				channelMode = "DualChannel";
				break;
			case TagLib::MPEG::Header::ChannelMode::JointStereo:
				channelMode = "JointStereo";
				break;
			case TagLib::MPEG::Header::ChannelMode::SingleChannel:
				channelMode = "SingleChannel";
				break;
			case TagLib::MPEG::Header::ChannelMode::Stereo:
				channelMode = "Stereo";
				break;
		}
		channels->setAttribute(XMLString::transcode("mode"), XMLString::transcode(channelMode));
		headerInfo->appendChild(channels);

		DOMElement* mpegAudioVersion = doc->createElement(XMLString::transcode("mpeg-audio-version"));
		char layerString[1];
		sprintf(layerString, "%u", f.audioProperties()->layer());
		mpegAudioVersion->setAttribute(XMLString::transcode("layer"), XMLString::transcode(layerString));
		const char* version;
		switch (f.audioProperties()->version()) {
			case TagLib::MPEG::Header::Version::Version1:
				version = "1";
				break;
			case TagLib::MPEG::Header::Version::Version2:
				version = "2";
				break;
			case TagLib::MPEG::Header::Version::Version2_5:
				version = "2.5";
				break;
		}
		mpegAudioVersion->setTextContent(XMLString::transcode(version));
		headerInfo->appendChild(mpegAudioVersion);

		track->appendChild(headerInfo);

		doc->getDocumentElement()->appendChild(track);
	}

	DOMWriter* writer = ((DOMImplementationLS*)domImplementation)->createDOMWriter();
	if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
		writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("index.xml");
	writer->writeNode(fileFormatTarget, *doc);
	fileFormatTarget->flush();
	writer->release();

	doc->release();
	XMLPlatformUtils::Terminate();

	if (outputXSD) {
		writeXsdFile();
	}

	if (outputXSL) {
		writeXslFile();
	}
}

void writeXsdFile()
{
	XMLPlatformUtils::Initialize();
	DOMImplementation* domImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));
	// <xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema">
	DOMDocument* doc = domImplementation->createDocument(XMLString::transcode("http://www.w3.org/2001/XMLSchema"), XMLString::transcode("xsd:schema"), 0);

	// <xsd:element name="tag-info">
	DOMElement* xsdElementTagInfo = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementTagInfo->setAttribute(XMLString::transcode("name"), XMLString::transcode("tag-info"));

	//	<xsd:complexType>
	DOMElement* xsdComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

	//		<xsd:sequence>
	DOMElement* xsdSequence = doc->createElement(XMLString::transcode("xsd:sequence"));

	//			<xsd:element name="track" maxOccurs="unbounded" minOccurs="0">
	DOMElement* xsdElementTrack = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementTrack->setAttribute(XMLString::transcode("name"), XMLString::transcode("track"));
	xsdElementTrack->setAttribute(XMLString::transcode("maxOccurs"), XMLString::transcode("unbounded"));
	xsdElementTrack->setAttribute(XMLString::transcode("minOccurs"), XMLString::transcode("0"));

	//				<xsd:complexType>
	DOMElement* xsdElementTrackComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

	//					<xsd:sequence>
	DOMElement* xsdElementTrackSequence = doc->createElement(XMLString::transcode("xsd:sequence"));

	//						<xsd:element name="artist" type="xsd:string"/>
	DOMElement* xsdElementTrackElementArtist = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementTrackElementArtist->setAttribute(XMLString::transcode("name"), XMLString::transcode("artist"));
	xsdElementTrackElementArtist->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:string"));

	//						<xsd:element name="title">
	DOMElement* xsdElementTrackElementTitle = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementTrackElementTitle->setAttribute(XMLString::transcode("name"), XMLString::transcode("title"));

	//							<xsd:complexType>
	DOMElement* xsdElementTrackElementTitleComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

	//								<xsd:simpleContent>
	DOMElement* xsdElementTrackElementTitleSimpleContent = doc->createElement(XMLString::transcode("xsd:simpleContent"));

	//									<xsd:extension base="xsd:string">
	DOMElement* xsdElementTrackElementTitleExtension = doc->createElement(XMLString::transcode("xsd:extension"));
	xsdElementTrackElementTitleExtension->setAttribute(XMLString::transcode("base"), XMLString::transcode("xsd:string"));

	//										<xsd:attribute type="xs:short" name="length" use="optional"/>
	DOMElement* xsdElementTrackElementAttributeLength = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementTrackElementAttributeLength->setAttribute(XMLString::transcode("name"), XMLString::transcode("length"));
	xsdElementTrackElementAttributeLength->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:short"));
	xsdElementTrackElementAttributeLength->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementTrackElementTitleExtension->appendChild(xsdElementTrackElementAttributeLength);
	xsdElementTrackElementTitleSimpleContent->appendChild(xsdElementTrackElementTitleExtension);
	xsdElementTrackElementTitleComplexType->appendChild(xsdElementTrackElementTitleSimpleContent);
	xsdElementTrackElementTitle->appendChild(xsdElementTrackElementTitleComplexType);

	// <xsd:element name="album">
	DOMElement* xsdElementTrackElementAlbum = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementTrackElementAlbum->setAttribute(XMLString::transcode("name"), XMLString::transcode("album"));

    //	<xsd:complexType>
	DOMElement* xsdElementTrackElementAlbumComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

	//		<xsd:simpleContent>
	DOMElement* xsdElementTrackElementAlbumSimpleContent = doc->createElement(XMLString::transcode("xsd:simpleContent"));

    //			<xsd:extension base="xsd:string">
	DOMElement* xsdElementTrackElementAlbumExtension = doc->createElement(XMLString::transcode("xsd:extension"));
	xsdElementTrackElementAlbumExtension->setAttribute(XMLString::transcode("base"), XMLString::transcode("xsd:string"));

    //				<xsd:attribute type="xsd:string" name="genre" use="optional"/>
	DOMElement* xsdElementTrackElementAlbumExtensionAttributeGenre = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementTrackElementAlbumExtensionAttributeGenre->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:string"));
	xsdElementTrackElementAlbumExtensionAttributeGenre->setAttribute(XMLString::transcode("name"), XMLString::transcode("genre"));
	xsdElementTrackElementAlbumExtensionAttributeGenre->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

    //				<xsd:attribute type="xsd:short" name="year" use="optional"/>
	DOMElement* xsdElementTrackElementAlbumExtensionAttributeYear = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementTrackElementAlbumExtensionAttributeYear->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:short"));
	xsdElementTrackElementAlbumExtensionAttributeYear->setAttribute(XMLString::transcode("name"), XMLString::transcode("year"));
	xsdElementTrackElementAlbumExtensionAttributeYear->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementTrackElementAlbumExtension->appendChild(xsdElementTrackElementAlbumExtensionAttributeGenre);
	xsdElementTrackElementAlbumExtension->appendChild(xsdElementTrackElementAlbumExtensionAttributeYear);
	xsdElementTrackElementAlbumSimpleContent->appendChild(xsdElementTrackElementAlbumExtension);
	xsdElementTrackElementAlbumComplexType->appendChild(xsdElementTrackElementAlbumSimpleContent);
	xsdElementTrackElementAlbum->appendChild(xsdElementTrackElementAlbumComplexType);

	// <xsd:element type="xsd:string" name="comment"/>
	DOMElement* xsdElementTrackElementComment = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementTrackElementComment->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:string"));
	xsdElementTrackElementComment->setAttribute(XMLString::transcode("name"), XMLString::transcode("comment"));

    // <xsd:element type="xsd:string" name="filename"/>
	DOMElement* xsdElementTrackElementFilename = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementTrackElementFilename->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:string"));
	xsdElementTrackElementFilename->setAttribute(XMLString::transcode("name"), XMLString::transcode("filename"));

	// <xsd:element name="header-info">
	DOMElement* xsdElementHeaderInfo = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementHeaderInfo->setAttribute(XMLString::transcode("name"), XMLString::transcode("header-info"));

	// <xsd:complexType>
	DOMElement* xsdElementHeaderInfoComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

	// <xsd:sequence>
	DOMElement* xsdElementHeaderInfoSequence = doc->createElement(XMLString::transcode("xsd:sequence"));

	// <xsd:element name="bitrate">
	DOMElement* xsdElementHeaderInfoElementBitrate = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementHeaderInfoElementBitrate->setAttribute(XMLString::transcode("name"), XMLString::transcode("bitrate"));

    //	<xsd:complexType>
	DOMElement* xsdElementHeaderInfoElementBitrateComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

    //		<xsd:simpleContent>
	DOMElement* xsdElementHeaderInfoElementBitrateSimpleContent = doc->createElement(XMLString::transcode("xsd:simpleContent"));

    //			<xsd:extension base="xs:string">
	DOMElement* xsdElementHeaderInfoElementBitrateExtension = doc->createElement(XMLString::transcode("xsd:extension"));
	xsdElementHeaderInfoElementBitrateExtension->setAttribute(XMLString::transcode("base"), XMLString::transcode("xsd:string"));

    //				<xsd:attribute type="xsd:short" name="kbits" use="optional"/>
	DOMElement* xsdElementHeaderInfoElementBitrateAttributeKbits = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementHeaderInfoElementBitrateAttributeKbits->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:short"));
	xsdElementHeaderInfoElementBitrateAttributeKbits->setAttribute(XMLString::transcode("name"), XMLString::transcode("kbits"));
	xsdElementHeaderInfoElementBitrateAttributeKbits->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementHeaderInfoElementBitrateExtension->appendChild(xsdElementHeaderInfoElementBitrateAttributeKbits);
	xsdElementHeaderInfoElementBitrateSimpleContent->appendChild(xsdElementHeaderInfoElementBitrateExtension);
	xsdElementHeaderInfoElementBitrateComplexType->appendChild(xsdElementHeaderInfoElementBitrateSimpleContent);
	xsdElementHeaderInfoElementBitrate->appendChild(xsdElementHeaderInfoElementBitrateComplexType);

	// <xsd:element name="samplerate">
	DOMElement* xsdElementHeaderInfoElementSampleRate = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementHeaderInfoElementSampleRate->setAttribute(XMLString::transcode("name"), XMLString::transcode("samplerate"));

    //	<xsd:complexType>
	DOMElement* xsdElementHeaderInfoElementSampleRateComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

    //		<xsd:simpleContent>
	DOMElement* xsdElementHeaderInfoElementSampleRateSimpleContent = doc->createElement(XMLString::transcode("xsd:simpleContent"));

    //			<xsd:extension base="xsd:string">
	DOMElement* xsdElementHeaderInfoElementSampleRateExtension = doc->createElement(XMLString::transcode("xsd:extension"));
	xsdElementHeaderInfoElementSampleRateExtension->setAttribute(XMLString::transcode("base"), XMLString::transcode("xsd:string"));

    //				<xsd:attribute type="xsd:int" name="hz" use="optional"/>
	DOMElement* xsdElementHeaderInfoElementSampleRateAttributeHz = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementHeaderInfoElementSampleRateAttributeHz->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:int"));
	xsdElementHeaderInfoElementSampleRateAttributeHz->setAttribute(XMLString::transcode("name"), XMLString::transcode("hz"));
	xsdElementHeaderInfoElementSampleRateAttributeHz->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementHeaderInfoElementSampleRateExtension->appendChild(xsdElementHeaderInfoElementSampleRateAttributeHz);
	xsdElementHeaderInfoElementSampleRateSimpleContent->appendChild(xsdElementHeaderInfoElementSampleRateExtension);
	xsdElementHeaderInfoElementSampleRateComplexType->appendChild(xsdElementHeaderInfoElementSampleRateSimpleContent);
	xsdElementHeaderInfoElementSampleRate->appendChild(xsdElementHeaderInfoElementSampleRateComplexType);

	// <xsd:element name="channels">
	DOMElement* xsdElementHeaderInfoElementChannels = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementHeaderInfoElementChannels->setAttribute(XMLString::transcode("name"), XMLString::transcode("channels"));

    //	<xsd:complexType>
	DOMElement* xsdElementHeaderInfoElementChannelsComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

    //		<xsd:simpleContent>
	DOMElement* xsdElementHeaderInfoElementChannelsSimpleContent = doc->createElement(XMLString::transcode("xsd:simpleContent"));

    //			<xsd:extension base="xsd:byte">
	DOMElement* xsdElementHeaderInfoElementChannelsExtension = doc->createElement(XMLString::transcode("xsd:extension"));
	xsdElementHeaderInfoElementChannelsExtension->setAttribute(XMLString::transcode("base"), XMLString::transcode("xsd:byte"));

    //				<xsd:attribute type="xsd:string" name="mode" use="optional"/>
	DOMElement* xsdElementHeaderInfoElementChannelsAttributeMode = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementHeaderInfoElementChannelsAttributeMode->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:string"));
	xsdElementHeaderInfoElementChannelsAttributeMode->setAttribute(XMLString::transcode("name"), XMLString::transcode("mode"));
	xsdElementHeaderInfoElementChannelsAttributeMode->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementHeaderInfoElementChannelsExtension->appendChild(xsdElementHeaderInfoElementChannelsAttributeMode);
	xsdElementHeaderInfoElementChannelsSimpleContent->appendChild(xsdElementHeaderInfoElementChannelsExtension);
	xsdElementHeaderInfoElementChannelsComplexType->appendChild(xsdElementHeaderInfoElementChannelsSimpleContent);
	xsdElementHeaderInfoElementChannels->appendChild(xsdElementHeaderInfoElementChannelsComplexType);

	// <xsd:element name="mpeg-audio-version">
	DOMElement* xsdElementHeaderInfoElementMpegAudioVersion = doc->createElement(XMLString::transcode("xsd:element"));
	xsdElementHeaderInfoElementMpegAudioVersion->setAttribute(XMLString::transcode("name"), XMLString::transcode("mpeg-audio-version"));

    //	<xsd:complexType>
	DOMElement* xsdElementHeaderInfoElementMpegAudioVersionComplexType = doc->createElement(XMLString::transcode("xsd:complexType"));

    //		<xsd:simpleContent>
	DOMElement* xsdElementHeaderInfoElementMpegAudioVersionSimpleContent = doc->createElement(XMLString::transcode("xsd:simpleContent"));

    //			<xsd:extension base="xsd:byte">
	DOMElement* xsdElementHeaderInfoElementMpegAudioVersionExtension = doc->createElement(XMLString::transcode("xsd:extension"));
	xsdElementHeaderInfoElementMpegAudioVersionExtension->setAttribute(XMLString::transcode("base"), XMLString::transcode("xsd:byte"));

    //				<xsd:attribute type="xsd:byte" name="layer" use="optional"/>
	DOMElement* xsdElementHeaderInfoElementMpegAudioVersionAttributeLayer = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementHeaderInfoElementMpegAudioVersionAttributeLayer->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:byte"));
	xsdElementHeaderInfoElementMpegAudioVersionAttributeLayer->setAttribute(XMLString::transcode("name"), XMLString::transcode("layer"));
	xsdElementHeaderInfoElementMpegAudioVersionAttributeLayer->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementHeaderInfoElementMpegAudioVersionExtension->appendChild(xsdElementHeaderInfoElementMpegAudioVersionAttributeLayer);
	xsdElementHeaderInfoElementMpegAudioVersionSimpleContent->appendChild(xsdElementHeaderInfoElementMpegAudioVersionExtension);
	xsdElementHeaderInfoElementMpegAudioVersionComplexType->appendChild(xsdElementHeaderInfoElementMpegAudioVersionSimpleContent);
	xsdElementHeaderInfoElementMpegAudioVersion->appendChild(xsdElementHeaderInfoElementMpegAudioVersionComplexType);

	xsdElementHeaderInfoSequence->appendChild(xsdElementHeaderInfoElementBitrate);
	xsdElementHeaderInfoSequence->appendChild(xsdElementHeaderInfoElementSampleRate);
	xsdElementHeaderInfoSequence->appendChild(xsdElementHeaderInfoElementChannels);
	xsdElementHeaderInfoSequence->appendChild(xsdElementHeaderInfoElementMpegAudioVersion);
	xsdElementHeaderInfoComplexType->appendChild(xsdElementHeaderInfoSequence);

	// <xsd:attribute type="xsd:byte" name="is_copyrighted" use="optional"/>
	DOMElement* xsdElementHeaderInfoAttributeIsCopyrighted = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementHeaderInfoAttributeIsCopyrighted->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:byte"));
	xsdElementHeaderInfoAttributeIsCopyrighted->setAttribute(XMLString::transcode("name"), XMLString::transcode("is_copyrighted"));
	xsdElementHeaderInfoAttributeIsCopyrighted->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

    // <xsd:attribute type="xsd:byte" name="is_original" use="optional"/>
	DOMElement* xsdElementHeaderInfoAttributeIsOriginal = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementHeaderInfoAttributeIsOriginal->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:byte"));
	xsdElementHeaderInfoAttributeIsOriginal->setAttribute(XMLString::transcode("name"), XMLString::transcode("is_original"));
	xsdElementHeaderInfoAttributeIsOriginal->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

    // <xsd:attribute type="xsd:byte" name="protection_enabled" use="optional"/>
	DOMElement* xsdElementHeaderInfoAttributeProtectionEnabled = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementHeaderInfoAttributeProtectionEnabled->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:byte"));
	xsdElementHeaderInfoAttributeProtectionEnabled->setAttribute(XMLString::transcode("name"), XMLString::transcode("protection_enabled"));
	xsdElementHeaderInfoAttributeProtectionEnabled->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementHeaderInfoComplexType->appendChild(xsdElementHeaderInfoAttributeIsCopyrighted);
	xsdElementHeaderInfoComplexType->appendChild(xsdElementHeaderInfoAttributeIsOriginal);
	xsdElementHeaderInfoComplexType->appendChild(xsdElementHeaderInfoAttributeProtectionEnabled);

	xsdElementHeaderInfo->appendChild(xsdElementHeaderInfoComplexType);

	xsdElementTrackSequence->appendChild(xsdElementTrackElementArtist);
	xsdElementTrackSequence->appendChild(xsdElementTrackElementTitle);
	xsdElementTrackSequence->appendChild(xsdElementTrackElementAlbum);
	xsdElementTrackSequence->appendChild(xsdElementTrackElementComment);
	xsdElementTrackSequence->appendChild(xsdElementTrackElementFilename);
	xsdElementTrackSequence->appendChild(xsdElementHeaderInfo);
	xsdElementTrackComplexType->appendChild(xsdElementTrackSequence);

	// <xsd:attribute type="xsd:byte" name="number" use="optional"/>
	DOMElement* xsdElementTrackAttributeNumber = doc->createElement(XMLString::transcode("xsd:attribute"));
	xsdElementTrackAttributeNumber->setAttribute(XMLString::transcode("type"), XMLString::transcode("xsd:byte"));
	xsdElementTrackAttributeNumber->setAttribute(XMLString::transcode("name"), XMLString::transcode("number"));
	xsdElementTrackAttributeNumber->setAttribute(XMLString::transcode("use"), XMLString::transcode("optional"));

	xsdElementTrackComplexType->appendChild(xsdElementTrackAttributeNumber);

	xsdElementTrack->appendChild(xsdElementTrackComplexType);
	xsdSequence->appendChild(xsdElementTrack);
	xsdComplexType->appendChild(xsdSequence);
	xsdElementTagInfo->appendChild(xsdComplexType);

	doc->getDocumentElement()->appendChild(xsdElementTagInfo);

	DOMWriter* writer = ((DOMImplementationLS*)domImplementation)->createDOMWriter();
	if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
		writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("index.xsd");
	writer->writeNode(fileFormatTarget, *doc);
	fileFormatTarget->flush();
	writer->release();

	doc->release();
	XMLPlatformUtils::Terminate();
}

void writeXslFile()
{
	XMLPlatformUtils::Initialize();
	DOMImplementation* domImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));
	// <?xml version="1.0" encoding="utf-8" ?>
	// <xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	DOMDocument* doc = domImplementation->createDocument(XMLString::transcode("http://www.w3.org/1999/XSL/Transform"), XMLString::transcode("xsl:stylesheet"), 0);
	doc->getDocumentElement()->setAttribute(XMLString::transcode("version"), XMLString::transcode("1.0"));

	// <xsl:template match="/">
	DOMElement* xslTemplate = doc->createElement(XMLString::transcode("xsl:template"));
	xslTemplate->setAttribute(XMLString::transcode("match"), XMLString::transcode("/"));

	// <html>
	DOMElement* html = doc->createElement(XMLString::transcode("html"));

	// <head>
	DOMElement* head = doc->createElement(XMLString::transcode("head"));

	// <title>MP3-Leser</title>
	DOMElement* title = doc->createElement(XMLString::transcode("title"));
	title->setTextContent(XMLString::transcode("MP3-Leser"));

	// <link rel=\"stylesheet\" type=\"text/css\" href=\"index.css\"/>
	DOMElement* link = doc->createElement(XMLString::transcode("link"));
	link->setAttribute(XMLString::transcode("rel"), XMLString::transcode("stylesheet"));
	link->setAttribute(XMLString::transcode("type"), XMLString::transcode("text/css"));
	link->setAttribute(XMLString::transcode("href"), XMLString::transcode("index.css"));

	head->appendChild(title);
	head->appendChild(link);
	html->appendChild(head);

	// <body>
	DOMElement* body = doc->createElement(XMLString::transcode("body"));

	// <h1>Track List</h1>
	DOMElement* h1 = doc->createElement(XMLString::transcode("h1"));
	h1->setTextContent(XMLString::transcode("Track List"));

	body->appendChild(h1);

	// <table align="center" width="1000">
	DOMElement* table = doc->createElement(XMLString::transcode("table"));
	table->setAttribute(XMLString::transcode("align"), XMLString::transcode("center"));

	// <thead>
	DOMElement* thead = doc->createElement(XMLString::transcode("thead"));

	// <tr bgcolor="#CCCCCC">
	DOMElement* tr = doc->createElement(XMLString::transcode("tr"));
	tr->setAttribute(XMLString::transcode("bgcolor"), XMLString::transcode("#CCCCCC"));

	//  <th width="20"><b>Track</b></td>
	DOMElement* td1 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldTrack = doc->createElement(XMLString::transcode("b"));
	boldTrack->setTextContent(XMLString::transcode("Track"));
	td1->appendChild(boldTrack);
	tr->appendChild(td1);

	// <th width="100"><b>Artist</b></td>
	DOMElement* td2 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldArtist = doc->createElement(XMLString::transcode("b"));
	boldArtist->setTextContent(XMLString::transcode("Artist"));
	td2->appendChild(boldArtist);
	tr->appendChild(td2);

	// <th width="100"><b>Title</b></td>
	DOMElement* td3 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldTitle = doc->createElement(XMLString::transcode("b"));
	boldTitle->setTextContent(XMLString::transcode("Title"));
	td3->appendChild(boldTitle);
	tr->appendChild(td3);

	// <th width="100"><b>Album</b></td>
	DOMElement* td4 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldAlbum = doc->createElement(XMLString::transcode("b"));
	boldAlbum->setTextContent(XMLString::transcode("Album"));
	td4->appendChild(boldAlbum);
	tr->appendChild(td4);

	// <th width="50"><b>Year</b></td>
	DOMElement* td5 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldYear = doc->createElement(XMLString::transcode("b"));
	boldYear->setTextContent(XMLString::transcode("Year"));
	td5->appendChild(boldYear);
	tr->appendChild(td5);

	// <th width="100"><b>Genre</b></td>
	DOMElement* td6 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldGenre = doc->createElement(XMLString::transcode("b"));
	boldGenre->setTextContent(XMLString::transcode("Genre"));
	td6->appendChild(boldGenre);
	tr->appendChild(td6);

	// <th width="100"><b>Comment</b></td>
	DOMElement* td7 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldComment = doc->createElement(XMLString::transcode("b"));
	boldComment->setTextContent(XMLString::transcode("Comment"));
	td7->appendChild(boldComment);
	tr->appendChild(td7);

	// <th width="50"><b>Length</b></td>
	DOMElement* td8 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldLength = doc->createElement(XMLString::transcode("b"));
	boldLength->setTextContent(XMLString::transcode("Length"));
	td8->appendChild(boldLength);
	tr->appendChild(td8);

	// <th width="100"><b>Filename</b></td>
	DOMElement* td9 = doc->createElement(XMLString::transcode("td"));
	DOMElement* boldFilename = doc->createElement(XMLString::transcode("b"));
	boldFilename->setTextContent(XMLString::transcode("Filename"));
	td9->appendChild(boldFilename);
	tr->appendChild(td9);

	thead->appendChild(tr);
	table->appendChild(thead);

	// <xsl:for-each select="tag-info/track">
	DOMElement* xslForEach = doc->createElement(XMLString::transcode("xsl:for-each"));
	xslForEach->setAttribute(XMLString::transcode("select"), XMLString::transcode("tag-info/track"));

	//	<tr class="white">
	DOMElement* trWhite = doc->createElement(XMLString::transcode("tr"));
	trWhite->setAttribute(XMLString::transcode("class"), XMLString::transcode("white"));

	// <td><xsl:value-of select="@number"/></td>
	DOMElement* tdValueOfNumber = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfNumber = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfNumber->setAttribute(XMLString::transcode("select"), XMLString::transcode("@number"));
	tdValueOfNumber->appendChild(xslValueOfNumber);
	trWhite->appendChild(tdValueOfNumber);

	// <td><xsl:value-of select="artist"/></td>
	DOMElement* tdValueOfArtist = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfArtist = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfArtist->setAttribute(XMLString::transcode("select"), XMLString::transcode("artist"));
	tdValueOfArtist->appendChild(xslValueOfArtist);
	trWhite->appendChild(tdValueOfArtist);

	// <td><xsl:value-of select="title"/></td>
	DOMElement* tdValueOfTitle = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfTitle = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfTitle->setAttribute(XMLString::transcode("select"), XMLString::transcode("title"));
	tdValueOfTitle->appendChild(xslValueOfTitle);
	trWhite->appendChild(tdValueOfTitle);

	// <td><xsl:value-of select="album"/></td>
	DOMElement* tdValueOfAlbum = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfAlbum = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfAlbum->setAttribute(XMLString::transcode("select"), XMLString::transcode("album"));
	tdValueOfAlbum->appendChild(xslValueOfAlbum);
	trWhite->appendChild(tdValueOfAlbum);

	// <td><xsl:value-of select="album/@year"/></td>
	DOMElement* tdValueOfYear = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfYear = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfYear->setAttribute(XMLString::transcode("select"), XMLString::transcode("album/@year"));
	tdValueOfYear->appendChild(xslValueOfYear);
	trWhite->appendChild(tdValueOfYear);

	// <td><xsl:value-of select="album/@genre"/></td>
	DOMElement* tdValueOfGenre = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfGenre = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfGenre->setAttribute(XMLString::transcode("select"), XMLString::transcode("album/@genre"));
	tdValueOfGenre->appendChild(xslValueOfGenre);
	trWhite->appendChild(tdValueOfGenre);

	// <td><xsl:value-of select="comment"/></td>
	DOMElement* tdValueOfComment = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfComment = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfComment->setAttribute(XMLString::transcode("select"), XMLString::transcode("comment"));
	tdValueOfComment->appendChild(xslValueOfComment);
	trWhite->appendChild(tdValueOfComment);

	// <td><xsl:value-of select="title/@length"/></td>
	DOMElement* tdValueOfLength = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfLength = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfLength->setAttribute(XMLString::transcode("select"), XMLString::transcode("title/@length"));
	tdValueOfLength->appendChild(xslValueOfLength);
	trWhite->appendChild(tdValueOfLength);

	// <td><xsl:value-of select="filename"/></td>
	DOMElement* tdValueOfFilename = doc->createElement(XMLString::transcode("td"));
	DOMElement* xslValueOfFilename = doc->createElement(XMLString::transcode("xsl:value-of"));
	xslValueOfFilename->setAttribute(XMLString::transcode("select"), XMLString::transcode("filename"));
	tdValueOfFilename->appendChild(xslValueOfFilename);
	trWhite->appendChild(tdValueOfFilename);

	xslForEach->appendChild(trWhite);

	table->appendChild(xslForEach);
	body->appendChild(table);
	html->appendChild(body);

	xslTemplate->appendChild(html);

	doc->getDocumentElement()->appendChild(xslTemplate);

	DOMWriter* writer = ((DOMImplementationLS*)domImplementation)->createDOMWriter();
	if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
		writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("index.xsl");
	writer->writeNode(fileFormatTarget, *doc);
	fileFormatTarget->flush();
	writer->release();

	doc->release();
	XMLPlatformUtils::Terminate();

	writeCssFile();
}

void writeCssFile()
{
	ofstream cssFile("index.css");

	cssFile << "body {\n";
	cssFile << "\tfont-family: sans-serif;\n";
	cssFile << "\tfont-size:  12px;\n";
	cssFile << "\tbackground-color: #ABABAB;\n";
	cssFile << "\tcolor: #303030;\n";
	cssFile << "}\n";
	cssFile << "h1 {\n";
	cssFile << "\tfont-size: 16px;\n";
	cssFile << "\ttext-align: center;\n";
	cssFile << "}\n";
	cssFile << "table {\n";
	cssFile << "\tbackground-color: #FFFFFF;\n";
	cssFile << "\tborder: \"thin solid black\";\n";
	cssFile << "\twidth: 960px;\n";
	cssFile << "}\n";
	cssFile << "table.nb {\n";
	cssFile << "\tbackground-color: #FFFFFF;\n";
	cssFile << "\tborder: thin solid white;\n";
	cssFile << "}\n";
	cssFile << "td, th {\n";
	cssFile << "\tpadding: 2px 5px 2px 5px;\n";
	cssFile << "}\n";
	cssFile << "tbody > tr:nth-child(odd) {\n";
	cssFile << "\tbackground-color: white;\n";
	cssFile << "}\n";
	cssFile << "tbody > tr:nth-child(even) {\n";
	cssFile << "\tbackground-color: #EEEEEE;\n";
	cssFile << "}\n";
	cssFile << "a {\n";
	cssFile << "\tcolor: #444444;\n";
	cssFile << "}\n";

	cssFile.close();
}

int xsd_schema(char *filename)
{
	std::ofstream out(filename);

	if (!out.is_open()) {
		return -1;
	}

	out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
	out << "<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n";
	out << "<xsd:element name=\"tag-info\">\n";
	out << "\t<xsd:complexType>\n";
	out << "\t\t<xsd:sequence>\n";
	out << "\t\t\t<xsd:element name=\"track\" maxOccurs=\"unbounded\">\n";
	out << "\t\t\t\t<xsd:complexType>\n";
	out << "\t\t\t\t\t<xsd:sequence>\n";
	out << "\t\t\t\t\t\t<xsd:element name=\"artist\" type=\"xsd:string\"/>\n";
	out << "\t\t\t\t\t\t<xsd:element name=\"title\">\n";
	out << "\t\t\t\t\t\t\t<xsd:complexType>\n";
	out << "\t\t\t\t\t\t\t\t<xsd:simpleContent>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:extension base=\"xsd:string\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t<xsd:attribute name=\"length\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t<xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:pattern value=\"\\d?\\d?:?\\d?\\d:\\d\\d\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t</xsd:restriction>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t</xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t</xsd:attribute>\n";
	out << "\t\t\t\t\t\t\t\t\t</xsd:extension>\n";
	out << "\t\t\t\t\t\t\t\t</xsd:simpleContent>\n";
	out << "\t\t\t\t\t\t\t</xsd:complexType>\n";
	out << "\t\t\t\t\t\t</xsd:element>\n";
	out << "\t\t\t\t\t\t<xsd:element name=\"album\">\n";
	out << "\t\t\t\t\t\t\t<xsd:complexType>\n";
	out << "\t\t\t\t\t\t\t\t<xsd:simpleContent>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:extension base=\"xsd:string\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t<xsd:attribute name=\"year\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t<xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t<xsd:restriction base=\"xsd:nonNegativeInteger\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:maxInclusive value=\"2100\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t</xsd:restriction>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t</xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t</xsd:attribute>\n";
	out << "\t\t\t\t\t\t\t\t\t\t<xsd:attribute name=\"genre\" type=\"genreChoice\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t</xsd:extension>\n";
	out << "\t\t\t\t\t\t\t\t</xsd:simpleContent>\n";
	out << "\t\t\t\t\t\t\t</xsd:complexType>\n";
	out << "\t\t\t\t\t\t</xsd:element>\n";
	out << "\t\t\t\t\t\t<xsd:element name=\"comment\" type=\"xsd:string\" minOccurs=\"0\"/>\n";
	out << "\t\t\t\t\t\t<xsd:element name=\"filename\" type=\"xsd:string\"/>\n";
	out << "\t\t\t\t\t\t<xsd:element name=\"header-info\">\n";
	out << "\t\t\t\t\t\t\t<xsd:complexType>\n";
	out << "\t\t\t\t\t\t\t\t<xsd:sequence>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"bitrate\" type=\"kbitChoice\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"mpeg-audio-version\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t<xsd:complexType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t<xsd:simpleContent>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t<xsd:extension base=\"xsd:string\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:attribute name=\"layer\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Layer 1\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Layer 2\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Layer 3\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Reserviert\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t</xsd:restriction>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t</xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t\t</xsd:attribute>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t</xsd:extension>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t</xsd:simpleContent>\n";
	out << "\t\t\t\t\t\t\t\t\t\t</xsd:complexType>\n";
	out << "\t\t\t\t\t\t\t\t\t</xsd:element>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"error-protection\" type=\"Choice\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"samplerate\" type=\"HzChoice\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"private\" type=\"Choice\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"channel-mode\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t<xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Stereo\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Joint Stereo\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Dual Channel\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t\t<xsd:enumeration value=\"Mono\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t\t\t</xsd:restriction>\n";
	out << "\t\t\t\t\t\t\t\t\t\t</xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t\t\t</xsd:element>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"copyright\" type=\"Choice\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"original\" type=\"Choice\"/>\n";
	out << "\t\t\t\t\t\t\t\t\t<xsd:element name=\"emphasis\" type=\"EmphasisChoice\"/>\n";
	out << "\t\t\t\t\t\t\t\t</xsd:sequence>\n";
	out << "\t\t\t\t\t\t\t</xsd:complexType>\n";
	out << "\t\t\t\t\t\t</xsd:element>\n";
	out << "\t\t\t\t\t</xsd:sequence>\n";
	out << "\t\t\t\t\t<xsd:attribute name=\"number\">\n";
	out << "\t\t\t\t\t\t<xsd:simpleType>\n";
	out << "\t\t\t\t\t\t\t<xsd:restriction base=\"xsd:nonNegativeInteger\">\n";
	out << "\t\t\t\t\t\t\t\t<xsd:maxExclusive value=\"256\"/>\n";
	out << "\t\t\t\t\t\t\t</xsd:restriction>\n";
	out << "\t\t\t\t\t\t</xsd:simpleType>\n";
	out << "\t\t\t\t\t</xsd:attribute>\n";
	out << "\t\t\t\t</xsd:complexType>\n";
	out << "\t\t\t</xsd:element>\n";
	out << "\t\t</xsd:sequence>\n";
	out << "\t</xsd:complexType>\n";
	out << "</xsd:element>\n";
	out << "<xsd:complexType name=\"Choice\">\n";
	out << "\t<xsd:complexContent>\n";
	out << "\t\t<xsd:restriction base=\"xsd:anyType\">\n";
	out << "\t\t\t<xsd:attribute name=\"choice\" type=\"YesNoChoice\"/>\n";
	out << "\t\t</xsd:restriction>\n";
	out << "\t</xsd:complexContent>\n";
	out << "</xsd:complexType>\n";
	out << "<xsd:complexType name=\"EmphasisChoice\">\n";
	out << "\t<xsd:complexContent>\n";
	out << "\t\t<xsd:restriction base=\"xsd:anyType\">\n";
	out << "\t\t\t<xsd:attribute name=\"choice\" type=\"MultiEmphasisChoice\"/>\n";
	out << "\t\t</xsd:restriction>\n";
	out << "\t</xsd:complexContent>\n";
	out << "</xsd:complexType>\n";
	out << "<xsd:complexType name=\"HzChoice\">\n";
	out << "\t<xsd:complexContent>\n";
	out << "\t\t<xsd:restriction base=\"xsd:anyType\">\n";
	out << "\t\t\t<xsd:attribute name=\"Hz\" type=\"FrequencyChoice\"/>\n";
	out << "\t\t</xsd:restriction>\n";
	out << "\t</xsd:complexContent>\n";
	out << "</xsd:complexType>\n";
	out << "<xsd:complexType name=\"kbitChoice\">\n";
	out << "\t<xsd:complexContent>\n";
	out << "\t\t<xsd:restriction base=\"xsd:anyType\">\n";
	out << "\t\t\t<xsd:attribute name=\"kbits\" type=\"bitrateChoice\"/>\n";
	out << "\t\t</xsd:restriction>\n";
	out << "\t</xsd:complexContent>\n";
	out << "</xsd:complexType>\n";
	out << "<xsd:simpleType name=\"YesNoChoice\">\n";
	out << "\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t<xsd:enumeration value=\"Yes\"/>\n";
	out << "\t\t<xsd:enumeration value=\"No\"/>\n";
	out << "\t</xsd:restriction>\n";
	out << "</xsd:simpleType>\n";
	out << "<xsd:simpleType name=\"MultiEmphasisChoice\">\n";
	out << "\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t<xsd:enumeration value=\"No\"/>\n";
	out << "\t\t<xsd:enumeration value=\"50/15ms\"/>\n";
	out << "\t\t<xsd:enumeration value=\"reserviert\"/>\n";
	out << "\t\t<xsd:enumeration value=\"CCIT J.17\"/>\n";
	out << "\t</xsd:restriction>\n";
	out << "</xsd:simpleType>\n";
	out << "<xsd:simpleType name=\"FrequencyChoice\">\n";
	out << "\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t<xsd:enumeration value=\"44100\"/>\n";
	out << "\t\t<xsd:enumeration value=\"22050\"/>\n";
	out << "\t\t<xsd:enumeration value=\"11025\"/>\n";
	out << "\t\t<xsd:enumeration value=\"48000\"/>\n";
	out << "\t\t<xsd:enumeration value=\"24000\"/>\n";
	out << "\t\t<xsd:enumeration value=\"12000\"/>\n";
	out << "\t\t<xsd:enumeration value=\"32000\"/>\n";
	out << "\t\t<xsd:enumeration value=\"16000\"/>\n";
	out << "\t\t<xsd:enumeration value=\"8000\"/>\n";
	out << "\t</xsd:restriction>\n";
	out << "</xsd:simpleType>\n";
	out << "<xsd:simpleType name=\"bitrateChoice\">\n";
	out << "\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t<xsd:enumeration value=\"8\"/>\n";
	out << "\t\t<xsd:enumeration value=\"16\"/>\n";
	out << "\t\t<xsd:enumeration value=\"24\"/>\n";
	out << "\t\t<xsd:enumeration value=\"32\"/>\n";
	out << "\t\t<xsd:enumeration value=\"40\"/>\n";
	out << "\t\t<xsd:enumeration value=\"48\"/>\n";
	out << "\t\t<xsd:enumeration value=\"56\"/>\n";
	out << "\t\t<xsd:enumeration value=\"64\"/>\n";
	out << "\t\t<xsd:enumeration value=\"80\"/>\n";
	out << "\t\t<xsd:enumeration value=\"96\"/>\n";
	out << "\t\t<xsd:enumeration value=\"112\"/>\n";
	out << "\t\t<xsd:enumeration value=\"128\"/>\n";
	out << "\t\t<xsd:enumeration value=\"144\"/>\n";
	out << "\t\t<xsd:enumeration value=\"160\"/>\n";
	out << "\t\t<xsd:enumeration value=\"192\"/>\n";
	out << "\t\t<xsd:enumeration value=\"224\"/>\n";
	out << "\t\t<xsd:enumeration value=\"256\"/>\n";
	out << "\t\t<xsd:enumeration value=\"288\"/>\n";
	out << "\t\t<xsd:enumeration value=\"320\"/>\n";
	out << "\t\t<xsd:enumeration value=\"352\"/>\n";
	out << "\t\t<xsd:enumeration value=\"384\"/>\n";
	out << "\t\t<xsd:enumeration value=\"416\"/>\n";
	out << "\t\t<xsd:enumeration value=\"448\"/>\n";
	out << "\t</xsd:restriction>\n";
	out << "</xsd:simpleType>\n";
	out << "<xsd:simpleType name=\"genreChoice\">\n";
	out << "\t<xsd:restriction base=\"xsd:string\">\n";
	out << "\t\t<xsd:enumeration value=\"Blues\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Classic Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Country\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Dance\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Disco\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Funk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Grunge\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Hip-Hop\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Jazz\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Metal\"/>\n";
	out << "\t\t<xsd:enumeration value=\"New Age\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Oldies\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Other\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Pop\"/>\n";
	out << "\t\t<xsd:enumeration value=\"R&amp;B\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Rap\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Raggae\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Techno\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Industrial\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Alternative\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Ska\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Death Metal\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Pranks\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Soundtrack\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Euro-Techno\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Ambient\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Trip-Hop\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Vocal\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Jazz+Funk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Fusion\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Trance\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Classical\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Instrumental\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Acid\"/>\n";
	out << "\t\t<xsd:enumeration value=\"House\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Game\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Sound Clip\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Gospel\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Noise\"/>\n";
	out << "\t\t<xsd:enumeration value=\"AlternRock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Bass\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Soul\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Punk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Space\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Meditative\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Instrumental Pop\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Instrumental Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Ethnic\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Gothic\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Darkwave\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Techno-Industrial\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Electronic\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Pop-Folk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Eurodance\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Dream\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Southern Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Comedy\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Cult\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Gangsta\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Top 40\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Christian Rap\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Pop/Funk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Jungle\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Native American\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Cabaret\"/>\n";
	out << "\t\t<xsd:enumeration value=\"New Wave\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Psychadelic\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Rave\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Showtunes\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Trailer\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Lo-Fi\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Tribal\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Acid Punk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Acid Jazz\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Polka\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Retro\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Musical\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Rock &amp; Roll\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Hard Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Folk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Folk/Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"National Folk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Swing\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Fusion\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Bebob\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Latin\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Revival\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Celtic\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Bluegrass\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Avantgarde\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Gothic Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Progress. Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Psychadel. Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Symphonic Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Slow Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Big Band\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Chorus\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Easy Listening\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Acoustic\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Humour\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Speech\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Chanson\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Opera\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Chamber Music\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Sonata\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Symphony\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Booty Bass\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Primus\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Porn Groove\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Satire\"/>\n";
	out << "\t\t<!-- added 1999 26 April by Ben Gertzfield -->\n";
	out << "\t\t<xsd:enumeration value=\"Slow Jam\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Club\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Tango\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Samba\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Folklore\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Ballad\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Power Ballad\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Rhythmic Soul\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Freestyle\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Duet\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Punk Rock\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Drum Solo\"/>\n";
	out << "\t\t<xsd:enumeration value=\"A Capella\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Euro-House\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Dance Hall\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Goa\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Drum &amp; Bass\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Club-House\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Hardcore\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Terror\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Indie\"/>\n";
	out << "\t\t<xsd:enumeration value=\"BritPop\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Negerpunk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Polsk Punk\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Beat\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Christian Gangsta Rap\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Heavy Metal\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Black Metal\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Crossover\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Contemporary Christian\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Christian Rock\"/>\n";
	out << "\t\t<!-- winamp 1.91 genres -->\n";
	out << "\t\t<xsd:enumeration value=\"Merengue\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Salsa\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Thrash Metal\"/>\n";
	out << "\t\t<!-- winamp 1.92 genres -->\n";
	out << "\t\t<xsd:enumeration value=\"Anime\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Jpop\"/>\n";
	out << "\t\t<xsd:enumeration value=\"SynthPop\"/>\n";
	out << "\t\t<xsd:enumeration value=\"Unknown\"/>\n";
	out << "\t</xsd:restriction>\n";
	out << "</xsd:simpleType>\n";
	out << "</xsd:schema>\n";

	out.close();
	return 0;
}

int xsl_stylesheet(char *filename)
{
	std::ofstream out(filename);

	if (!out.is_open()) {
		return -1;
	}

	out << "<!-- xsl-file dated: 2003-11-09 -->\n";
	out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
	out << "<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" version=\"1.0\">\n";
	out << "<xsl:template match=\"/\">\n";
	out << "<html>\n";
	out << "\t<head>\n";
	out << "\t\t<title>MP3-Leser</title>\n";
	out << "\t</head>\n";
	out << "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"index.css\"/>\n";
	out << "\t<body>\n";
	out << "\t\t<br/><br/>\n";
	out << "\t\t<center>\n";
	out << "\t\t\t<u><b>Track List</b></u>\n";
	out << "\t\t</center>\n";
	out << "\t\t<br/><br/>\n";
	out << "\t\t<table align=\"center\" width=\"1000\">\n";
	out << "\t\t\t<tr bgcolor=\"#CCCCCC\">\n";
	out << "\t\t\t\t<td width=\"20\"><b>Track</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Artist</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Title</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Album</b></td>\n";
	out << "\t\t\t\t<td width=\"50\"><b>Year</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Genre</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Comment</b></td>\n";
	out << "\t\t\t\t<td width=\"50\"><b>Length</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Filename</b></td>\n";
	out << "\t\t\t</tr>\n";
	out << "\t\t\t<tr>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t</tr>\n";
	out << "\t\t\t<xsl:for-each select=\"tag-info/track\">\n";
	out << "\t\t\t<xsl:if test=\"position() mod 2 = 0\">\n";
	out << "\t\t\t\t<tr class=\"white\">\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"@number\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"artist\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"title\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"album\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"album/@year\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"album/@genre\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"comment\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"title/@length\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"filename\"/></td>\n";
	out << "\t\t\t\t</tr>\n";
	out << "\t\t\t</xsl:if>\n";
	out << "\t\t\t<xsl:if test=\"position() mod 2 = 1\">\n";
	out << "\t\t\t\t<tr class=\"gray\">\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"@number\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"artist\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"title\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"album\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"album/@year\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"album/@genre\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"comment\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"title/@length\"/></td>\n";
	out << "\t\t\t\t\t<td><xsl:value-of select=\"filename\"/></td>\n";
	out << "\t\t\t\t</tr>\n";
	out << "\t\t\t</xsl:if>\n";
	out << "\t\t\t</xsl:for-each>\n";
	out << "\t\t</table>\n";
	out << "\t</body>\n";
	out << "</html>\n";
	out << "</xsl:template>\n";
	out << "</xsl:stylesheet>\n";

	out.close();
	return 0;
}

int xml_head(char *filename)
{
	std::ofstream out(filename);

	if (!out.is_open()) {
		return -1;
	}

	out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
	out << "<?xml-stylesheet type=\"text/xsl\" href=\"index.xsl\"?>";
	out << "<tag-info xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"index.xsd\">\n";

	out.close();
	return 0;
}

int xml_content(tags *tagger, header *mp3header, char *filename)
{
	std::ofstream out(filename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	out << "\t<track number=\"" << tagger->trackno << "\">\n";
	out << "\t\t<artist>" << tagger->artist << "</artist>\n";
	out << "\t\t<title length=\"" << tagger->length << "\">" << tagger->title << "</title>\n";
	out << "\t\t<album year=\"" << tagger->year << "\" genre=\"" << genre_string[tagger->genre] << "\">" << tagger->album << "</album>\n";
	out << "\t\t<comment>" << tagger->comment << "</comment>\n";
	out << "\t\t<filename>" << tagger->filename << "</filename>\n";
	out << "\t\t<header-info>\n";
	out << "\t\t\t<bitrate kbits=\"" << mp3header->Bitrate << "\"/>\n";
	out << "\t\t\t<mpeg-audio-version layer=\"" << mp3header->Layer << "\">" << mp3header->MPEG_Audio_Version << "</mpeg-audio-version>\n";
	out << "\t\t\t<error-protection choice=\"" << mp3header->Error_Protection << "\"/>\n";
	out << "\t\t\t<samplerate Hz=\"" << mp3header->Samplerate << "\"/>\n";
	out << "\t\t\t<private choice=\"" << mp3header->Private << "\"/>\n";
	out << "\t\t\t<channel-mode>" << mp3header->Channel_Mode << "</channel-mode>\n";
	out << "\t\t\t<copyright choice=\"" << mp3header->Copyright << "\"/>\n";
	out << "\t\t\t<original choice=\"" << mp3header->Original << "\"/>\n";
	out << "\t\t\t<emphasis choice=\"" << mp3header->Emphasis << "\"/>\n";
	out << "\t\t</header-info>\n";
	out << "\t</track>\n";

	out.close();
	return 0;
}

int xml_foot(char *filename)
{
	std::ofstream out(filename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	out << "</tag-info>\n";

	out.close();
	return 0;
}
