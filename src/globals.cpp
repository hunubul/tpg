#include "globals.h"
#include "logging.h"
#include "lodepng\lodepng.h"
#include <algorithm>

using namespace std;

int    globals::ConsoleWidth, globals::ConsoleHeight;
SIZES  globals::UpperBoxSiz, globals::UpperBoxPos;
SIZES  globals::SideBoxSiz, globals::SideBoxPosLeft, globals::SideBoxPosRight;
SIZES  globals::BottomBoxSiz, globals::BottomBoxPos;
SIZES  globals::MiddleBoxSiz, globals::MiddleBoxPos;
player globals::p1;
std::vector<enemy> globals::enemies;
int globals::menu_attack_size = 5;
std::vector< std::string> globals::attack_choices = { "from left","from above","from right","from below","frontal attack" };
std::vector<std::vector<POINTS>> globals::POINTS_LEFT = {
	{ {  0, 0   },{   0, 180 },{   0, 360 },{   0, 540 },{   0, 720 } },
	{ {190, 106 },{ 190, 230 },{ 190, 360 },{ 190, 483 },{ 190, 611 } },
	{ {316, 180 },{ 316, 268 },{ 316, 360 },{ 316, 450 },{ 316, 537 } }
};
std::vector<std::vector<POINTS>> globals::POINTS_MIDDLE = {
	{ { 316,180 },{ 316,268 },{ 316,360 },{ 316,450 },{ 316,537 } },
	{ { 638,180 },{ 638,268 },{ 638,360 },{ 638,450 },{ 638,537 } },
	{ { 958,180 },{ 958,268 },{ 958,360 },{ 958,450 },{ 958,537 } }
};
std::vector<std::vector<POINTS>> globals::POINTS_RIGHT = {
	{ {  958,180 },{  958,268 },{  958,360 },{  958,450 },{  958,537 } },
	{ { 1090,106 },{ 1090,230 },{ 1090,360 },{ 1090,483 },{ 1090,611 } },
	{ { 1280,0   },{ 1280,180 },{ 1280,360 },{ 1280,540 },{ 1280,720 } }
};
std::vector<std::vector<POINTS>> globals::POINTS_TOP = {
	{ {   0, 0   },{  190, 106 },{ 316, 180 } },
	{ { 638, 0   },{  638, 106 },{ 638, 180 } },
	{ {1280, 0   },{ 1090, 106 },{ 958, 180 } }
};
std::vector<std::vector<POINTS>> globals::POINTS_BOTTOM = {
	{ {  316, 537 },{  190, 611 },{    0, 720 } },
	{ {  638, 537 },{  638, 611 },{  638, 720 } },
	{ { 1028, 537 },{ 1090, 611 },{ 1280, 720 } }
};

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
	elems.push_back(temp);
	return elems;
}

