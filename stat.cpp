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

#include "stat.h"

stats::stats()
{
	tot_alb_count = 0;
	tot_art_count = 0;

	alb_count = 0;
	art_count = 0;
	mp3_count = 0;
}

void get_albumcount(char *album, stats *mystat)
{
	bool isnew=true;
	//if( mystat->alb_count == 0 )
	//  strncpy( mystat->album[ mystat->alb_count ], album, sizeof( mystat->album[ mystat->alb_count ] ) );

	for (int i = 0; i < mystat->alb_count; i++) {
		if (!strcmp(mystat->album[i], album)) {
			isnew=false;
		}
	}

	if (isnew == true) {
		strncpy(mystat->album[mystat->alb_count], album, sizeof(mystat->album[mystat->alb_count]));
		mystat->alb_count++;
	}

	mystat->tot_alb_count++;
}

void get_artistcount( char *artist, stats *mystat )
{
	bool isnew = true;
	if (mystat->art_count == 0) {
		strncpy(mystat->artist[mystat->art_count], artist, sizeof(mystat->artist[mystat->art_count]));
	}

	for (int i = 0; i < mystat->art_count; i++) {
		if (!strcmp(mystat->artist[i], artist)) {
			isnew = false;
		}
	}

	if (isnew == true) {
		strncpy(mystat->artist[mystat->art_count], artist, sizeof(mystat->artist[mystat->art_count]));
		mystat->art_count++;
	}

	mystat->tot_art_count++;
}

int stat_head(html *output)
{
	std::ofstream out(output->statfilename);

	if (!out.is_open()) {
		return -1;
	}

	out << "<html>\n";
	out << "\t<head>\n";
	out << "\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"./index.css\">\n";
	out << "\t\t<title>tag2html stat page</title>\n";
	out << "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html\" charset=\"iso-8859-1\">\n";
	out << "\t</head>\n";
	out << "\t<body>\n";
	out << "\t\t<br><br><br>\n";
	out << "\t\t<table align=\"center\" width=\"300\">\n";
	out << "\t\t\t<tr valign=\"top\" bgcolor=\"#CCCCCC\">\n";
	out << "\t\t\t\t<td align=\"center\">\n";
	out << "\t\t\t\t\t<b>Statistics</b>\n";
	out << "\t\t\t\t</td>\n";
	out << "\t\t\t</tr>\n";
	out << "\t\t\t<tr>\n";
	out << "\t\t\t\t<td>\n";
	out << "\t\t\t\t</td>\n";
	out << "\t\t\t</tr>\n";
	out << "\t\t\t<tr>\n";
	out << "\t\t\t\t<td>\n";
	out << "\t\t\t\t\t<table class=\"nb\">\n";

	out.close();
	return 0;
}

int stat_content(stats *mystat, html *output)
{
	std::ofstream out(output->statfilename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	//mystat->ipop_genre = ( ( 100 * mystat->gen_count ) / mystat->tot_gen_count );

	out << "\t\t\t\t\t\t<tr>\n";
	out << "\t\t\t\t\t\t\t<td><b>File Type:</b></td>\n";
	out << "\t\t\t\t\t\t\t<td>" << "MP3" << "</td>\n";
	out << "\t\t\t\t\t\t</tr>\n";   // for later use!
	out << "\t\t\t\t\t\t<tr><td><b>File Count:</b></td>\n";
	out << "\t\t\t\t\t\t\t<td>" << mystat->mp3_count << "</td>\n";
	out << "\t\t\t\t\t\t</tr>\n";
	out << "\t\t\t\t\t\t<tr>\n";
	out << "\t\t\t\t\t\t\t<td><b>Artist Count:</b></td>\n";
	out << "\t\t\t\t\t\t\t<td>" << mystat->art_count << "</td>\n";
	out << "\t\t\t\t\t\t</tr>\n";
	out << "\t\t\t\t\t\t<tr>\n";
	out << "\t\t\t\t\t\t\t<td><b>Album Count:</b></td>\n";
	out << "\t\t\t\t\t\t\t<td>" << mystat->alb_count << "</td>\n";
	out << "\t\t\t\t\t\t</tr>\n";
	out << "\t\t\t\t\t\t<tr>\n";
	out << "\t\t\t\t\t\t\t<td><b>Total Size:</b></td>\n";
	out << "\t\t\t\t\t\t\t<td>" << ( mystat->tot_filesize/(1024*1024)) << " MByte</td>\n";
	out << "\t\t\t\t\t\t</tr>\n";
	out << "\t\t\t\t\t\t<tr>\n";
	out << "\t\t\t\t\t\t\t<td><b>Total Length:</b></td>\n";
	out << "\t\t\t\t\t\t\t<td> ~ " << mystat->tot_length << "</td>\n";
	out << "\t\t\t\t\t\t</tr>\n";

	out.close();
	return 0;
}

int stat_foot(html *output)
{
	std::ofstream out(output->statfilename, std::ios::app);

	if (!out.is_open()) {
		return -1;
	}

	out << "\t\t\t\t\t</table>\n";
	out << "\t\t\t\t</td>\n";
	out << "\t\t\t</tr>\n";
	out << "\t\t</table>\n";
	out << "\t\t<br><br><br>\n";
	out << "\t</body>\n";
	out << "</html>\n";

	out.close();
	return 0;
}
