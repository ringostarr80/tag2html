/***************************************************************************
                          stat.h  -  description
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

#ifndef STAT_H
#define STAT_H

#include "classes.h"
#include "html.h"

void get_albumcount(char *album, stats *mystat);
void get_artistcount( char *artist, stats *mystat );
int stat_head(html *output);
int stat_content(stats *mystat, html *output);
int stat_foot(html *output);

#endif /* STAT_H */