void globals::Pic2ASCII(string PicName, SIZES BoxSize, ASCII_IMAGE &ASCII) {
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
	//ClearBox(TopLeft, BoxSize);
	if (!err) {
		CalculatePNGSizes(&PNG, &subsec, Con);
		PreciseProcessPNG(&PNG, subsec, CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
		//WriteOutPic(&PNG, TopLeft, BoxSize);
		ASCII.HeightTile = PNG.HeightTile;
		ASCII.WidthTile = PNG.WidthTile;
		ASCII.ASCII_Color = PNG.ASCII_Color;
		//ASCII.ASCII_Image = PNG.ASCII_Image;
		free(PNG.Image);
		//free(PNG.ASCII_Image);
		//free(PNG.ASCII_Color);
	}
	else ErrorOccured(PicName + ".png was not found");
}
void globals::Pic2ASCIIandWrite(string PicName, SIZES TopLeft, SIZES BoxSize) {
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
void globals::Pic2ASCIIWarpandWrite(string PicName, vector<POINTS> PointsTo) {
	if (PointsTo.size() < 4) FatalError("Not enough Points to warp!!");
	CONSOLEINFO Con;
	IMAGE PNG;
	SUBSECTION subsec;
	CHAR_SET CharSet;
	string PicPath = IMAGE_PATH + PicName + ".png";
	SIZES BoxSize;
	vector<double> Xpoints = { PointsTo[0].X,PointsTo[1].X,PointsTo[2].X,PointsTo[3].X };
	vector<double> Ypoints = { PointsTo[0].Y,PointsTo[1].Y,PointsTo[2].Y,PointsTo[3].Y };
	SIZES TopLeft = {
		static_cast<int>( min(PointsTo[0].X,PointsTo[3].X) ),
		static_cast<int>( min(PointsTo[0].Y,PointsTo[1].Y) )
	};
	/*-----------Initializing CONSOLEINFO-----------*/
	Con.FontSize.X = FontX;
	Con.FontSize.Y = FontY;
	Con.Size.X = (int)(*max_element(Xpoints.begin(), Xpoints.end()) - TopLeft.X);
	Con.Size.Y = (int)(*max_element(Ypoints.begin(), Ypoints.end()) - TopLeft.Y);
    Con.CharAmount.X = BoxSize.X = Con.Size.X / FontX;
	Con.CharAmount.Y = BoxSize.Y = Con.Size.Y / FontY;
	/*----------------------------------------------*/
	CharSetImporter(&CharSet, "8x8terminal.dat");
	CalculateWeights(&CharSet); /* Calculating charset weights... */
	int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
	//ClearBox(TopLeft, BoxSize);
	if (!err) {
		/*Warp PNG */
		//vector<unsigned char> temp(PNG.Image, PNG.Image + PNG.Height*PNG.Width*4);
		cv::Mat temp(PNG.Height,PNG.Width, CV_8UC4, (void*)PNG.Image);
		vector<POINTS> PointsFrom = {
			{ 0, 0 },
			{ (double)PNG.Width, 0 },
			{ (double)PNG.Width, (double)PNG.Height },
			{ 0, (double)PNG.Height }
		};
		vector<POINTS> PointsTotemp = {
			{ PointsTo[0].X - TopLeft.X, PointsTo[0].Y - TopLeft.Y }, //Warp to
			{ PointsTo[1].X - TopLeft.X, PointsTo[1].Y - TopLeft.Y },
			{ PointsTo[2].X - TopLeft.X, PointsTo[2].Y - TopLeft.Y },
			{ PointsTo[3].X - TopLeft.X, PointsTo[3].Y - TopLeft.Y }
		};
		cv::Mat tempmat = OpenWarpPerspective( temp,
			  PointsFrom[0],   PointsFrom[1],   PointsFrom[2],   PointsFrom[3],
			PointsTotemp[0], PointsTotemp[1], PointsTotemp[2], PointsTotemp[3]
		);
		//std::vector<std::uint8_t> ImageBuffer;
		//lodepng::encode(ImageBuffer, tempmat.data, tempmat.cols, tempmat.rows);
		//lodepng::save_file(ImageBuffer, ("./asd/"+PicName + "asd.png").c_str());
		free(PNG.Image);
		PNG.Image = (unsigned char*)tempmat.data;
		PNG.Height = tempmat.rows;
		PNG.Width = tempmat.cols;
		CalculatePNGSizes(&PNG, &subsec, Con);
		/*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
		PreciseProcessPNG(&PNG, subsec, CharSet);
		TopLeft = { TopLeft.X / FontX, TopLeft.Y / FontY };
		WriteOutPic(&PNG, TopLeft, BoxSize);
		//free(PNG.Image);
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
void globals::ClearPolygonBox(const POINTS& lu, const POINTS& ru, const POINTS& rl, const POINTS& ll) {
	//Check if params are valid
	/*if (lu.X > ru.X||ru.Y>rl.Y||rl.X < ll.X||ll.Y < lu.Y) throw "ClearPolygonBox params not valid!";
	TCODConsole::root->setDefaultForeground(TCODColor::grey);
	for (int i = lu.Y; i < ll.Y; i++) {
		for (int j = 0; j < BoxSize.X; j++) {
			TCODColor temp = TCODConsole::root->getCharForeground(lu.X + j, lu.Y + i) - TCODColor::darkestGrey;
			if (TCODConsole::root->getChar(lu.X + j, lu.Y + i) == ' ' || temp == TCODColor::black)
				TCODConsole::root->print(lu.X + j, lu.Y + i, ".");
		}
	}
	TCODConsole::root->setDefaultForeground(TCODColor::white);*/
}