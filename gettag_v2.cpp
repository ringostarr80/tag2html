/***************************************************************************
                          gettag_v2.cpp  -  description
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

/***************************************************************************

source: id3.org/id3v2.3.0

ID3v2/file identifier   "ID3"
ID3v2 version           $03 00
ID3v2 flags             %abc00000
ID3v2 size              4 * %0xxxxxxx

a - Unsynchronisation, b - Extended header, c - Experimental indicator

***************************************************************************/


#define DEBUG_IT 1

#include <stdio.h>
#include "gettag_v2.h"

/**
   @params charpointer - a pointer to char-array with containing information,
   framesize as int the size of the frame, ptr_to_cstr is a pointer to a char-array
   where the information is supposed to end, max_charsize is an int with the maximum
   that can be used with ptr_to_cstr
   @return errorcode: 0 - no error
   @brief reads a frame with distinguishing ISO8859-1 and Unicode
 */
int read_frame(char* charpointer, int framesize, char *ptr_to_cstr, int max_charsize){
  // 0 = ISO8859-1
  if(charpointer[0] == 0){
      if(DEBUG_IT)std::cout << "ISO8859-1" << std::endl;
    charpointer++; framesize--;
    std::string artist(charpointer, framesize);
    std::strncpy(ptr_to_cstr, artist.c_str(), max_charsize);
      }else if(charpointer[0] == 1){
    // FF FE oder FE FF, byteorder-issus
    // we just interpret our normal ascii-code characters

    if(charpointer[1] == -1 && charpointer[2] == -2){
      if(DEBUG_IT)std::cout << "Unicode FF FE" << std::endl;
      framesize -= 3; charpointer +=3;
      for(int i = 0, j = 0; i <= framesize; i += 2, j++)
	ptr_to_cstr[j] = charpointer[i];
    }else if(charpointer[1] == -2 && charpointer[2] == -1){
      if(DEBUG_IT)std::cout << "Unicode FE FF" << std::endl;
      framesize -= 3; charpointer +=3;
      for(int i = 1, j = 0; i <= framesize; i += 2, j++)
	ptr_to_cstr[j] = charpointer[i];
    }else{
      std::cout << "(read_frame) Unicode, but nor FFFE nor FEFF" << std::endl;
    }

  }else{
    std::cout << "(read_frame) unhandled frame" << std::endl;
  }

  return 0;
}


void tag_kuerzen_v2(char* tag, int zeichenanzahl)
{
  bool end_found=false;
  for(int i=zeichenanzahl; i>0; i--)
  {
    if(tag[i]!=' ')
    {
      tag[i+1]='\0';
      i=0;
      end_found=true;
    }
  }
  if(end_found==false)
    tag[0]='\0';
}

/**
   @params filename_ptr - the name of mp3-file, mp3tags - tags to be written
   @return errorcode: 0 - no error
   @brief parses an mp3-file for id3v2-tags and writes some in a tags-object
 */
