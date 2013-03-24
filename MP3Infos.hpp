/*
 * File:   MP3Infos.hpp
 * Author: Ringo Leese
 *
 * Created on 20. Februar 2013, 22:33
 */

#ifndef MP3INFOS_HPP
#define	MP3INFOS_HPP

#include <string>
#include <taglib/mpegfile.h>

using namespace std;

namespace Tag2Html {
	class MP3Infos {
	public:
		MP3Infos();
		MP3Infos(const MP3Infos& orig);
		virtual ~MP3Infos();

		uint track;
		string artist;
		string title;
		int length;
		string album;
		uint year;
		string genre;
		string comment;
		string filename;
		long filesize;
		int bitrate;
		int samplerate;
		bool isCopyrighted;
		bool isOriginal;
		bool protectionEnabled;
		int channels;
		TagLib::MPEG::Header::ChannelMode channelMode;
		int layer;
		TagLib::MPEG::Header::Version version;
	private:
	};
}

#endif	/* MP3INFOS_HPP */
