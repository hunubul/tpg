#include "globals.h"
#include "logging.h"
#include "lodepng\lodepng.h"

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

void globals::Pic2ASCII(string PicName, SIZES TopLeft, SIZES BoxSize) {
	CONSOLEINFO Con;
	IMAGE PNG;
	SUBSECTION subsec;
	CHAR_SET CharSet;
	string PicPath = IMAGE_PATH + PicName + ".png";
	/*-----------Initializing CONSOLEINFO-----------*/
	Con.FontSize.X = FontX;
	Con.FontSize.Y = FontY;
	Con.CharAmount.X = BoxSize.X;
	Con.CharAmount.Y = BoxSize.Y;
	Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
	Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
	/*----------------------------------------------*/
	CharSetImporter(&CharSet, "8x8terminal.dat");
	CalculateWeights(&CharSet); /* Calculating charset weights... */
	int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
	ClearBox(TopLeft, BoxSize);
	if (!err) {
		CalculatePNGSizes(&PNG, &subsec, Con);
		PreciseProcessPNG(&PNG, subsec, CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
		WriteOutPic(&PNG, TopLeft, BoxSize);
		free(PNG.Image);
		free(PNG.ASCII_Image);
		free(PNG.ASCII_Color);
	}
	else ErrorOccured(PicName + ".png was not found");
}
void globals::ClearBox(SIZES TopLeft, SIZES BoxSize) {
#ifdef DEBUG
	TCODConsole::root->setDefaultForeground(TCODColor::grey);
	for (int i = 0; i < BoxSize.Y; i++) {
		for (int j = 0; j < BoxSize.X; j++) {
			TCODColor temp = TCODConsole::root->getCharForeground(TopLeft.X + j, TopLeft.Y + i) - TCODColor::darkestGrey;
			if (TCODConsole::root->getChar(TopLeft.X + j, TopLeft.Y + i) == ' ' || temp == TCODColor::black) TCODConsole::root->print(TopLeft.X + j, TopLeft.Y + i, ".");
		}
	}
	TCODConsole::root->setDefaultForeground(TCODColor::white);
#endif // DEBUG
}