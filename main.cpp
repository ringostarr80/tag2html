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

#include <id3/globals.h>

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
#include <id3/tag.h>

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
	{0, 0, 0, 0, 0, 0}
};

/* Struktur zur Parameterübergabe */
struct arguments {
	char *args[0];    /* ARG1 */
	char *output_file;
	bool silent, verbose, version, html, xml;
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

	char xml_file[12], xsd_file[12], xsl_file[12];
	strncpy(xml_file, "./index.xml", sizeof(xml_file));
	strncpy(xsd_file, "./index.xsd", sizeof(xsd_file));
	strncpy(xsl_file, "./index.xsl", sizeof(xsl_file));

	// structs
	DIR *mydir;
	struct dirent *mydirent;

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
	while ((mydirent = readdir(mydir)) != NULL) {
		if (strstr(mydirent->d_name, ".mp3") != NULL) {
			//ID3_Tag myTag(mydirent->d_name);
			//ID3_Frame* myFrame = myTag.Find(ID3FID_TITLE);
			//if (myFrame != NULL) {
			//	cout << myFrame->GetDescription() << endl;
			//}

			// found .mp3 so lets get started
			mysort->cur_index++;
			mystat->mp3_count++;

			get_mp3tags(mydirent->d_name, mytag);
			//get_mp3tags_v2(mydirent->d_name, mytag);
			mystat->tot_filesize+= get_filesize(mydirent->d_name);
			get_albumcount(mytag->album, mystat);
			get_artistcount(mytag->artist, mystat);
			get_mp3header(mydirent->d_name, myheader);
			statheader->Length+= myheader->Length;
			get_mp3time(myheader->Length, myheader);
			strncpy(mytag->filename, mydirent->d_name, sizeof(mytag->filename));
			strncpy(mytag->length, myheader->Length_String, sizeof(mytag->length));

			mysort->make_array(mytag);

			switch (mytag->layout_flag) {
				case 0: mytag->layout_flag=1;
					break;
				case 1: mytag->layout_flag=0;
					break;
			}
		}
	}

	if(arguments.html)
	{
		if (arguments.verbose) {
			cout << "creating file " << dir << "/index.css" << endl;
		}
		if (html_css(myhtml) == -1) {
			cerr << "error: can't open css-file!" << endl;
			exit(-1);
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
	}

	if (arguments.xml) {
		if (arguments.verbose) {
			cout << "creating file " << dir << "/index.xsd" << endl;
		}
		if (xsd_schema(xsd_file) == -1) {
			cerr << "error: can't open xsd-file!" << endl;
			exit(-1);
		}
		if (arguments.verbose) {
			cout << "creating file " << dir << "/index.xsl" << endl;
		}
		if (xsl_stylesheet(xsl_file) == -1) {
			cerr << "error: can't open xsl-file!" << endl;
			exit(-1);
		}
		if (arguments.verbose) {
			cout << "creating file " << dir << "/index.xml" << endl;
		}
		if (xml_head(xml_file) == -1) {
			cerr << "error: can't open xml-file!" << endl;
			exit(-1);
		}
	}

	get_mp3time(statheader->Length, statheader);
	strncpy(mystat->tot_length, statheader->Length_String, sizeof(mystat->tot_length));
	mysort->sort_tag();

	for (int i = 0; i <= mysort->cur_index; i++) {
		if (arguments.html) {
			if (html_content(&mysort->s_tag[i], myhtml) == -1) {
				cerr << "error: can't open html-file!" << endl;
				exit(-1);
			}

			if (info_body(myhtml->infofilename, myheader, &mysort->s_tag[i]) == -1) {
				cerr << "error: can't open html-file!" << endl;
				exit(-1);
			}
		}

		if (arguments.xml) {
			if (xml_content(&mysort->s_tag[i], myheader, xml_file) == -1) {
				cerr << "error: can't open xml-file!" << endl;
				exit(-1);
			}
		}
	}

	if (arguments.html) {
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
	}

	if (arguments.xml) {
		if (xml_foot(xml_file) == -1) {
			cerr << "error: can't open xml-file!" << endl;
			exit(-1);
		}
		if (!arguments.silent) {
			cout << "data successfully written to " << dir << "/index.xml" << endl;
		}
	}

	delete mytag;
	delete myhtml;
	delete myheader;
	delete mysort;
	delete mystat;
	delete statheader;

	return (0);
}
