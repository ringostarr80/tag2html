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

#include <iostream>
#include <fstream>

#include "classes.h"
#include "tag.h"

int info_head(char *filename)
{
	std::ofstream out(filename);

	if (!out.is_open()) {
		return -1;
	}

	out << "<html>\n";
	out << "\t<head>\n";
	out << "\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"./index.css\">\n";
	out << "\t\t<title>tag2html info page</title>\n";
	out << "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html\" charset=\"iso-8859-1\">\n";
	out << "\t</head>\n";
	out << "\t<body>\n";
	out << "\t\t<br><br>\n";
	out << "\t\t<center>\n";
	out << "\t\t\t<u><b>File Info</b></u>\n";
	out << "\t\t</center>\n";
	out << "\t\t<br><br>\n";
	out << "\t\t<table align=\"center\" width=\"1000\">\n";
	out << "\t\t\t<tr bgcolor=\"#CCCCCC\">\n";
	out << "\t\t\t\t<td width=\"100\"><b>Filename</b></td>\n";
	out << "\t\t\t\t<td width=\"50\"><b>Bitrate</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>MPEG Audio Version</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Layer</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Error Protection</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Sampling Rate</b></td>\n";
	out << "\t\t\t\t<td width=\"50\"><b>Private</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Channel Mode</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Copyright</b></td>\n";
	out << "\t\t\t\t<td width=\"50\"><b>Original</b></td>\n";
	out << "\t\t\t\t<td width=\"100\"><b>Emphasis</b></td>\n";
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
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t\t<td></td>\n";
	out << "\t\t\t</tr>\n";

	out.close();
	return 0;
}

int info_body(char *filename, header *myheader, tags *mytag)
{
	std::ofstream out(filename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	if (mytag->layout_flag == 1) {
		out << "\t\t\t<tr class=\"gray\">\n";
	}
	if (mytag->layout_flag == 0) {
		out << "\t\t\t<tr class=\"white\">\n";
	}

	out << "\t\t\t\t<td>" << mytag->filename << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Bitrate << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->MPEG_Audio_Version << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Layer << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Error_Protection << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Samplerate << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Private << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Channel_Mode << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Copyright << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Original << "</td>\n";
	out << "\t\t\t\t<td>" << myheader->Emphasis << "</td>\n";
	out << "\t\t\t</tr>\n";

	out.close();
	return mytag->layout_flag;
}

int info_foot(char *filename)
{
	std::ofstream out(filename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	out << "\t\t</table>\n";
	out << "\t\t<br><br><br>\n";
	out << "\t</body>\n";
	out << "</html>\n";

	out.close();
	return 0;
}
