CC = g++
FILE_MP3Collection_OBJ = MP3Collection.o
FILE_MP3Infos_OBJ = MP3Infos.o
FILE_CLASSES_OBJ = classes.o
FILE_CSS_OBJ = css.o
FILE_GETHEADER_OBJ = getheader.o
FILE_GETTAG_OBJ = gettag.o
FILE_GETTAG_V2_OBJ = gettag_v2.o
FILE_HTML_OBJ = html.o
FILE_INFO_OBJ = info.o
FILE_MAIN_OBJ = main.o
FILE_SORT_OBJ = sort.o
FILE_STAT_OBJ = stat.o
FILE_XML_OBJ = xml.o

build: $(FILE_MP3Collection_OBJ) $(FILE_MP3Infos_OBJ) $(FILE_CLASSES_OBJ) $(FILE_CSS_OBJ) $(FILE_GETHEADER_OBJ) $(FILE_GETTAG_OBJ) $(FILE_GETTAG_V2_OBJ) $(FILE_HTML_OBJ) $(FILE_INFO_OBJ) $(FILE_MAIN_OBJ) $(FILE_SORT_OBJ) $(FILE_STAT_OBJ) $(FILE_XML_OBJ)
	$(CC) -Wall -o tag2html $(FILE_MP3Collection_OBJ) $(FILE_MP3Infos_OBJ) $(FILE_CLASSES_OBJ) $(FILE_CSS_OBJ) \
	$(FILE_GETHEADER_OBJ) $(FILE_GETTAG_OBJ) $(FILE_GETTAG_V2_OBJ) $(FILE_HTML_OBJ) $(FILE_INFO_OBJ) \
	$(FILE_MAIN_OBJ) $(FILE_SORT_OBJ) $(FILE_STAT_OBJ) $(FILE_XML_OBJ) \
	-ltag -ltag_c -lxerces-c -lz

$(FILE_MP3Collection_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_MP3Collection_OBJ).d -o $(FILE_MP3Collection_OBJ) MP3Collection.cpp

$(FILE_MP3Infos_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_MP3Infos_OBJ).d -o $(FILE_MP3Infos_OBJ) MP3Infos.cpp

$(FILE_CLASSES_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_CLASSES_OBJ).d -o $(FILE_CLASSES_OBJ) classes.cpp

$(FILE_CSS_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_CSS_OBJ).d -o $(FILE_CSS_OBJ) css.cpp

$(FILE_GETHEADER_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_GETHEADER_OBJ).d -o $(FILE_GETHEADER_OBJ) getheader.cpp

$(FILE_GETTAG_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_GETTAG_OBJ).d -o $(FILE_GETTAG_OBJ) gettag.cpp

$(FILE_GETTAG_V2_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_GETTAG_V2_OBJ).d -o $(FILE_GETTAG_V2_OBJ) gettag_v2.cpp

$(FILE_HTML_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_HTML_OBJ).d -o $(FILE_HTML_OBJ) html.cpp

$(FILE_INFO_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_INFO_OBJ).d -o $(FILE_INFO_OBJ) info.cpp

$(FILE_MAIN_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_MAIN_OBJ).d -o $(FILE_MAIN_OBJ) main.cpp

$(FILE_SORT_OBJ):
	g++ -Wall -Wall -c -O3 -MMD -MP -MF $(FILE_SORT_OBJ).d -o $(FILE_SORT_OBJ) sort.cpp

$(FILE_STAT_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_STAT_OBJ).d -o $(FILE_STAT_OBJ) stat.cpp

$(FILE_XML_OBJ):
	g++ -Wall -c -O3 -MMD -MP -MF $(FILE_XML_OBJ).d -o $(FILE_XML_OBJ) xml.cpp

clean:
	rm -f *.o *.o.d tag2html
