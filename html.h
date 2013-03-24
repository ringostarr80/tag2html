#ifndef HTML_H
#define HTML_H

#include <iostream>
#include <fstream>

#include <string.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include "MP3Collection.hpp"
#include "MP3Infos.hpp"
#include "classes.h"
#include "tag.h"

XERCES_CPP_NAMESPACE_USE

void writeHtmlFile(Tag2Html::MP3Collection* mp3Collection);
void writeStatFile(Tag2Html::MP3Collection* mp3Collection);

#endif
