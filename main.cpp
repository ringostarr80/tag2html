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

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegheader.h>
#include <taglib/mpegproperties.h>
#include <id3/globals.h>

// definition d. klassen / funktionen einbinden...
#include "classes.h"
#include "tag.h"
#include "sort.h"
#include "xml.h"
#include "html.h"
#include "getheader.h"
#include "gettag.h"    // id3v1
#include "gettag_v2.h" // id3v2
#include "info.h"
#include "stat.h"
#include "MP3Collection.hpp"

const char *argp_program_version = "tag2html 0.2 beta";
const char *argp_program_bug_address = "<ringo19@gmx.de>";

static char doc[] = "";

/* Angabe der Argumente, die Akzeptiert werden */
static char args_doc[] = "";

/* unterst�tzte Optionen */
static struct argp_option options[] = {
	{"quiet", 'q', 0, 0, "no Output", 0},
	{"silent", 's', 0, OPTION_ALIAS, 0, 0},
	{"verbose", 'v', 0, 0, "verbose-Mode", 0},
	{"output", 'o', "FILE", 0, "Output to FILE instead of standard output", 0},
	{"html", 'h', 0, 0, "Output to HTML-File", 0},
	{"xml", 'x', 0, 0, "Output to XML-File", 0},
	{"xsd", 1, 0, 0, "Output XSD-File", 0},
	{"xsl", 2, 0, 0, "Output XSL-File", 0},
	{0, 0, 0, 0, 0, 0}
};

/* Struktur zur Parameterübergabe */
struct arguments {
	char *args[0];    /* ARG1 */
	char *output_file;
	bool silent, verbose, version, html, xml, xsd, xsl;
};

/* Parse-Funktion */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	// struct arguments *arguments = state->input;
	arguments *arguments = (struct arguments*)state->input;

	switch(key) {
		case 'q':
		case 's':
			arguments->silent = true;
			break;
		case 'v':
			arguments->verbose = true;
			break;
		case 'o':
			arguments->output_file = arg;
			break;
		case 'h':
			arguments->html = true;
			break;
		case 'x':
			arguments->xml = true;
			break;

		case 1:
			arguments->xsd = true;
			break;

		case 2:
			arguments->xsl = true;
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num >= 1) { // too many arguments
				argp_usage(state);
			}

			arguments->args[state->arg_num] = arg;
			break;

		case ARGP_KEY_END:
			if (state->arg_num < 0) { // too less arguments
				argp_usage(state);
			}
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* Our argp parser. */
static struct argp argp = {
	options,
	parse_opt,
	args_doc,
	doc,
	0,
	0,
	0
};

using namespace std;

int main(int argc, char **argv)
{
	struct arguments arguments;

	/**
	 * default values
	 */
	char outputfile[2] = "-";
	arguments.silent = false;
	arguments.verbose = false;
	arguments.version = false;
	arguments.html = false;
	arguments.xml = false;
	arguments.xsd = false;
	arguments.xsl = false;
	arguments.output_file = outputfile;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (!arguments.html && !arguments.xml && !arguments.version) {
		return(1);
	}

	// klassen erstellen...
	tags *mytag = new tags;
	html *myhtml = new html;
	header *myheader = new header;
	header *statheader = new header;
	mp3_sort *mysort = new mp3_sort;
	stats *mystat = new stats;

	// structs
	DIR *mydir;
	struct dirent *directoryEntry;

	Tag2Html::MP3Collection* mp3Collection = new Tag2Html::MP3Collection();

	char dir[1024];

	mytag->layout_flag = 1;
	mytag->trackno = 0;

	strncpy(myhtml->cssfilename, "./index.css", sizeof(myhtml->cssfilename));
	strncpy(myhtml->htmlfilename, "./index.html", sizeof(myhtml->htmlfilename));
	strncpy(myhtml->statfilename, "./stats.html", sizeof(myhtml->statfilename));
	strncpy(myhtml->infofilename, "./info.html", sizeof(myhtml->infofilename));

	if ((mydir=opendir(getcwd(dir, 1024)))==NULL) {
		cerr << "error: can't open current directory to read contents!" << endl;
		exit(-1);
	}

	if (!arguments.silent) {
		cout << "searching for mp3-files in  " << dir << "/" << endl;
	}
	while ((directoryEntry = readdir(mydir)) != NULL) {
		if (strstr(directoryEntry->d_name, ".mp3") != NULL) {
			TagLib::FileRef* f = new TagLib::FileRef(directoryEntry->d_name);
			if (f->tag()->isEmpty()) {
				continue;
			}

			mp3Collection->add(f);
		}
	}

	if (arguments.html) {
		writeHtmlFile(mp3Collection);

		/*
		if (arguments.verbose) {
			cout << "creating file " << dir << "/index.css" << endl;
		}
		if (arguments.verbose) {
			cout << "creating file " << dir << "/index.html" << endl;
		}
		if (html_head(myhtml, mytag) == -1) {
			cerr << "error: can't open html-file!" << endl;
			exit(-1);
		}

		if (stat_head(myhtml) == -1) {
			cerr << "error: can't open stat-file!" << endl;
			exit(-1);
		}

		if (info_head(myhtml->infofilename) == -1) {
			cerr << "error: can't open info-file!" << endl;
			exit(-1);
		}
		*/
	}

	if (arguments.xml) {
		writeXmlFile(mp3Collection, arguments.xsl, arguments.xsd);
	}

	get_mp3time(statheader->Length, statheader);
	strncpy(mystat->tot_length, statheader->Length_String, sizeof(mystat->tot_length));
	mysort->sort_tag();

	for (int i = 0; i <= mysort->cur_index; i++) {
		if (arguments.html) {
			/*
			if (html_content(&mysort->s_tag[i], myhtml) == -1) {
				cerr << "error: can't open html-file!" << endl;
				exit(-1);
			}

			if (info_body(myhtml->infofilename, myheader, &mysort->s_tag[i]) == -1) {
				cerr << "error: can't open html-file!" << endl;
				exit(-1);
			}
			*/
		}
	}

	if (arguments.html) {
		/*
		if (html_foot(myhtml) == -1) {
			cerr << "error: can't open html-file!" << endl;
			exit(-1);
		}

		if (!arguments.silent) {
			cout << "data successfully written to " << dir << "/index.html" << endl;
		}

		if (stat_content(mystat, myhtml) == -1) {
			cerr << "error: can't open stat-file!" << endl;
			exit(-1);
		}

		if (stat_foot(myhtml) == -1) {
			cerr << "error: can't open stat-file!" << endl;
			exit(-1);
		}

		if (info_foot(myhtml->infofilename) == -1) {
			cerr << "error: can't open info-file!" << endl;
			exit(-1);
		}
		*/
	}

	delete mytag;
	delete myhtml;
	delete myheader;
	delete mysort;
	delete mystat;
	delete statheader;

	return (0);
}
