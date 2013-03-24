/*
 * File:   MP3Collection.cpp
 * Author: Ringo Leese
 *
 * Created on 20. Februar 2013, 21:59
 */

#include <vector>
#include <list>
#include <taglib/mpegfile.h>

#include "cpplinq.hpp"
#include "MP3Collection.hpp"

using namespace cpplinq;

namespace Tag2Html {
	MP3Collection::MP3Collection() {

	}

	MP3Collection::MP3Collection(const MP3Collection& orig) {

	}

	MP3Collection::~MP3Collection() {

	}

	void MP3Collection::add(TagLib::FileRef* fileRef) {
		MP3Infos* mp3Info = new MP3Infos();
		mp3Info->track = fileRef->tag()->track();
		mp3Info->artist = fileRef->tag()->artist().to8Bit(true);
		mp3Info->title = fileRef->tag()->title().to8Bit(true);
		mp3Info->length = fileRef->audioProperties()->length();
		mp3Info->album = fileRef->tag()->album().to8Bit(true);
		mp3Info->year = fileRef->tag()->year();
		mp3Info->genre = fileRef->tag()->genre().to8Bit(true);
		mp3Info->comment = fileRef->tag()->comment().to8Bit(true);
		mp3Info->filename = fileRef->file()->name();
		mp3Info->filesize = fileRef->file()->length();
		mp3Info->bitrate = fileRef->audioProperties()->bitrate();
		mp3Info->samplerate = fileRef->audioProperties()->sampleRate();

		TagLib::MPEG::File* mpegFileRef = new TagLib::MPEG::File(fileRef->file()->name());
		mp3Info->isCopyrighted = mpegFileRef->audioProperties()->isCopyrighted();
		mp3Info->isOriginal = mpegFileRef->audioProperties()->isOriginal();
		mp3Info->protectionEnabled = mpegFileRef->audioProperties()->protectionEnabled();
		mp3Info->channels = mpegFileRef->audioProperties()->channels();
		mp3Info->channelMode = mpegFileRef->audioProperties()->channelMode();
		mp3Info->layer = mpegFileRef->audioProperties()->layer();
		mp3Info->version = mpegFileRef->audioProperties()->version();

		this->items.push_back(mp3Info);
	}

	list<MP3Infos*> MP3Collection::getSortedList() {
		auto result = from_iterators(this->items.begin(), this->items.end()) >> orderby_ascending([](MP3Infos* const &infos) {
			return infos->track;
		}) >> to_list();
		return result;
	}

	int MP3Collection::getArtistCount() {
		auto result = from_iterators(this->items.begin(), this->items.end()) >> select([](MP3Infos* const &infos) {
			return infos->artist;
		}) >> distinct() >> count();

		return result;
	}

	int MP3Collection::getAlbumCount() {
		auto result = from_iterators(this->items.begin(), this->items.end()) >> select([](MP3Infos* const &infos) {
			return infos->album;
		}) >> distinct() >> count();

		return result;
	}

	long MP3Collection::getTotalFilesize() {
		auto result = from_iterators(this->items.begin(), this->items.end()) >> select([](MP3Infos* const &infos) {
			return infos->filesize;
		}) >> sum();

		return result;
	}

	int MP3Collection::getTotalLength() {
		auto result = from_iterators(this->items.begin(), this->items.end()) >> select([](MP3Infos* const &infos) {
			return infos->length;
		}) >> sum();

		return result;
	}
}
