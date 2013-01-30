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

#include "xml.h"

using namespace std;

int xsd_schema(char *filename)
{
	std::ofstream out(filename);

	if (!out.is_open()) {
		return -1;
	}

	out << "<!-- xsd-file dated: 2003-11-09 -->\n";
	out << "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n";
	out << "<!-- xsd-file dated 2003-09-12 -->\n";
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
	out << "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n";
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

	out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n";
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
