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
	writeStatFile(mp3Collection);
}

void writeStatFile(Tag2Html::MP3Collection* mp3Collection)
{
	ostringstream convert;

	XMLPlatformUtils::Initialize();
	DOMImplementation* domImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));
	// <html>
	DOMDocument* doc = domImplementation->createDocument(0, XMLString::transcode("html"), 0);

	DOMDocumentType* docType = domImplementation->createDocumentType(XMLString::transcode("html"), XMLString::transcode(""), XMLString::transcode(""));
	doc->insertBefore(docType, doc->getDocumentElement());

	//	<head>
	DOMElement* head = doc->createElement(XMLString::transcode("head"));

	//		<link rel="stylesheet" type="text/css" href="index.css">
	DOMElement* headLink = doc->createElement(XMLString::transcode("link"));
	headLink->setAttribute(XMLString::transcode("rel"), XMLString::transcode("stylesheet"));
	headLink->setAttribute(XMLString::transcode("type"), XMLString::transcode("text/css"));
	headLink->setAttribute(XMLString::transcode("href"), XMLString::transcode("index.css"));
	head->appendChild(headLink);

	//		<title>tag2html stat page</title>
	DOMElement* headTitle = doc->createElement(XMLString::transcode("title"));
	headTitle->setTextContent(XMLString::transcode("tag2html stat page"));
	head->appendChild(headTitle);

	//		<meta http-equiv="Content-Type" content="text/html" charset="utf-8">
	DOMElement* headMeta = doc->createElement(XMLString::transcode("meta"));
	headMeta->setAttribute(XMLString::transcode("http-equiv"), XMLString::transcode("Content-Type"));
	headMeta->setAttribute(XMLString::transcode("content"), XMLString::transcode("text/html"));
	headMeta->setAttribute(XMLString::transcode("charset"), XMLString::transcode("utf-8"));
	head->appendChild(headMeta);

	//	<body>
	DOMElement* body = doc->createElement(XMLString::transcode("body"));

	//		<br><br><br>
	body->appendChild(doc->createElement(XMLString::transcode("br")));
	body->appendChild(doc->createElement(XMLString::transcode("br")));
	body->appendChild(doc->createElement(XMLString::transcode("br")));

	//		<table align="center" width="300">
	DOMElement* table = doc->createElement(XMLString::transcode("table"));
	table->setAttribute(XMLString::transcode("align"), XMLString::transcode("center"));
	table->setAttribute(XMLString::transcode("width"), XMLString::transcode("300"));

	//			<tbody>
	DOMElement* tbody = doc->createElement(XMLString::transcode("tbody"));

	//				<tr valign="top" bgcolor="#CCCCCC">
	DOMElement* tableRow = doc->createElement(XMLString::transcode("tr"));
	tableRow->setAttribute(XMLString::transcode("valign"), XMLString::transcode("top"));
	tableRow->setAttribute(XMLString::transcode("bgcolor"), XMLString::transcode("#CCCCCC"));

	//					<td align="center">
	DOMElement* tableColumn = doc->createElement(XMLString::transcode("td"));
	tableColumn->setAttribute(XMLString::transcode("align"), XMLString::transcode("center"));
	tableRow->appendChild(tableColumn);

	//						<b>Statistics</b>
	DOMElement* columnStatistics = doc->createElement(XMLString::transcode("b"));
	columnStatistics->setTextContent(XMLString::transcode("Statistics"));
	tableColumn->appendChild(columnStatistics);

	//				<tr>
	DOMElement* tableRow2 = doc->createElement(XMLString::transcode("tr"));

	//					<td>
	DOMElement* tableColumn2 = doc->createElement(XMLString::transcode("td"));

	//						<table class="nb">
	DOMElement* innerTable = doc->createElement(XMLString::transcode("table"));
	innerTable->setAttribute(XMLString::transcode("class"), XMLString::transcode("nb"));

	//							<tr>
	DOMElement* rowFileType = doc->createElement(XMLString::transcode("tr"));

	//								<td style="font-weight: bold;">File Type:</td>
	DOMElement* columnFileType = doc->createElement(XMLString::transcode("td"));
	columnFileType->setAttribute(XMLString::transcode("style"), XMLString::transcode("font-weight: bold;"));
	columnFileType->setTextContent(XMLString::transcode("File Type:"));
	rowFileType->appendChild(columnFileType);

	//								<td>MP3</td>
	DOMElement* columnFileTypeValue = doc->createElement(XMLString::transcode("td"));
	columnFileTypeValue->setTextContent(XMLString::transcode("MP3"));
	rowFileType->appendChild(columnFileTypeValue);

	//							<tr>
	DOMElement* rowFileCount = doc->createElement(XMLString::transcode("tr"));

	//								<td style="font-weight: bold;">File Count:</td>
	DOMElement* columnFileCount = doc->createElement(XMLString::transcode("td"));
	columnFileCount->setAttribute(XMLString::transcode("style"), XMLString::transcode("font-weight: bold;"));
	columnFileCount->setTextContent(XMLString::transcode("File Count:"));
	rowFileCount->appendChild(columnFileCount);

	//								<td>" << mystat->mp3_count << "</td>
	DOMElement* columnFileCountValue = doc->createElement(XMLString::transcode("td"));
	convert.str("");
	convert << mp3Collection->items.size();
	columnFileCountValue->setTextContent(XMLString::transcode(convert.str().c_str()));
	rowFileCount->appendChild(columnFileCountValue);

	//							<tr>\n";
	DOMElement* rowArtistCount = doc->createElement(XMLString::transcode("tr"));

	//								<td style="font-weight: bold;">Artist Count:</td>
	DOMElement* columnArtistCount = doc->createElement(XMLString::transcode("td"));
	columnArtistCount->setAttribute(XMLString::transcode("style"), XMLString::transcode("font-weight: bold;"));
	columnArtistCount->setTextContent(XMLString::transcode("Artist Count:"));
	rowArtistCount->appendChild(columnArtistCount);

	//								<td>" << mystat->art_count << "</td>
	DOMElement* columnArtistCountValue = doc->createElement(XMLString::transcode("td"));
	convert.str("");
	convert << mp3Collection->getArtistCount();
	columnArtistCountValue->setTextContent(XMLString::transcode(convert.str().c_str()));
	rowArtistCount->appendChild(columnArtistCountValue);

	//							<tr>
	DOMElement* rowAlbumCount = doc->createElement(XMLString::transcode("tr"));

	//								<td style="font-weight: bold;">Album Count:</td>
	DOMElement* columnAlbumCount = doc->createElement(XMLString::transcode("td"));
	columnAlbumCount->setAttribute(XMLString::transcode("style"), XMLString::transcode("font-weight: bold;"));
	columnAlbumCount->setTextContent(XMLString::transcode("Album Count:"));
	rowAlbumCount->appendChild(columnAlbumCount);

	//								<td>" << mystat->alb_count << "</td>
	DOMElement* columnAlbumCountValue = doc->createElement(XMLString::transcode("td"));
	convert.str("");
	convert << mp3Collection->getAlbumCount();
	columnAlbumCountValue->setTextContent(XMLString::transcode(convert.str().c_str()));
	rowAlbumCount->appendChild(columnAlbumCountValue);

	//							<tr>
	DOMElement* rowTotalSize = doc->createElement(XMLString::transcode("tr"));

	//								<td style="font-weight: bold;">Total Size:</td>
	DOMElement* columnTotalSize = doc->createElement(XMLString::transcode("td"));
	columnTotalSize->setAttribute(XMLString::transcode("style"), XMLString::transcode("font-weight: bold;"));
	columnTotalSize->setTextContent(XMLString::transcode("Total Filesize:"));
	rowTotalSize->appendChild(columnTotalSize);

	//								<td>" << ( mystat->tot_filesize/(1024*1024)) << " MByte</td>
	DOMElement* columnTotalSizeValue = doc->createElement(XMLString::transcode("td"));
	convert.str("");
	convert << (mp3Collection->getTotalFilesize() / 1024 / 1024) << " MB";
	columnTotalSizeValue->setTextContent(XMLString::transcode(convert.str().c_str()));
	rowTotalSize->appendChild(columnTotalSizeValue);

	//							<tr>
	DOMElement* rowTotalLength = doc->createElement(XMLString::transcode("tr"));

	//								<td style="font-weight: bold;">Total Length:</td>
	DOMElement* columnTotalLength = doc->createElement(XMLString::transcode("td"));
	columnTotalLength->setAttribute(XMLString::transcode("style"), XMLString::transcode("font-weight: bold;"));
	columnTotalLength->setTextContent(XMLString::transcode("Total Length:"));
	rowTotalLength->appendChild(columnTotalLength);

	//								<td> ~ " << mystat->tot_length << "</td>
	DOMElement* columnTotalLengthValue = doc->createElement(XMLString::transcode("td"));
	convert.str("");

	char statLength[12];
	struct tm* timeinfo = new tm();
	int seconds = mp3Collection->getTotalLength();
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
	strftime(statLength, 12, "%H:%M:%S", timeinfo);

	columnTotalLengthValue->setTextContent(XMLString::transcode(statLength));
	rowTotalLength->appendChild(columnTotalLengthValue);

	innerTable->appendChild(rowFileType);
	innerTable->appendChild(rowFileCount);
	innerTable->appendChild(rowArtistCount);
	innerTable->appendChild(rowAlbumCount);
	innerTable->appendChild(rowTotalSize);
	innerTable->appendChild(rowTotalLength);
	tableColumn2->appendChild(innerTable);
	tableRow2->appendChild(tableColumn2);

	tbody->appendChild(tableRow);
	tbody->appendChild(tableRow2);
	table->appendChild(tbody);
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
	XMLFormatTarget *fileFormatTarget = new LocalFileFormatTarget("stats.html");
	writer->writeNode(fileFormatTarget, *doc);
	fileFormatTarget->flush();
	writer->release();

	doc->release();
	XMLPlatformUtils::Terminate();
}
