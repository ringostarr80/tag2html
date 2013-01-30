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

#ifndef CLASSES_H
#define CLASSES_H

extern const char *genre_string[];

/*
 *  class html
 *  {
 *    public:
 *    char htmlfilename[ 1024 ];	// dateiname der html-output-datei
 *    int layout_flags;		// bestimmungen fuer das aussehen...
 *    // layout_flags = 0;	tracklist + weisser hg
 *    // layout_flags = 1;	tracklist + grauer hg
 *    // layout_flags = 2;	album-page + weisser hg
 *    // layout_flags = 3;	album-page + grauer hg
 *  };
 *
 */

class tags
{
	public:
		char TAG[3];
		char filename[1024];
		char title[1024];
		char artist[1024];
		char album[1024];
		char year[1024];
		char comment[1024];
		char length[1024];
		int layout_flag;
		unsigned short genre;
		unsigned short trackno;
};

class html
{
	public:
		char cssfilename[1024];
		char htmlfilename[1024];
		char statfilename[1024];
		char infofilename[1024];
};

class stats
{
	public:
		stats();
		char tot_length[1024];
		char album[1024][1024];
		char artist[1024][1024];

		int tot_alb_count;
		int tot_art_count;

		int alb_count;
		int art_count;
		int mp3_count;

		float tot_filesize;
};

#endif /* CLASSES_H */
