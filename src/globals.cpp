#include "globals.h"

using namespace std;

int    globals::ConsoleWidth, globals::ConsoleHeight;
SIZES  globals::UpperBoxSiz, globals::UpperBoxPos;
SIZES  globals::SideBoxSiz, globals::SideBoxPosLeft, globals::SideBoxPosRight;
SIZES  globals::BottomBoxSiz, globals::BottomBoxPos;
SIZES  globals::MiddleBoxSiz, globals::MiddleBoxPos;
player globals::p1;
std::vector<enemy> globals::enemies;

/** Split string by tokens
@param s string to tokenize
@param delim char delimeter */
vector<string> globals::split(const string &s, char delim) {
	vector<string> elems;
	string temp;
	for each (char var in s) {
		if (var == delim) {
			elems.push_back(temp);
			temp.clear();
		}
		else temp.push_back(var);
	}
	if (elems.size() == 0) return vector<string>(1, "Error");
	return elems;
}