int get_mp3tags_v2(char *filename_ptr, tags *mp3tags)
{
  std::ifstream mp3_datei;
  int tagsize = 0, version = 0;
  char *mp3header_buffer;
  bool synch_bool = 0, ext_header_bool = 0, experimental_bool = 0;

  // open mp3
  mp3_datei.open(filename_ptr, std::ios::in);
  if(!mp3_datei)
  {
    std::cerr << "error: could not open '" << filename_ptr
	      << "' to read tags!" << std::endl;
    return -1;
  }
  // wanna start at the beginning, with no offset
  mp3_datei.seekg(0, std::ios::beg);

  /////////////////////////
  //// mp3header START ////
  /////////////////////////

  // check for id3v2-tag in beginning of the file (no support for
  // tag at another position yet)
  mp3header_buffer = new char[10];
  for(int i=0; i<=9; i++)
  {
    mp3_datei.get(mp3header_buffer[i]);
  }

  // read 'ID3' at begin, otherwise leave
  if(DEBUG_IT)std::cout << mp3header_buffer[0] << mp3header_buffer[2]
			<< mp3header_buffer[2] << std::endl;
  if(mp3header_buffer[0] != 'I' || mp3header_buffer[1] != 'D' ||
     mp3header_buffer[2] != '3'){
    mp3_datei.close();
   std::cout << std::endl << "no mp3v2-TAGS found in "
	      << filename_ptr << "." << std::endl;
   return -2;
  }

  // Version checking, just 1-4 supported, just major version
  // is checked, minor version is in mp3header_buffer[4]
  // version is also later used
  version = mp3header_buffer[3];
  if(DEBUG_IT)std::cout << "ver: "<< version << std::endl;

  if(mp3header_buffer[3] < 0 || mp3header_buffer[3] > 4){
    mp3_datei.close();
   std::cout << std::endl << "id3v2-version not supported (file "
	      << filename_ptr << ")." << std::endl;
   return -3;
  }

  // retrieve tag size
  // ID3v2 size              4 * %0xxxxxxx
  // most significant bit of each byte is ignored - is zero
  // 7-shift equals *128
  // tagsize includes header, so for add a 10
  tagsize = (mp3header_buffer[6] << 21) + (mp3header_buffer[7] << 14)
    + (mp3header_buffer[8] << 7) + (mp3header_buffer[9]) + 10;

  if(DEBUG_IT)std::cout << "tagsize: " << tagsize << std::endl;

  // get tags
  if(mp3header_buffer[5] & 0x80) synch_bool = 1;
  if(mp3header_buffer[5] & 0x40) ext_header_bool = 1;
  if(mp3header_buffer[5] & 0x20) experimental_bool = 1;
  if(DEBUG_IT)std::cout << "Unsynchronisation: " << synch_bool << std::endl;
  if(DEBUG_IT)std::cout << "Extended header: " << ext_header_bool << std::endl;
  if(DEBUG_IT)std::cout << "Experimental: " << experimental_bool << std::endl;

  delete(mp3header_buffer);

  /////////////////////////
  //// mp3header END //////
  /////////////////////////

  // if there is an extended header, we wanna skip it
  if(ext_header_bool){
    // first read header-length
    char *header_len = new char[4];
  for(int i = 0; i <= 3; i++)mp3_datei.get(header_len[i]);

  // headerlength -4 - the four bytes of headersize
  int header_length = (header_len[0] << 21) + (header_len[1] << 14)
    + (header_len[2] << 7) + header_len[3] - 4;
  // skipping
  for(int i = 0; i < header_length; i++)mp3_datei.get(header_len[i]);
  }


  // Now we read in the whole tag
  char * buff = new char[tagsize];
  char* charpointer = 0;
  int length = tagsize;
  int pos = 0;
  int bytes_left = 0;
  int framesize = 0;
  int size_frameheader = 0;
  std::string framename1;

  // if version is prior to 3, size_frameheader is 6, 10 otherwise
  if(version <= 2)size_frameheader = 6;
  else size_frameheader = 10;


  for(int i = 0; i < length; i++)mp3_datei.get(buff[i]);

  // now we are done with the file, we close it
  mp3_datei.close();

  /////////////////////////
  //// main-loop START ////
  /////////////////////////
  for(int i = 0;i <= 65000;i++){

    bytes_left = length - pos;
    if(bytes_left < 1){
      if(DEBUG_IT)std::cout << "No bytes left" << std::endl;
      break;
    }

    // are we having a frame?
    if(buff[pos] < 'A' || buff[pos] > 'Z'){
      if(DEBUG_IT)std::cout << "We have no frame" << std::endl;
      break;
    }


    /* Layout of frameheader: (id3v2 ver. 3 and above)
       Frame ID       $xx xx xx xx (four characters)
       Size           $xx xx xx xx
       Flags          $xx xx
    */
    // retrieving framename and framesize
    // framename: 3 bytes vor version 1,2; 4 bytes for version 3,4
    if(version <= 2){
      framesize = (buff[pos+3] << 16) + (buff[pos+4] << 8)
	+ (buff[pos+5]);

      charpointer = &buff[pos];
      std::string framename( reinterpret_cast<const char*>(charpointer), 3);
      framename1 = framename;
      if(DEBUG_IT)std::cout << framename << std::endl;
    }else{
      framesize = (buff[pos+4] << 24) + (buff[pos+5] << 16)
	+ (buff[pos+6] << 8) + buff[pos+7];

      charpointer = &buff[pos];
      std::string framename( reinterpret_cast<const char*>(charpointer), 4);
      framename1 = framename;
      if(DEBUG_IT)std::cout << framename << std::endl;
    }

    if(framesize < 1){
      std::cout << "framesize less 1" << std::endl; return -1;
    }

    if(DEBUG_IT)std::cout << "framesize: " << framesize << std::endl;

    // now comes the part, where we find the stuff

    // TPE1: lead performer(s)/solois TPE2: Band/orchestra/accompaniment
    // TPE3: conductor/performer/refinement TPE4: interpreted, remixed
    // TP1 .. TP2 for versions prior to 3
    if(   framename1 == "TPE1" || framename1 == "TPE2"
       || framename1 == "TPE3" || framename1 == "TPE4"
       || framename1 == "TP1" || framename1 == "TP2"
       || framename1 == "TP3" || framename1 == "TP4" ){

      charpointer = &buff[pos+size_frameheader];

      read_frame(charpointer, framesize, mp3tags->artist, 29);
      tag_kuerzen_v2(mp3tags->artist, 29);
      if(DEBUG_IT)std::cout << "mp3tags->artist: " << mp3tags->artist << std::endl;

      // for(int i = pos+size_frameheader; i <= pos+size_frameheader+framesize;i++)
      // printf("%d: %d\n", i-(pos+size_frameheader), buff[i]);
    }

    // version <= 2: TT2 - Title/Songname/Content description
    // versopm >= 3: TIT2 - Title/Songname/Content description
    if(   framename1 == "TT2" || framename1 == "TIT2"){

      charpointer = &buff[pos+size_frameheader];

      read_frame(charpointer, framesize, mp3tags->title, 29);
      tag_kuerzen_v2(mp3tags->title, 29);
      if(DEBUG_IT)std::cout << "mp3tags->title: " << mp3tags->title << std::endl;
    }

    // version <= 2: TAL - Album/Movie/Show title
    // versopm >= 3: TALB - Album/Movie/Show title
    if(   framename1 == "TAL" || framename1 == "TALB"){

      charpointer = &buff[pos+size_frameheader];

      read_frame(charpointer, framesize, mp3tags->album, 29);
      tag_kuerzen_v2(mp3tags->album, 29);      tag_kuerzen_v2(mp3tags->title, 29);
      if(DEBUG_IT)std::cout << "mp3tags->album: " << mp3tags->album << std::endl;
    }

    // version <= 2: TOR - Original release year, TYE - Year
    // versopm >= 3: TORY - Original release year
    if(   framename1 == "TOR" || framename1 == "TORY" || framename1 == "TYE"){

      charpointer = &buff[pos+size_frameheader];

      read_frame(charpointer, framesize, mp3tags->year, 4);
      tag_kuerzen_v2(mp3tags->year, 4);
      if(DEBUG_IT)std::cout << "mp3tags->year: " << mp3tags->year << std::endl;
    }


    // version <= 2: COM - Original release year
    // versopm >= 3: COMM - Comments
    if(   framename1 == "COM" || framename1 == "COMM"){

      charpointer = &buff[pos+size_frameheader];

      read_frame(charpointer, framesize, mp3tags->comment, 27);
      tag_kuerzen_v2(mp3tags->comment, 27);
      if(DEBUG_IT)std::cout << "mp3tags->comment: " << mp3tags->comment << std::endl;
    }


    // version <= 2: TRK - Tracknumber
    // versopm >= 3: TRCK - Track number/Position in set
    if(   framename1 == "TRK" || framename1 == "TRCK"){

      charpointer = &buff[pos+size_frameheader];

      read_frame(charpointer, framesize, mp3tags->year, 1);
      //tag_kuerzen_v2(mp3tags->trackno, 1);
      if(DEBUG_IT)std::cout << "mp3tags->trackno: " << mp3tags->year << std::endl;
      if(DEBUG_IT)printf("trackno: %d\n", mp3tags->trackno);
    }



    pos += framesize + size_frameheader;
  }

  ///////////////////////
  //// main-loop END ////
  ///////////////////////

  return 0;
}
