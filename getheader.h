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

#ifndef GETHEADER_H
#define GETHEADER_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>

#include "classes.h"
#include "tag.h"

int get_filesize(char *buffer);
int get_mp3header(char *filename_ptr, header *mp3header);
int get_mp3time(unsigned int time, header *mp3header);

#endif /* GETHEADER_H */
