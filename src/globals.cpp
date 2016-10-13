#include "globals.h"
#include "logging.h"
#include <algorithm>

using namespace std;

//int    globals::ConsoleWidth, globals::ConsoleHeight;
//int    globals::ConsoleWidthPixels, globals::ConsoleHeightPixels;
//SIZES  globals::UpperBoxSiz, globals::UpperBoxPos;
//SIZES  globals::SideBoxSiz, globals::SideBoxPosLeft, globals::SideBoxPosRight;
//SIZES  globals::BottomBoxSiz, globals::BottomBoxPos;
//SIZES  globals::MiddleBoxSiz, globals::MiddleBoxPos;
player globals::p1;
std::vector<enemy> globals::enemies;
//CHAR_SET globals::CharSet;
int globals::menu_attack_size = 5;
std::vector< std::string> globals::attack_choices = { "from left","from above","from right","from below","frontal attack" };
/*std::vector<std::vector<POINTS>> globals::POINTS_LEFT = {
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
};*/

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

//void globals::Pic2ASCII(string PicName, SIZES BoxSize, ASCII_IMAGE &ASCII) {
	//CONSOLEINFO Con;
	//IMAGE PNG;
	//SUBSECTION subsec;
	//string PicPath = IMAGE_PATH + PicName + ".png";
	///*-----------Initializing CONSOLEINFO-----------*/
	//Con.FontSize.X = FontX;
	//Con.FontSize.Y = FontY;
	//Con.CharAmount.X = BoxSize.X;
	//Con.CharAmount.Y = BoxSize.Y;
	//Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
	//Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
	///*----------------------------------------------*/
	//int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
	////ClearBox(TopLeft, BoxSize);
	//if (!err) {
	//	CalculatePNGSizes(&PNG, &subsec, Con);
	//	PreciseProcessPNG(&PNG, subsec, CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
	//	//WriteOutPic(&PNG, TopLeft, BoxSize);
	//	ASCII.HeightTile = PNG.HeightTile;
	//	ASCII.WidthTile = PNG.WidthTile;
	//	ASCII.ASCII_Color = PNG.ASCII_Color;
	//	//ASCII.ASCII_Image = PNG.ASCII_Image;
	//	free(PNG.Image);
	//	//free(PNG.ASCII_Image);
	//	//free(PNG.ASCII_Color);
	//}
	//else ErrorOccured(PicName + ".png was not found");
//}
//void globals::Pic2ASCIIandWrite(string PicName, SIZES TopLeft, SIZES BoxSize) {
	//CONSOLEINFO Con;
	//IMAGE PNG;
	//SUBSECTION subsec;
	//string PicPath = IMAGE_PATH + PicName + ".png";
	///*-----------Initializing CONSOLEINFO-----------*/
	//Con.FontSize.X = FontX;
	//Con.FontSize.Y = FontY;
	//Con.CharAmount.X = BoxSize.X;
	//Con.CharAmount.Y = BoxSize.Y;
	//Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
	//Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
	///*----------------------------------------------*/
	//int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
	//ClearBox(TopLeft, BoxSize);
	//if (!err) {
	//	CalculatePNGSizes(&PNG, &subsec, Con);
	//	PreciseProcessPNG(&PNG, subsec, CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
	//	WriteOutPic(&PNG, TopLeft, BoxSize);
	//	free(PNG.Image);
	//	free(PNG.ASCII_Image);
	//	free(PNG.ASCII_Color);
	//}
	//else ErrorOccured(PicName + ".png was not found");
