#include <vector>
#include <sstream>
#include <stdlib.h>

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegheader.h>
#include <taglib/mpegproperties.h>

#include "MP3Collection.hpp"
#include "MP3Infos.hpp"
#include "xml.h"
#include "css.h"

using namespace std;

void writeXmlFile(Tag2Html::MP3Collection* mp3Collection, bool outputXSL, bool outputXSD)
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

	ostringstream convert;
	list<Tag2Html::MP3Infos*> sortedList = mp3Collection->getSortedList();
	for (list<Tag2Html::MP3Infos*>::iterator mp3info = sortedList.begin(); mp3info != sortedList.end(); mp3info++) {
		DOMElement* track = doc->createElement(XMLString::transcode("track"));
		convert.str("");
		convert << (*mp3info)->track;
		track->setAttribute(XMLString::transcode("number"), XMLString::transcode(convert.str().c_str()));

		DOMElement* artist = doc->createElement(XMLString::transcode("artist"));
		artist->setTextContent(XMLString::transcode((*mp3info)->artist.c_str()));
		track->appendChild(artist);

		DOMElement* title = doc->createElement(XMLString::transcode("title"));
		title->setTextContent(XMLString::transcode((*mp3info)->title.c_str()));
		if ((*mp3info)->length < 3600) {
			char trackLength[12];
			struct tm* timeinfo = new tm();

			int seconds = (*mp3info)->length;
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

		convert.str("");
		convert << (*mp3info)->year;
		DOMElement* album = doc->createElement(XMLString::transcode("album"));
		album->setTextContent(XMLString::transcode((*mp3info)->album.c_str()));
		album->setAttribute(XMLString::transcode("year"), XMLString::transcode(convert.str().c_str()));
		album->setAttribute(XMLString::transcode("genre"), XMLString::transcode((*mp3info)->genre.c_str()));
		track->appendChild(album);

		DOMElement* comment = doc->createElement(XMLString::transcode("comment"));
		comment->setTextContent(XMLString::transcode((*mp3info)->comment.c_str()));
		track->appendChild(comment);

		DOMElement* filename = doc->createElement(XMLString::transcode("filename"));
		filename->setTextContent(XMLString::transcode((*mp3info)->filename.c_str()));
		track->appendChild(filename);

		DOMElement* headerInfo = doc->createElement(XMLString::transcode("header-info"));

		const char* isCopyrighted = ((*mp3info)->isCopyrighted) ? "1" : "0";
		const char* isOriginal = ((*mp3info)->isOriginal) ? "1" : "0";
		const char* protectionEnabled = ((*mp3info)->protectionEnabled) ? "1" : "0";
		headerInfo->setAttribute(XMLString::transcode("is_copyrighted"), XMLString::transcode(isCopyrighted));
		headerInfo->setAttribute(XMLString::transcode("is_original"), XMLString::transcode(isOriginal));
		headerInfo->setAttribute(XMLString::transcode("protection_enabled"), XMLString::transcode(protectionEnabled));

		DOMElement* bitrate = doc->createElement(XMLString::transcode("bitrate"));
		convert.str("");
		convert << (*mp3info)->bitrate;
		bitrate->setAttribute(XMLString::transcode("kbits"), XMLString::transcode(convert.str().c_str()));
		headerInfo->appendChild(bitrate);

		DOMElement* sampleRate = doc->createElement(XMLString::transcode("samplerate"));
		convert.str("");
		convert << (*mp3info)->samplerate;
		sampleRate->setAttribute(XMLString::transcode("hz"), XMLString::transcode(convert.str().c_str()));
		headerInfo->appendChild(sampleRate);

		DOMElement* channels = doc->createElement(XMLString::transcode("channels"));
		convert.str("");
		convert << (*mp3info)->channels;
		channels->setTextContent(XMLString::transcode(convert.str().c_str()));
		const char* channelMode = "SingleChannel";
		switch ((*mp3info)->channelMode) {
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
		convert.str("");
		convert << (*mp3info)->layer;
		mpegAudioVersion->setAttribute(XMLString::transcode("layer"), XMLString::transcode(convert.str().c_str()));
		const char* version = "1";
		switch ((*mp3info)->version) {
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

	DOMLSSerializer* writer = ((DOMImplementationLS*)domImplementation)->createLSSerializer();
	//if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
	//	writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	//}
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("index.xml");
	DOMLSOutput* theOutput = ((DOMImplementationLS*)domImplementation)->createLSOutput();
    theOutput->setByteStream(fileFormatTarget);
	writer->write(doc, theOutput);
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

	DOMLSSerializer* writer = ((DOMImplementationLS*)domImplementation)->createLSSerializer();
	//if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
	//	writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	//}
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("index.xsd");
	DOMLSOutput* theOutput = ((DOMImplementationLS*)domImplementation)->createLSOutput();
    theOutput->setByteStream(fileFormatTarget);
	writer->write(doc, theOutput);
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

	DOMLSSerializer* writer = ((DOMImplementationLS*)domImplementation)->createLSSerializer();
	//if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
	//	writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	//}
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("index.xsl");
	DOMLSOutput* theOutput = ((DOMImplementationLS*)domImplementation)->createLSOutput();
    theOutput->setByteStream(fileFormatTarget);
	writer->write(doc, theOutput);
	fileFormatTarget->flush();
	writer->release();

	doc->release();
	XMLPlatformUtils::Terminate();

	writeCssFile();
}
