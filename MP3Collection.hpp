/*
 * File:   MP3Collection.hpp
 * Author: Ringo Leese
 *
 * Created on 20. Februar 2013, 21:59
 */

#ifndef MP3COLLECTION_HPP
#define	MP3COLLECTION_HPP

#include <vector>
#include <taglib/fileref.h>

#include "cpplinq.hpp"
#include "MP3Infos.hpp"

using namespace std;

namespace Tag2Html {
	class MP3Collection {
	public:
		vector<MP3Infos*> items;

		MP3Collection();
		MP3Collection(const MP3Collection& orig);
		virtual ~MP3Collection();

		void add(TagLib::FileRef* fileRef);
		list<MP3Infos*> getSortedList();
		int getArtistCount();
		int getAlbumCount();
		long getTotalFilesize();
		int getTotalLength();
	private:
	};
}

#endif	/* MP3COLLECTION_HPP */
