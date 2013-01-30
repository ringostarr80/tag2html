/***************************************************************************
                          gettag.h  -  description
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

#ifndef GETTAG_H
#define GETTAG_H

#include <iostream>
#include <fstream>

#include "classes.h"
#include "tag.h"


void tag_kuerzen(char* tag, int zeichenanzahl);
int get_mp3tags(char *filename_ptr, tags *mp3tags);

#endif /* GETTAG_H */
