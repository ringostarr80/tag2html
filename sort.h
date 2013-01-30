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

#ifndef SORT_H
#define SORT_H


// must be string.h for use of strncpy
#include <string.h>

// first must be classes.h, there ist declaration of "tags"
#include "classes.h"
#include "tag.h"


class mp3_sort
{
	public:
		mp3_sort();
		~mp3_sort();
		tags *s_tag;

		int cur_index;

		void make_array(tags *mytag);
		void sort_tag(void);
		int strSwap(char *one, char *two);
		int strLess(char *one, char *two);
		//  return < 0 --> two nach one im alphabet
		//  return = 0 --> one + two sind gleich
		//  return > 0 --> two vor one im alphabet
};



#endif /* SORT_H */
