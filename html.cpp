#include <sstream>

#include "html.h"
#include "css.h"

void writeHtmlFile(Tag2Html::MP3Collection* mp3Collection)
{
	XMLPlatformUtils::Initialize();
	DOMImplementation* domImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));
	// <html>
	DOMDocument* doc = domImplementation->createDocument(0, XMLString::transcode("html"), 0);

	DOMDocumentType* docType = domImplementation->createDocumentType(XMLString::transcode("html"), XMLString::transcode(""), XMLString::transcode(""));
	doc->insertBefore(docType, doc->getDocumentElement());

	//	<head>
	DOMElement* head = doc->createElement(XMLString::transcode("head"));

	//		<link rel="stylesheet" type="text/css" href="./index.css">
	DOMElement* linkStylesheet = doc->createElement(XMLString::transcode("link"));
	linkStylesheet->setAttribute(XMLString::transcode("rel"), XMLString::transcode("stylesheet"));
	linkStylesheet->setAttribute(XMLString::transcode("type"), XMLString::transcode("text/css"));
	linkStylesheet->setAttribute(XMLString::transcode("href"), XMLString::transcode("index.css"));
	head->appendChild(linkStylesheet);

	//		<title>MP3-Leser</title>
	DOMElement* title = doc->createElement(XMLString::transcode("title"));
	title->setTextContent(XMLString::transcode("MP3-Leser"));
	head->appendChild(title);

	//		<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	DOMElement* meta = doc->createElement(XMLString::transcode("meta"));
	meta->setAttribute(XMLString::transcode("http-equiv"), XMLString::transcode("Content-Type"));
	meta->setAttribute(XMLString::transcode("content"), XMLString::transcode("text/html"));
	meta->setAttribute(XMLString::transcode("charset"), XMLString::transcode("utf-8"));
	head->appendChild(meta);

	//	<body>
	DOMElement* body = doc->createElement(XMLString::transcode("body"));

	//		<h1>
	DOMElement* h1 = doc->createElement(XMLString::transcode("h1"));

	//			Track List ( view <a href="stats.html">stats</a>, <a href="info.html">info</a> )
	h1->appendChild(doc->createTextNode(XMLString::transcode("Track List ( view ")));
	DOMElement* statsLink = doc->createElement(XMLString::transcode("a"));
	statsLink->setAttribute(XMLString::transcode("href"), XMLString::transcode("stats.html"));
	statsLink->setTextContent(XMLString::transcode("stats"));
	h1->appendChild(statsLink);
	h1->appendChild(doc->createTextNode(XMLString::transcode(", ")));
	DOMElement* infoLink = doc->createElement(XMLString::transcode("a"));
	infoLink->setAttribute(XMLString::transcode("href"), XMLString::transcode("info.html"));
	infoLink->setTextContent(XMLString::transcode("info"));
	h1->appendChild(infoLink);
	h1->appendChild(doc->createTextNode(XMLString::transcode(" )")));
	body->appendChild(h1);

	//		<table align="center" width="1000">
	DOMElement* table = doc->createElement(XMLString::transcode("table"));
	table->setAttribute(XMLString::transcode("align"), XMLString::transcode("center"));

	//			<thead>
	DOMElement* tHead = doc->createElement(XMLString::transcode("thead"));

	//				<tr bgcolor="#CCCCCC">
	DOMElement* tHeadRow = doc->createElement(XMLString::transcode("tr"));
	tHeadRow->setAttribute(XMLString::transcode("bgcolor"), XMLString::transcode("#CCCCCC"));

	//					<th>Track</th>
	DOMElement* tHeadColumnTrack = doc->createElement(XMLString::transcode("th"));
	tHeadColumnTrack->setTextContent(XMLString::transcode("Track"));
	tHeadRow->appendChild(tHeadColumnTrack);

	//					<th>Artist</th>
	DOMElement* tHeadColumnArtist = doc->createElement(XMLString::transcode("th"));
	tHeadColumnArtist->setTextContent(XMLString::transcode("Artist"));
	tHeadRow->appendChild(tHeadColumnArtist);

	//					<th>Title</th>
	DOMElement* tHeadColumnTitle = doc->createElement(XMLString::transcode("th"));
	tHeadColumnTitle->setTextContent(XMLString::transcode("Title"));
	tHeadRow->appendChild(tHeadColumnTitle);

	//					<th>Album</th>
	DOMElement* tHeadColumnAlbum = doc->createElement(XMLString::transcode("th"));
	tHeadColumnAlbum->setTextContent(XMLString::transcode("Album"));
	tHeadRow->appendChild(tHeadColumnAlbum);

	//					<th>Year</th>
	DOMElement* tHeadColumnYear = doc->createElement(XMLString::transcode("th"));
	tHeadColumnYear->setTextContent(XMLString::transcode("Year"));
	tHeadRow->appendChild(tHeadColumnYear);

	//					<th>Genre</th>
	DOMElement* tHeadColumnGenre = doc->createElement(XMLString::transcode("th"));
	tHeadColumnGenre->setTextContent(XMLString::transcode("Genre"));
	tHeadRow->appendChild(tHeadColumnGenre);

	//					<th>Comment</th>
	DOMElement* tHeadColumnComment = doc->createElement(XMLString::transcode("th"));
	tHeadColumnComment->setTextContent(XMLString::transcode("Comment"));
	tHeadRow->appendChild(tHeadColumnComment);

	//					<th>Length</th>
	DOMElement* tHeadColumnLength = doc->createElement(XMLString::transcode("th"));
	tHeadColumnLength->setTextContent(XMLString::transcode("Length"));
	tHeadRow->appendChild(tHeadColumnLength);

	//					<th>Filename</th>
	DOMElement* tHeadColumnFilename = doc->createElement(XMLString::transcode("th"));
	tHeadColumnFilename->setTextContent(XMLString::transcode("Filename"));
	tHeadRow->appendChild(tHeadColumnFilename);

	tHead->appendChild(tHeadRow);
	table->appendChild(tHead);

	//			<tbody>
	DOMElement* tBody = doc->createElement(XMLString::transcode("tbody"));

	ostringstream convert;
	list<Tag2Html::MP3Infos*> sortedList = mp3Collection->getSortedList();
	for (list<Tag2Html::MP3Infos*>::iterator mp3info = sortedList.begin(); mp3info != sortedList.end(); mp3info++) {
		DOMElement* row = doc->createElement(XMLString::transcode("tr"));

		convert.str("");
		convert << (*mp3info)->track;
		DOMElement* trackColumn = doc->createElement(XMLString::transcode("td"));
		trackColumn->setTextContent(XMLString::transcode(convert.str().c_str()));
		row->appendChild(trackColumn);

		DOMElement* artistColumn = doc->createElement(XMLString::transcode("td"));
		artistColumn->setTextContent(XMLString::transcode((*mp3info)->artist.c_str()));
		row->appendChild(artistColumn);

		DOMElement* titleColumn = doc->createElement(XMLString::transcode("td"));
		titleColumn->setTextContent(XMLString::transcode((*mp3info)->title.c_str()));
		row->appendChild(titleColumn);

		DOMElement* albumColumn = doc->createElement(XMLString::transcode("td"));
		albumColumn->setTextContent(XMLString::transcode((*mp3info)->album.c_str()));
		row->appendChild(albumColumn);

		convert.str("");
		convert << (*mp3info)->year;
		DOMElement* yearColumn = doc->createElement(XMLString::transcode("td"));
		yearColumn->setTextContent(XMLString::transcode(convert.str().c_str()));
		row->appendChild(yearColumn);

		DOMElement* genreColumn = doc->createElement(XMLString::transcode("td"));
		genreColumn->setTextContent(XMLString::transcode((*mp3info)->genre.c_str()));
		row->appendChild(genreColumn);

		DOMElement* commentColumn = doc->createElement(XMLString::transcode("td"));
		commentColumn->setTextContent(XMLString::transcode((*mp3info)->comment.c_str()));
		row->appendChild(commentColumn);

		DOMElement* lengthColumn = doc->createElement(XMLString::transcode("td"));
		if ((*mp3info)->length < 3600) {
			char trackLength[12];
			struct tm* timeinfo = new tm();

			int seconds = (*mp3info)->length;
			if (seconds >= 60) {
				int minutes = seconds / 60;
				if (minutes >= 60) {
					timeinfo->tm_hour = minutes / 60;
					timeinfo->tm_min = minutes / 60;
				} else {
					timeinfo->tm_min = seconds / 60;
				}
			}
			timeinfo->tm_sec = seconds % 60;
			strftime(trackLength, 12, "%H:%M:%S", timeinfo);

			lengthColumn->setTextContent(XMLString::transcode(trackLength));
		}
		row->appendChild(lengthColumn);

		DOMElement* filenameColumn = doc->createElement(XMLString::transcode("td"));
		filenameColumn->setTextContent(XMLString::transcode((*mp3info)->filename.c_str()));
		row->appendChild(filenameColumn);

		tBody->appendChild(row);
	}

	table->appendChild(tBody);
	body->appendChild(table);
	doc->getDocumentElement()->appendChild(head);
	doc->getDocumentElement()->appendChild(body);

	DOMWriter* writer = ((DOMImplementationLS*)domImplementation)->createDOMWriter();
	if (writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
		writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}
	if (writer->canSetFeature(XMLUni::fgDOMXMLDeclaration, false)) {
		writer->setFeature(XMLUni::fgDOMXMLDeclaration, false);
	}
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("index.html");
	writer->writeNode(fileFormatTarget, *doc);
	fileFormatTarget->flush();
	writer->release();

	doc->release();
	XMLPlatformUtils::Terminate();

	writeCssFile();
}