//}
//void globals::Pic2ASCIIWarpandWrite(string PicName, WALL wall, vector<POINTS> PointsTo) {
	//if (PointsTo.size() < 4) FatalError("Not enough Points to warp!!");
	//CONSOLEINFO Con;
	//IMAGE PNG;
	//SUBSECTION subsec;
	//string PicPath = IMAGE_PATH + PicName + ".png";
	//SIZES BoxSize;
	//vector<double> Xpoints = { PointsTo[0].X,PointsTo[1].X,PointsTo[2].X,PointsTo[3].X };
	//vector<double> Ypoints = { PointsTo[0].Y,PointsTo[1].Y,PointsTo[2].Y,PointsTo[3].Y };
	//SIZES TopLeft = {
	//	(int)( min(PointsTo[0].X,PointsTo[3].X) ),
	//	(int)( min(PointsTo[0].Y,PointsTo[1].Y) )
	//};
	///*-----------Initializing CONSOLEINFO-----------*/
	//Con.FontSize.X = FontX;
	//Con.FontSize.Y = FontY;
	//Con.Size.X = (int)(*max_element(Xpoints.begin(), Xpoints.end()) - TopLeft.X);
	//Con.Size.Y = (int)(*max_element(Ypoints.begin(), Ypoints.end()) - TopLeft.Y);
 //   Con.CharAmount.X = BoxSize.X = Con.Size.X / FontX;
	//Con.CharAmount.Y = BoxSize.Y = Con.Size.Y / FontY;
	///*----------------------------------------------*/
	//int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
	////ClearBox(TopLeft, BoxSize);
	//if (!err) {
	//	/*Warp PNG */
	//	//vector<unsigned char> temp(PNG.Image, PNG.Image + PNG.Height*PNG.Width*4);
	//	cv::Mat temp(PNG.Height, PNG.Width, CV_8UC4, (void*)PNG.Image);
	//	vector<POINTS> PointsFrom = {
	//		{ 0, 0 },
	//		{ (double)PNG.Width, 0 },
	//		{ (double)PNG.Width, (double)PNG.Height },
	//		{ 0, (double)PNG.Height }
	//	};
	//	vector<POINTS> PointsTotemp;
	//	POINTS lu= { PointsTo[0].X - TopLeft.X, PointsTo[0].Y - TopLeft.Y };
	//	POINTS ru= { PointsTo[1].X - TopLeft.X, PointsTo[1].Y - TopLeft.Y };
	//	POINTS rl= { PointsTo[2].X - TopLeft.X, PointsTo[2].Y - TopLeft.Y };
	//	POINTS ll= { PointsTo[3].X - TopLeft.X, PointsTo[3].Y - TopLeft.Y };
	//	double lower;
	//	double VerLeftover; //Függõleges maradék amennyit fent és lent kihagy
	//	double VerLeftoverRatio;
	//	double RatioX = fmin(PointsTo[1].X - PointsTo[0].X, PointsTo[2].X - PointsTo[3].X);
	//	double RatioY = fmin(PointsTo[2].Y - PointsTo[1].Y, PointsTo[3].Y - PointsTo[0].Y);
	//	
	//	if ((double)RatioX/RatioY > (double)PNG.Width/PNG.Height) {
	//	// Függõleges lock
	//		ru.X = (double)RatioY*(double)PNG.Width / (double)PNG.Height;
	//		rl.X = (double)RatioY*(double)PNG.Width / (double)PNG.Height;

	//		double HorLeftover = (Con.Size.X - ru.X) / 2; //Vízszintes maradék amennyit balra és jobbra kihagy

	//		switch (wall) {
	//			case CEILING_WALL:
	//			case FLOOR_WALL:
	//			case LEFT_WALL:
	//				//PointsTo[0] = { lu.X + TopLeft.X + HorLeftover,fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - HorLeftover)*fabs(PointsTo[1].Y - PointsTo[0].Y)/Con.Size.X};
	//				//PointsTo[1] = { ru.X + TopLeft.X + HorLeftover,fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - (HorLeftover + ru.X))*fabs(PointsTo[1].Y - PointsTo[0].Y) / Con.Size.X };
	//				//PointsTo[2] = { rl.X + TopLeft.X + HorLeftover,fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - (HorLeftover + rl.X))*fabs(PointsTo[3].Y - PointsTo[2].Y) / Con.Size.X };
	//				//PointsTo[3] = { ll.X + TopLeft.X + HorLeftover,fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - HorLeftover)*fabs(PointsTo[3].Y - PointsTo[2].Y) / Con.Size.X };
	//				/*lu.Y = PerspektivaKozeprolY(PointsTo[0], lu.X + TopLeft.X + HorLeftover);
	//				ru.Y = PerspektivaKozeprolY(PointsTo[1], ru.X + TopLeft.X + HorLeftover);*/
	//				lu.Y = fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - HorLeftover)*fabs(PointsTo[1].Y - PointsTo[0].Y) / Con.Size.X;
	//				ru.Y = fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - (HorLeftover + ru.X))*fabs(PointsTo[1].Y - PointsTo[0].Y) / Con.Size.X;
	//				lower = fmin(lu.Y, ru.Y);
	//				PointsTotemp = {
	//					{ lu.X, lu.Y - lower }, //Warp to
	//					{ ru.X, ru.Y - lower },
	//					{ rl.X, Con.Size.Y-fabs(PointsTo[3].Y - PointsTo[2].Y) + (Con.Size.X - (HorLeftover + rl.X))*fabs(PointsTo[3].Y - PointsTo[2].Y) / Con.Size.X - lower },
	//					{ ll.X, Con.Size.Y-fabs(PointsTo[3].Y - PointsTo[2].Y) + (Con.Size.X - HorLeftover)*fabs(PointsTo[3].Y - PointsTo[2].Y) / Con.Size.X - lower }
	//				};
	//				break;
	//			case MIDDLE_WALL:
	//				PointsTotemp = {
	//					{ lu.X , lu.Y }, //Warp to
	//					{ ru.X , ru.Y },
	//					{ rl.X , rl.Y },
	//					{ ll.X , ll.Y }
	//				};
	//				break;
	//			case RIGHT_WALL:
	//				lu.Y = fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - (HorLeftover + ru.X))*fabs(PointsTo[1].Y - PointsTo[0].Y) / Con.Size.X;
	//				ru.Y = fabs(PointsTo[1].Y - PointsTo[0].Y) - (Con.Size.X - HorLeftover)*fabs(PointsTo[1].Y - PointsTo[0].Y) / Con.Size.X;
	//				lower = fmin(lu.Y, ru.Y);
	//				PointsTotemp = {
	//					{ lu.X, lu.Y - lower }, //Warp to
	//					{ ru.X, ru.Y - lower },
	//					{ rl.X, Con.Size.Y - fabs(PointsTo[3].Y - PointsTo[2].Y) + (Con.Size.X - HorLeftover)*fabs(PointsTo[3].Y - PointsTo[2].Y) / Con.Size.X - lower },
	//					{ ll.X, Con.Size.Y - fabs(PointsTo[3].Y - PointsTo[2].Y) + (Con.Size.X - (HorLeftover + rl.X))*fabs(PointsTo[3].Y - PointsTo[2].Y) / Con.Size.X - lower }
	//				};
	//				break;
	//			default:
	//				FatalError("Invalid wall");
	//				break;
	//		}
	//	} else {
	//	// Vízszintes lock
	//		rl.Y = (double)Con.Size.X*(double)PNG.Height / (double)PNG.Width;
	//		ll.Y = (double)Con.Size.X*(double)PNG.Height / (double)PNG.Width;

	//		switch (wall) {
	//		case LEFT_WALL:
	//			VerLeftover = (Con.Size.Y - ll.Y) / 2;
	//			VerLeftoverRatio = (PointsTo[2].Y - PointsTo[1].Y)*VerLeftover / Con.Size.Y;
	//			PointsTo[0].Y += VerLeftover;
	//			PointsTo[1].Y += VerLeftoverRatio;
	//			PointsTo[2].Y -= VerLeftoverRatio;
	//			PointsTo[3].Y -= VerLeftover;
	//		case CEILING_WALL:
	//		case FLOOR_WALL:
	//			lu.Y = PerspektivaKozeprolY(PointsTo[0], lu.X + TopLeft.X);
	//			ru.Y = PerspektivaKozeprolY(PointsTo[1], ru.X + TopLeft.X);
	//			lower = fmin(lu.Y, ru.Y);
	//			PointsTotemp = {
	//				{ lu.X, lu.Y - lower }, //Warp to
	//				{ ru.X, ru.Y - lower },
	//				{ rl.X, PerspektivaKozeprolY(PointsTo[2],rl.X + TopLeft.X) - lower },
	//				{ ll.X, PerspektivaKozeprolY(PointsTo[3],ll.X + TopLeft.X) - lower }
	//			};
	//			break;
	//		case MIDDLE_WALL:
	//			PointsTotemp = {
	//				{ lu.X , lu.Y }, //Warp to
	//				{ ru.X , ru.Y },
	//				{ rl.X , rl.Y },
	//				{ ll.X , ll.Y }
	//			};
	//			break;
	//		case RIGHT_WALL:
	//			VerLeftover = (Con.Size.Y - rl.Y) / 2;
	//			VerLeftoverRatio = (PointsTo[3].Y - PointsTo[0].Y)*VerLeftover / Con.Size.Y;
	//			PointsTo[0].Y += VerLeftoverRatio;
	//			PointsTo[1].Y += VerLeftover;
	//			PointsTo[2].Y -= VerLeftover;
	//			PointsTo[3].Y -= VerLeftoverRatio;
	//			lu.Y = PerspektivaKozeprolY(PointsTo[0], lu.X + TopLeft.X);
	//			ru.Y = PerspektivaKozeprolY(PointsTo[1], ru.X + TopLeft.X);
	//			lower = fmin(lu.Y, ru.Y);
	//			PointsTotemp = {
	//				{ lu.X, lu.Y - lower }, //Warp to
	//				{ ru.X, ru.Y - lower },
	//				{ rl.X, PerspektivaKozeprolY(PointsTo[2],rl.X + TopLeft.X) - lower },
	//				{ ll.X, PerspektivaKozeprolY(PointsTo[3],ll.X + TopLeft.X) - lower }
	//			};
	//			break;
	//		default:
	//			FatalError("Invalid wall.");
	//			break;
	//		}
	//	}
	//	cv::Mat tempmat = OpenWarpPerspective( temp,
	//		  PointsFrom[0],   PointsFrom[1],   PointsFrom[2],   PointsFrom[3],
	//		PointsTotemp[0], PointsTotemp[1], PointsTotemp[2], PointsTotemp[3]
	//	);
	//	//std::vector<std::uint8_t> ImageBuffer;
	//	//lodepng::encode(ImageBuffer, tempmat.data, tempmat.cols, tempmat.rows);
	//	//lodepng::save_file(ImageBuffer, ("./asd/"+PicName + "asd.png").c_str());
	//	free(PNG.Image);
	//	PNG.Image = (unsigned char*)tempmat.data;
	//	PNG.Height = tempmat.rows;
	//	PNG.Width = tempmat.cols;
	//	CalculatePNGSizes(&PNG, &subsec, Con);
	//	/*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
	//	PreciseProcessPNG(&PNG, subsec, CharSet);
	//	TopLeft = { TopLeft.X / FontX, TopLeft.Y / FontY };
	//	WriteOutPic(&PNG, TopLeft, BoxSize);
	//	//free(PNG.Image);
	//	free(PNG.ASCII_Image);
	//	free(PNG.ASCII_Color);
	//}
	//else ErrorOccured(PicName + ".png was not found");
