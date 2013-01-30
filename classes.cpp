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

#include "classes.h"

// table for genres
const char *genre_string[] = {
	"Blues",          // 0
	"Classic Rock",
	"Country",
	"Dance",
	"Disco",
	"Funk",           // 5
	"Grunge",
	"Hip-Hop",
	"Jazz",
	"Metal",
	"New Age",        // 10
	"Oldies",
	"Other",
	"Pop",
	"R&B",
	"Rap",            // 15
	"Raggae",
	"Rock",
	"Techno",
	"Industrial",
	"Alternative",    // 20
	"Ska",
	"Death Metal",
	"Pranks",
	"Soundtrack",
	"Euro-Techno",    // 25
	"Ambient",
	"Trip-Hop",
	"Vocal",
	"Jazz+Funk",
	"Fusion",         // 30
	"Trance",
	"Classical",
	"Instrumental",
	"Acid",
	"House",          // 35
	"Game",
	"Sound Clip",
	"Gospel",
	"Noise",
	"AlternRock",     // 40
	"Bass",
	"Soul",
	"Punk",
	"Space",
	"Meditative",     // 45
	"Instrumental Pop",
	"Instrumental Rock",
	"Ethnic",
	"Gothic",
	"Darkwave",       // 50
	"Techno-Industrial",
	"Electronic",
	"Pop-Folk",
	"Eurodance",
	"Dream",          // 55
	"Southern Rock",
	"Comedy",
	"Cult",
	"Gangsta",
	"Top 40",         // 60
	"Christian Rap",
	"Pop/Funk",
	"Jungle",
	"Native American",
	"Cabaret",        // 65
	"New Wave",
	"Psychadelic",
	"Rave",
	"Showtunes",
	"Trailer",        // 70
	"Lo-Fi",
	"Tribal",
	"Acid Punk",
	"Acid Jazz",
	"Polka",          // 75
	"Retro",
	"Musical",
	"Rock & Roll",
	"Hard Rock",
	"Folk",           // 80
	"Folk/Rock",
	"National Folk",
	"Swing",
	"Fusion",
	"Bebob",          // 85
	"Latin",
	"Revival",
	"Celtic",
	"Bluegrass",
	"Avantgarde",     // 90
	"Gothic Rock",
	"Progress. Rock",
	"Psychadel. Rock",
	"Symphonic Rock",
	"Slow Rock",      // 95
	"Big Band",
	"Chorus",
	"Easy Listening",
	"Acoustic",
	"Humour",         // 100
	"Speech",
	"Chanson",
	"Opera",
	"Chamber Music",
	"Sonata",         // 105
	"Symphony",
	"Booty Bass",
	"Primus",
	"Porn Groove",
	"Satire",         // 110

	// added 1999 26 Apr by Ben Gertzfield
	"Slow Jam",
	"Club",
	"Tango",
	"Samba",
	"Folklore",       // 115
	"Ballad",
	"Power Ballad",
	"Rhythmic Soul",
	"Freestyle",
	"Duet",           // 120
	"Punk Rock",
	"Drum Solo",
	"A Capella",
	"Euro-House",
	"Dance Hall",     // 125
	"Goa",
	"Drum & Bass",
	"Club-House",
	"Hardcore",
	"Terror",         // 130
	"Indie",
	"BritPop",
	"Negerpunk",
	"Polsk Punk",
	"Beat",           // 135
	"Christian Gangsta Rap", // DJ JC in da house
	"Heavy Metal",
	"Black Metal",
	"Crossover",
	"Contemporary Christian", // 140
	"Christian Rock",

	// winamp 1.91 genres
	"Merengue",
	"Salsa",
	"Thrash Metal",

	// winamp 1.92 genres
	"Anime",          // 145
	"Jpop",
	"SynthPop",
	"Unknown",        // 148
};
