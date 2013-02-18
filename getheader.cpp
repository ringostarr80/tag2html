/***************************************************************************
                          getheader.cpp  -  description
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

#include "getheader.h"

using namespace std;

int get_filesize(char *buffer)
{
	struct stat buf;

	if (stat(buffer, &buf) == -1) {
		return -1;
	}

	return buf.st_size;
}

int get_mp3header(char *filename_ptr, header *mp3header)
{
	char Header_Bytes[4];
	std::ifstream mp3_datei;

	mp3_datei.open(filename_ptr, std::ios::in);
	if (!mp3_datei) {
		std::cerr << "error: could not open '" << filename_ptr << "' to read tags!" << std::endl;
		return -1;
	}

	// nach Frame-Anfang für id3v1 suchen!
	// Dateiposition auf 0 setzen!
	mp3header->Dateiposition = 0;
	do {
		mp3_datei.seekg(mp3header->Dateiposition, std::ios::beg);
		// Die 4 ersten Bytes auslesen
		for (int i = 0; i <= 3; i++) {
			mp3_datei.get(Header_Bytes[i]);
		}

		// frame_sync_bits überprüfen!
		unsigned short hilfsvariable;
		mp3header->frame_sync_bits = Header_Bytes[0];
		mp3header->frame_sync_bits<<= 8;
		hilfsvariable = Header_Bytes[1];
		hilfsvariable&= 0xE0; // hilfsvariable mit 11100000 bin�r UND-verkn�pft
		mp3header->frame_sync_bits|= hilfsvariable;
		if (mp3header->frame_sync_bits == 0xFFE0) {
			mp3header->Frame_Sync_Bit = true;
		} else {
			mp3header->Frame_Sync_Bit = false;
			mp3header->Dateiposition++;
		}
	} while(!mp3header->Frame_Sync_Bit);

	// Dateigröße ermitteln
	mp3header->Dateigroesze = get_filesize(filename_ptr);
	mp3_datei.close();

	// MPEG Audio Version
	mp3header->mpeg_audio_version = Header_Bytes[1];
	mp3header->mpeg_audio_version&= 0x18; // mit 00011000 bin�r UND-verkn�fen
	mp3header->mpeg_audio_version>>= 3; // alle bitverschiebung um 3 stellen nach rechts
	if (mp3header->mpeg_audio_version == 3) {
		strncpy(mp3header->MPEG_Audio_Version, "MPEG 1", sizeof(mp3header->MPEG_Audio_Version));
	} else if (mp3header->mpeg_audio_version == 2) {
		strncpy(mp3header->MPEG_Audio_Version, "MPEG 2", sizeof(mp3header->MPEG_Audio_Version));
	} else if (mp3header->mpeg_audio_version == 1) {
		strncpy(mp3header->MPEG_Audio_Version, "Reserviert", sizeof(mp3header->MPEG_Audio_Version));
	} else if (mp3header->mpeg_audio_version == 0) {
		strncpy(mp3header->MPEG_Audio_Version, "MPEG 2.5", sizeof(mp3header->MPEG_Audio_Version));
	}

	// Layer description
	mp3header->layer_description = Header_Bytes[1];
	mp3header->layer_description&= 0x06;
	mp3header->layer_description>>= 1;
	if (mp3header->layer_description == 3) {
		strncpy(mp3header->Layer, "Layer 1", sizeof(mp3header->Layer));
	} else if (mp3header->layer_description == 2) {
		strncpy(mp3header->Layer, "Layer 2", sizeof(mp3header->Layer));
	} else if (mp3header->layer_description == 1) {
		strncpy(mp3header->Layer, "Layer 3", sizeof(mp3header->Layer));
	} else if (mp3header->layer_description == 0) {
		strncpy(mp3header->Layer, "Reserviert", sizeof(mp3header->Layer));
	}

	// Protection bit
	mp3header->protection_bit = Header_Bytes[1];
	mp3header->protection_bit&= 0x01;
	if (mp3header->protection_bit == 0) {
		strncpy(mp3header->Error_Protection, "Yes", sizeof(mp3header->Error_Protection));
	} else if (mp3header->protection_bit == 1) {
		strncpy(mp3header->Error_Protection, "No", sizeof(mp3header->Error_Protection));
	}

	// BitRate Index
	mp3header->bitrate_index = Header_Bytes[2];
	mp3header->bitrate_index>>= 4;
	mp3header->bitrate_index&= 0x0F;
	if (mp3header->bitrate_index == 0) { // VBR
		mp3header->Bitrate = 0;
	} else if (mp3header->bitrate_index == 15) { // ungültig
		mp3header->Bitrate = -1;
	} else if (mp3header->mpeg_audio_version == 3) {
		if (mp3header->layer_description == 3) {
			mp3header->Bitrate = mp3header->bitrate_index * 32;
		} else if (mp3header->layer_description == 2) {
			if (mp3header->bitrate_index == 1) {
				mp3header->Bitrate = 32;
			} else if (mp3header->bitrate_index == 2) {
				mp3header->Bitrate = 32 + 16;
			} else if (mp3header->bitrate_index <= 4) {
				mp3header->Bitrate = 32 + 16 + (mp3header->bitrate_index - 2) * 8;
			} else if (mp3header->bitrate_index <= 8) {
				mp3header->Bitrate = 32 + 16 + 2 * 8 + (mp3header->bitrate_index - 4) * 16;
			} else if (mp3header->bitrate_index <= 12) {
				mp3header->Bitrate = 32 + 16 + 2 * 8 + 4 * 16 + (mp3header->bitrate_index - 8) * 32;
			} else if (mp3header->bitrate_index <= 14) {
				mp3header->Bitrate = 32 + 16 + 2 * 8 + 4 * 16 + 4 * 32 + (mp3header->bitrate_index - 12) * 64;
			}
		} else if(mp3header->layer_description == 1) {
			if (mp3header->bitrate_index == 1) {
				mp3header->Bitrate = 32;
			} else if (mp3header->bitrate_index <= 5) {
				mp3header->Bitrate = 32 + (mp3header->bitrate_index - 1) * 8;
			} else if (mp3header->bitrate_index <= 9) {
				mp3header->Bitrate = 32 + 4 * 8 + (mp3header->bitrate_index - 5) * 16;
			} else if (mp3header->bitrate_index <= 13) {
				mp3header->Bitrate = 32 + 4 * 8 + 4 * 16 + (mp3header->bitrate_index - 9) * 32;
			} else if (mp3header->bitrate_index == 14) {
				mp3header->Bitrate = 320;
			}
		}
	} else {
		if (mp3header->layer_description == 3) {
			if (mp3header->bitrate_index == 1) {
				mp3header->Bitrate = 32;
			} else if (mp3header->bitrate_index == 2) {
				mp3header->Bitrate = 32 + 16;
			} else if (mp3header->bitrate_index <= 4) {
				mp3header->Bitrate = 32 + 16 + (mp3header->bitrate_index - 2) * 8;
			} else if (mp3header->bitrate_index <= 12) {
				mp3header->Bitrate = 32 + 16 + 2 * 8 + (mp3header->bitrate_index - 4) * 16;
			} else if (mp3header->bitrate_index <= 14) {
				mp3header->Bitrate = 32 + 16 + 2 * 8 + 8 * 16 + (mp3header->bitrate_index - 12) * 32;
			}
		} else {
			if (mp3header->bitrate_index <= 8) {
				mp3header->Bitrate = mp3header->bitrate_index * 8;
			} else {
				mp3header->Bitrate = 8 * 8 + (mp3header->bitrate_index - 8) * 16;
			}
		}
	}

	// Sampling Frequency
	unsigned short teiler;
	mp3header->sampling_rate_frequency = Header_Bytes[2];
	mp3header->sampling_rate_frequency>>= 2;
	mp3header->sampling_rate_frequency&= 0x03;
	if (mp3header->mpeg_audio_version == 3) {
		teiler = 1;
	} else if (mp3header->mpeg_audio_version == 2) {
		teiler = 2;
	} else if (mp3header->mpeg_audio_version == 0) {
		teiler = 4;
	}
	if (mp3header->sampling_rate_frequency == 0) {
		mp3header->Samplerate = 44100 / teiler;
	} else if (mp3header->sampling_rate_frequency == 1) {
		mp3header->Samplerate = 48000 / teiler;
	} else if (mp3header->sampling_rate_frequency == 2) {
		mp3header->Samplerate = 32000 / teiler;
	}

	// Padding Bit
	mp3header->padding_bit = Header_Bytes[2];
	mp3header->padding_bit>>= 1;
	mp3header->padding_bit&= 0x01; // 0 = frame is not padded
								  // 1 = frame is padded with one extra bit

	// Private Bit
	mp3header->private_bit = Header_Bytes[2];
	mp3header->private_bit&= 0x01;
	if (mp3header->private_bit == 1) {
		strncpy(mp3header->Private, "Yes", sizeof(mp3header->Private));
	} else if (mp3header->private_bit == 0) {
		strncpy(mp3header->Private, "No", sizeof(mp3header->Private));
	}

	// Channel Mode
	mp3header->channel_mode = Header_Bytes[3];
	mp3header->channel_mode>>= 6;
	mp3header->channel_mode&= 0x03;
	if (mp3header->channel_mode == 0) {
		strncpy(mp3header->Channel_Mode, "Stereo", sizeof(mp3header->Channel_Mode));
	} else if (mp3header->channel_mode == 1) {
		strncpy(mp3header->Channel_Mode, "Joint Stereo", sizeof(mp3header->Channel_Mode));
	} else if (mp3header->channel_mode == 2) {
		strncpy(mp3header->Channel_Mode, "Dual Channel", sizeof(mp3header->Channel_Mode));
	} else if (mp3header->channel_mode == 3) {
		strncpy(mp3header->Channel_Mode, "Mono", sizeof(mp3header->Channel_Mode));
	}

	// Mode Extension
	if (mp3header->channel_mode == 1) {
		mp3header->mode_extension = Header_Bytes[3];
		mp3header->mode_extension>>= 4;
		mp3header->mode_extension&= 0x03;
		if (mp3header->mode_extension == 0) {
			strncpy(mp3header->Intensity_Stereo, "Off", sizeof(mp3header->Intensity_Stereo));
			strncpy(mp3header->MS_Stereo, "Off", sizeof(mp3header->MS_Stereo));
		} else if(mp3header->mode_extension == 1) {
			strncpy(mp3header->Intensity_Stereo, "On", sizeof(mp3header->Intensity_Stereo));
			strncpy(mp3header->MS_Stereo, "Off", sizeof(mp3header->MS_Stereo));
		} else if(mp3header->mode_extension == 2) {
			strncpy(mp3header->Intensity_Stereo, "Off", sizeof(mp3header->Intensity_Stereo));
			strncpy(mp3header->MS_Stereo, "On", sizeof(mp3header->MS_Stereo));
		} else if(mp3header->mode_extension == 3) {
			strncpy(mp3header->Intensity_Stereo, "On", sizeof(mp3header->Intensity_Stereo));
			strncpy(mp3header->MS_Stereo, "On", sizeof(mp3header->MS_Stereo));
		}
	}

	// Copyright
	mp3header->copyright = Header_Bytes[3];
	mp3header->copyright>>= 3;
	mp3header->copyright&= 0x01;
	if (mp3header->copyright == 0) {
		strncpy(mp3header->Copyright, "No", sizeof(mp3header->Copyright));
	} else if (mp3header->copyright == 1) {
		strncpy(mp3header->Copyright, "Yes", sizeof(mp3header->Copyright));
	}

	// Original
	mp3header->original = Header_Bytes[3];
	mp3header->original>>= 2;
	mp3header->original&= 0x01;
	if (mp3header->original == 0) {
		strncpy(mp3header->Original, "No", sizeof(mp3header->Original));
	} else if (mp3header->original == 1) {
		strncpy(mp3header->Original, "Yes", sizeof(mp3header->Original));
	}

	// Emphasis
	mp3header->emphasis = Header_Bytes[3];
	mp3header->emphasis&= 0x03;
	if (mp3header->emphasis == 0) {
		strncpy(mp3header->Emphasis, "No", sizeof(mp3header->Emphasis));
	} else if (mp3header->emphasis == 1) {
		strncpy(mp3header->Emphasis, "50/15ms", sizeof(mp3header->Emphasis));
	} else if (mp3header->emphasis == 2) {
		strncpy(mp3header->Emphasis, "reserviert", sizeof(mp3header->Emphasis));
	} else if (mp3header->emphasis == 3) {
		strncpy(mp3header->Emphasis, "CCIT J.17", sizeof(mp3header->Emphasis));
	}

	// Frame Size
	if (mp3header->layer_description == 3) {
		mp3header->frame_size = (48000 / teiler * mp3header->Bitrate) / mp3header->Samplerate + mp3header->padding_bit;
	} else {
		mp3header->frame_size = (144000 / teiler * mp3header->Bitrate) / mp3header->Samplerate + mp3header->padding_bit;
	}

	// Dauer des Tracks
	// 38.1 Frames pro Sekunde
	mp3header->Length = static_cast<unsigned long int>((mp3header->Dateigroesze - mp3header->Dateiposition - 128) / (mp3header->frame_size + 4) / 38.1);

	return 0;
}

int get_mp3time(unsigned int time, header *mp3header)
{
	unsigned int stunden=0, minuten=0, sekunden=0;

	sekunden = time % 60;
	minuten = time / 60;
	if (minuten > 60) {
		stunden = minuten / 60;
		minuten%= 60;

		// Stunden
		sprintf(mp3header->buffer_stunden, "%d", stunden);
		strcpy(mp3header->Length_String, mp3header->buffer_stunden);
		strcat(mp3header->Length_String, ":");
	}

	// Minuten
	if (minuten < 10 && stunden > 0) {
		strcat(mp3header->Length_String, "0");
	}
	sprintf(mp3header->buffer_minuten, "%d", minuten);
	if (stunden == 0) {
		strcpy(mp3header->Length_String, mp3header->buffer_minuten);
	} else {
		strcat(mp3header->Length_String, mp3header->buffer_minuten);
	}
	strcat(mp3header->Length_String, ":");

	// Sekunden
	if (sekunden < 10) {
		strcat(mp3header->Length_String, "0");
	}
	sprintf(mp3header->buffer_sekunden, "%d", sekunden);
	strcat(mp3header->Length_String, mp3header->buffer_sekunden);

	return 0;
}