//}
//void globals::ClearBox(SIZES TopLeft, SIZES BoxSize) {
//#ifdef DEBUG
//	TCODConsole::root->setDefaultForeground(TCODColor::grey);
//	for (int i = 0; i < BoxSize.Y; i++) {
//		for (int j = 0; j < BoxSize.X; j++) {
//			TCODColor temp = TCODConsole::root->getCharForeground(TopLeft.X + j, TopLeft.Y + i) - TCODColor::darkestGrey;
//			if (TCODConsole::root->getChar(TopLeft.X + j, TopLeft.Y + i) == ' ' || temp == TCODColor::black) TCODConsole::root->print(TopLeft.X + j, TopLeft.Y + i, ".");
//		}
//	}
//	TCODConsole::root->setDefaultForeground(TCODColor::white);
//#endif // DEBUG
//}
//void globals::ClearPolygonBox(const POINTS& lu, const POINTS& ru, const POINTS& rl, const POINTS& ll) {
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
//}

/*inline double globals::PerspektivaKozeprolY(POINTS A, double x) {
	if(ConsoleWidthPixels/2 < A.X)
		if(ConsoleHeightPixels/2 < A.Y)
			return (x - ConsoleWidthPixels / 2)*(A.Y - ConsoleHeightPixels / 2) / (A.X - ConsoleWidthPixels / 2) + ConsoleHeightPixels / 2;
		else // if(ConsoleHeightPixels/2 >= A.Y)
			return ConsoleHeightPixels / 2 - (x - ConsoleWidthPixels / 2)*(ConsoleHeightPixels / 2 - A.Y) / (A.X - ConsoleWidthPixels / 2);
	else // if(ConsoleWidthPixels/2 >= A.X)
		if (ConsoleHeightPixels/2 < A.Y)
			return A.Y - (x - A.X)*(A.Y - ConsoleHeightPixels / 2) / (ConsoleWidthPixels / 2 - A.X);
		else // if(ConsoleHeightPixels/2 >= A.Y)
			return (x - A.X)*(ConsoleHeightPixels / 2 - A.Y) / (ConsoleWidthPixels / 2 - A.X) + A.Y;
}*/