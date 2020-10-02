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

#ifndef TAG_H
#define TAG_H

class header
{
	public:
		unsigned short bitrate_index;
		unsigned short channel_mode;
		unsigned short copyright;
		unsigned short emphasis;
		unsigned short frame_sync_bits;
		unsigned short layer_description;
		unsigned short mode_extension;
		unsigned short mpeg_audio_version;
		unsigned short original;
		unsigned short padding_bit;
		unsigned short private_bit;
		unsigned short protection_bit;
		unsigned short sampling_rate_frequency;
		unsigned short frame_size;
		unsigned short Dateiposition;
		bool Frame_Sync_Bit;
		char MPEG_Audio_Version[255];
		char Layer[255];
		short Bitrate;
		unsigned short Samplerate;
		char Channel_Mode[255];
		char Intensity_Stereo[255];
		char MS_Stereo[255];
		char Error_Protection[255];
		char Copyright[255];
		char Original[255];
		char Private[255];
		char Emphasis[255];
		int Dateigroesze;
		unsigned int Length; // in Sekunden
		char Length_String[20], buffer_stunden[9], buffer_minuten[9], buffer_sekunden[2];
};

int get_filesize(char *buffer);
int get_mp3tags(char *filename_ptr, tags *mp3tags);
int get_mp3header(char *filename_ptr, header *mp3header);
int get_mp3time(unsigned int time, header *mp3header);

#endif /* TAG_H */
