/***************************************************************************
                          gettag.cpp  -  description
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

#include "gettag.h"

void tag_kuerzen(char* tag, int zeichenanzahl)
{
	bool end_found=false;
	for (int i = zeichenanzahl; i > 0; i--) {
		if (tag[i] != ' ') {
			tag[i + 1] = '\0';
			i = 0;
			end_found = true;
		}
	}
	if (end_found == false) {
		tag[0] = '\0';
	}
}

int get_mp3tags(char *filename_ptr, tags *mp3tags)
{
	std::ifstream mp3_datei;

	mp3_datei.open(filename_ptr, std::ios::in);
	if (!mp3_datei) {
		std::cerr << "error: could not open '" << filename_ptr << "' to read tags!" << std::endl;
		return -1;
	} else { // ans Ende der Datei gehen!
		mp3_datei.seekg(-128, std::ios::end);
	}

	// überprüfen ob TAG gesetzt ist!
	// 1.-3. Byte
	for (int i = 0; i <= 2; i++) {
		mp3_datei.get(mp3tags->TAG[i]);
	}
	if (mp3tags->TAG[0] == 'T' && mp3tags->TAG[1] == 'A' && mp3tags->TAG[2] == 'G') {
		// Titel
		// 4.-33. Byte
		for (int i = 0; i <= 29; i++) {
			mp3_datei.get(mp3tags->title[i]);
		}
		tag_kuerzen(mp3tags->title, 29);

		// Artist
		// 34.-63. Byte
		for (int i = 0; i <= 29; i++) {
			mp3_datei.get(mp3tags->artist[i]);
		}
		tag_kuerzen(mp3tags->artist, 29);

		// Album
		// 64.-93.Byte
		for (int i = 0; i <= 29; i++) {
			mp3_datei.get(mp3tags->album[i]);
		}
		tag_kuerzen(mp3tags->album, 29);

		// Jahr
		// 94.-97.Byte
		for (int i = 0; i <= 3; i++) {
			mp3_datei.get(mp3tags->year[i]);
		}

		// Kommentar
		// 98.-127. Byte
		for (int i = 0; i <= 29; i++) {
			mp3_datei.get(mp3tags->comment[i]);
		}
		tag_kuerzen(mp3tags->comment, 27);

		// Tracknummer, nur wenn vorletztes Zeichen des Kommentars leer ist!
		// 127. Byte
		if (mp3tags->comment[28] == 0) {
			mp3tags->trackno = mp3tags->comment[29]; // wenn 0, dann unbekannt!
		}

		// Genre
		// 128. Byte
		unsigned short genre_byte;
		genre_byte = mp3_datei.get();
		if (genre_byte > 148) {
			genre_byte = 148;
		}
		mp3tags->genre = genre_byte;
	} else {
		std::cout << std::endl << "no mp3v1-TAGS found in " << filename_ptr << "." << std::endl;
	}

	return 0;
}
