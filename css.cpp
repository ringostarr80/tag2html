#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "css.h"

using namespace std;

void writeCssFile()
{
	ofstream cssFile("index.css");

	cssFile << "body {\n";
	cssFile << "\tfont-family: sans-serif;\n";
	cssFile << "\tfont-size:  12px;\n";
	cssFile << "\tbackground-color: #ABABAB;\n";
	cssFile << "\tcolor: #303030;\n";
	cssFile << "}\n";
	cssFile << "h1 {\n";
	cssFile << "\tfont-size: 16px;\n";
	cssFile << "\ttext-align: center;\n";
	cssFile << "}\n";
	cssFile << "table {\n";
	cssFile << "\tbackground-color: #FFFFFF;\n";
	cssFile << "\tborder: \"thin solid black\";\n";
	cssFile << "\twidth: 960px;\n";
	cssFile << "}\n";
	cssFile << "table.nb {\n";
	cssFile << "\tbackground-color: #FFFFFF;\n";
	cssFile << "\tborder: thin solid white;\n";
	cssFile << "}\n";
	cssFile << "td, th {\n";
	cssFile << "\tpadding: 2px 5px 2px 5px;\n";
	cssFile << "}\n";
	cssFile << "tbody > tr:nth-child(odd) {\n";
	cssFile << "\tbackground-color: white;\n";
	cssFile << "}\n";
	cssFile << "tbody > tr:nth-child(even) {\n";
	cssFile << "\tbackground-color: #EEEEEE;\n";
	cssFile << "}\n";
	cssFile << "a {\n";
	cssFile << "\tcolor: #444444;\n";
	cssFile << "}\n";

	cssFile.close();
}
