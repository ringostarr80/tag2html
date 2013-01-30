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

#include "sort.h"

mp3_sort::mp3_sort() {
	s_tag = new tags[1024];
	cur_index = -1;
}

mp3_sort::~mp3_sort() {
	delete s_tag;
}

void mp3_sort::make_array(tags *mytag) {
	strncpy(s_tag[cur_index].filename, mytag->filename, sizeof(s_tag[cur_index].filename));
	strncpy(s_tag[cur_index].title, mytag->title, sizeof(s_tag[cur_index].title));
	strncpy(s_tag[cur_index].artist, mytag->artist, sizeof(s_tag[cur_index].artist));
	strncpy(s_tag[cur_index].album, mytag->album, sizeof(s_tag[cur_index].album));
	strncpy(s_tag[cur_index].year, mytag->year, sizeof(s_tag[cur_index].year));
	strncpy(s_tag[cur_index].comment, mytag->comment, sizeof(s_tag[cur_index].comment));
	strncpy(s_tag[cur_index].length, mytag->length, sizeof(s_tag[cur_index].length));
	s_tag[cur_index].genre = mytag->genre;
	s_tag[cur_index].trackno = mytag->trackno;
	s_tag[cur_index].layout_flag = mytag->layout_flag;
}

int mp3_sort::strLess(char *one, char *two) {
	return strcmp(one, two);
}

int mp3_sort::strSwap(char *one, char *two) {
	char *tmp;
	tmp=new char[1024];

	strcpy(tmp, one);
	strcpy(one, two);
	strcpy(two, tmp);

	one[strlen(one)]=0;
	two[strlen(two)]=0;

	delete tmp;
	return 0;
}

void mp3_sort::sort_tag(void) {
	int temp;

	// Bubblesort
	for (int i = 0; i <= cur_index; i++) {
		for (int j = cur_index; j > i; j--) {
			if (s_tag[j-1].trackno>s_tag[j].trackno) {
				strSwap(s_tag[j].artist, s_tag[j-1].artist);
				strSwap(s_tag[j].filename, s_tag[j-1].filename);
				strSwap(s_tag[j].title, s_tag[j-1].title);
				strSwap(s_tag[j].album, s_tag[j-1].album);
				strSwap(s_tag[j].year, s_tag[j-1].year);
				strSwap(s_tag[j].comment, s_tag[j-1].comment);
				strSwap(s_tag[j].length, s_tag[j-1].length);
				temp = s_tag[j].genre;
				s_tag[j].genre = s_tag[j-1].genre;
				s_tag[j-1].genre = temp;

				temp = s_tag[j].trackno;
				s_tag[j].trackno = s_tag[j-1].trackno;
				s_tag[j-1].trackno = temp;
			}
		}
	}

	for (int i = 0; i <= cur_index; i++) {
		for (int j = cur_index; j > i; j--) {
			if (strLess(s_tag[j].album, s_tag[j-1].album) > 0) {
				strSwap(s_tag[j].artist, s_tag[j-1].artist);
				strSwap(s_tag[j].filename, s_tag[j-1].filename);
				strSwap(s_tag[j].title, s_tag[j-1].title);
				strSwap(s_tag[j].album, s_tag[j-1].album);
				strSwap(s_tag[j].year, s_tag[j-1].year);
				strSwap(s_tag[j].comment, s_tag[j-1].comment);
				strSwap(s_tag[j].length, s_tag[j-1].length);
				temp = s_tag[j].genre;
				s_tag[j].genre = s_tag[j-1].genre;
				s_tag[j-1].genre = temp;

				temp = s_tag[j].trackno;
				s_tag[j].trackno = s_tag[j-1].trackno;
				s_tag[j-1].trackno = temp;
			}
		}
	}

	for (int i = 0; i <= cur_index; i++) {
		for (int j = cur_index; j > i; j--) {
			if (strLess(s_tag[j].year, s_tag[j-1].year) > 0) {
				strSwap(s_tag[j].artist, s_tag[j-1].artist);
				strSwap(s_tag[j].filename, s_tag[j-1].filename);
				strSwap(s_tag[j].title, s_tag[j-1].title);
				strSwap(s_tag[j].album, s_tag[j-1].album);
				strSwap(s_tag[j].year, s_tag[j-1].year);
				strSwap(s_tag[j].comment, s_tag[j-1].comment);
				strSwap(s_tag[j].length, s_tag[j-1].length);
				temp = s_tag[j].genre;
				s_tag[j].genre = s_tag[j-1].genre;
				s_tag[j-1].genre = temp;

				temp = s_tag[j].trackno;
				s_tag[j].trackno = s_tag[j-1].trackno;
				s_tag[j-1].trackno = temp;
			}
		}
	}

	for (int i = 0; i <= cur_index; i++) {
		for (int j = cur_index; j > i; j--) {
			if (strLess(s_tag[j].artist, s_tag[j-1].artist) > 0) {
				strSwap(s_tag[j].artist, s_tag[j-1].artist);
				strSwap(s_tag[j].filename, s_tag[j-1].filename);
				strSwap(s_tag[j].title, s_tag[j-1].title);
				strSwap(s_tag[j].album, s_tag[j-1].album);
				strSwap(s_tag[j].year, s_tag[j-1].year);
				strSwap(s_tag[j].comment, s_tag[j-1].comment);
				strSwap(s_tag[j].length, s_tag[j-1].length);
				temp = s_tag[j].genre;
				s_tag[j].genre = s_tag[j-1].genre;
				s_tag[j-1].genre = temp;

				temp = s_tag[j].trackno;
				s_tag[j].trackno = s_tag[j-1].trackno;
				s_tag[j-1].trackno = temp;
			}
		}
	}
}
