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

#include "html.h"

int html_css(html *output)
{
	std::ofstream out(output->cssfilename);

	if (!out.is_open()) {
		return -1;
	}

	out << "<!-- css-file dated: 2003-11-09 -->\n";
	out << "body\n";
	out << "{\n";
	out << "\tfont-family: sans-serif;\n";
	out << "\tfont-size:  10px;\n";
	out << "\tbackground-color: #ABABAB;\n";
	out << "\ttext=#303030;\n";
	out << "}\n";
	out << "table\n";
	out << "{\n";
	out << "\tbackground-color: #FFFFFF;\n";
	out << "\tborder: \"thin solid black\";\n";
	out << "\tfont-family: sans-serif;\n";
	out << "\tfont-size: 10px;\n";
	out << "}\n";
	out << "table.nb\n";
	out << "{\n";
	out << "\tbackground-color: #FFFFFF;\n";
	out << "\tborder: thin solid white;\n";
	out << "\tfont-family: sans-serif;\n";
	out << "\tfont-size: 10px;\n";
	out << "}\n";
	out << "tr.white\n";
	out << "{\n";
	out << "\tfont-family: sans-serif;\n";
	out << "\tfont-size: 10px;\n";
	out << "\tbackground-color: white;\n";
	out << "}\n";
	out << "tr.gray\n";
	out << "{\n";
	out << "\tfont-family: sans-serif;\n";
	out << "\tfont-size: 10px;\n";
	out << "\tbackground-color: #EEEEEE;\n";
	out << "}\n";
	out << "a\n";
	out << "{\n";
	out << "\tcolor: #444444;\n";
	out << "}\n";

	out.close();
	return 0;
}

int html_head(html *output, tags *mytag)
{
	std::ofstream out(output->htmlfilename);

	if (!out.is_open()) {
		return -1;
	}

	if (mytag->layout_flag > 1) {
		out << "<html>\n";
		out << "\t<head>\n";
		out << "\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"./index.css\">\n";
		out << "\t\t<title>MP3-Leser</title>\n";
		out << "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html\" charset=\"iso-8859-1\">\n";
		out << "\t</head>\n";
		out << "\t<body>\n";
		out << "\t\t<br><br>\n";
		out << "\t\t<center>\n";
		out << "\t\t\t<u><b>List of Albums</b></u>\n";
		out << "\t\t</center>\n";
		out << "\t\t<br><br>\n";
		out << "\t\t<table align=\"center\" width=\"500\">\n";
		out << "\t\t\t<tr bgcolor=\"#CCCCCC\">\n";
		out << "\t\t\t\t<td>Album</td>\n";
		out << "\t\t\t\t<td>Artist</td>\n";
		out << "\t\t\t</tr>\n";
		out << "\t\t\t<tr class=\"white\">\n";
		out << "\t\t\t\t<td></td>\n";
		out << "\t\t\t\t<td></td>\n";
		out << "\t\t\t</tr>\n";
	} else {
		out << "<html>\n";
		out << "\t<head>\n";
		out << "\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"./index.css\">\n";
		out << "\t\t<title>MP3-Leser</title>\n";
		out << "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html\" charset=\"iso-8859-1\">\n";
		out << "\t</head>\n";
		out << "\t<body>\n";
		out << "\t\t<br><br>\n";
		out << "\t\t<center>\n";
		out << "\t\t\t<u><b>Track List</b></u> ( view <a href=\"./stats.html\"><b>stats</b></a>, <a href=\"./info.html\"><b>info</b></a> )\n";
		out << "\t\t</center>\n";
		out << "\t\t<br><br>\n";
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
	}

	out.close();
	return 0;
}

int html_content(tags *tagger, html *output)
{
	std::ofstream out(output->htmlfilename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	if (tagger->layout_flag < 2) {
		if (tagger->layout_flag == 1) {
			out << "\t\t\t<tr class=\"gray\">\n";
		}
		if (tagger->layout_flag == 0) {
			out << "\t\t\t<tr class=\"white\">\n";
		}

		if (tagger->trackno < 10) {
			out << "\t\t\t\t<td>0" << tagger->trackno << "</td>\n";
		} else {
			if (tagger->trackno != 0) {
				out << "\t\t\t\t<td>" << tagger->trackno << "</td>\n";
			}
		}
		out << "\t\t\t\t<td>" << tagger->artist << "</td>\n";
		out << "\t\t\t\t<td>" << tagger->title << "</td>\n";
		out << "\t\t\t\t<td>" << tagger->album << "</td>\n";
		out << "\t\t\t\t<td>" << tagger->year << "</td>\n";
		out << "\t\t\t\t<td>" << genre_string[tagger->genre] << "</td>\n";
		if ((strstr(tagger->comment, "http://") != NULL) || (strstr(tagger->comment, "www.") != NULL)) {
			out << "\t\t\t\t<td><a href=\"" << tagger->comment << "\">" << tagger->comment << "</a></td>\n";
		} else {
			out << "\t\t\t\t<td>" << tagger->comment << "</td>\n";
		}

		out << "\t\t\t\t<td>" << tagger->length << "</td>\n";
		out << "\t\t\t\t<td>" << tagger->filename << "</td>\n";
		out << "\t\t\t</tr>\n";
	}

	if (tagger->layout_flag > 1) {
		if (tagger->layout_flag == 2) {
			out << "\t\t\t<tr class=\"white\">\n";
		}
		if (tagger->layout_flag == 3) {
			out << "\t\t\t<tr class=\"gray\">\n";
		}

		out << "\t\t\t\t<td><a href=\"./" << tagger->album << ".html\">" << tagger->album << "</a></td>\n";
		out << "\t\t\t\t<td>" << tagger->artist << "</td>\n";
	}

	out.close();
	return tagger->layout_flag;
}

int html_foot(html *output)
{
	std::ofstream out(output->htmlfilename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	out << "\t\t</table>\n";
	out << "\t</body>\n";
	out << "</html>\n";

	out.close();
	return 0;
}
