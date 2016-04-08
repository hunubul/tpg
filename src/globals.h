#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* Define this to enable DEBUG MODE */
#define DEBUG

#include "Image2ASCIIcolors.h"
#include "character.h"
#include <vector>

namespace globals {
	/* Globals */
	static const std::string IMAGE_PATH = "images/";
	static const std::string DOLGOK_PATH = "dolgok/";
	static const int FPS = 60;
	static const std::string ERR_LOG = "error.log";

	extern int ConsoleWidth, ConsoleHeight;
	static const int FontX = 8;
	static const int FontY = 8;
	extern SIZES UpperBoxSiz, UpperBoxPos;
	extern SIZES SideBoxSiz, SideBoxPosLeft, SideBoxPosRight;
	extern SIZES BottomBoxSiz, BottomBoxPos;
	extern SIZES MiddleBoxSiz, MiddleBoxPos;
	extern player p1;
	extern std::vector<enemy> enemies;

	/* Harc globals */
	static const int menu_height = 10; //Menü magassága
	static const int menu_ch_sel_width = 20; //Kijelölés szélessége
	static const int menu_harc_size = 3; //Size of menu_choices
	static std::string harc_menu_choices[] = { "Attack","Use item","Rest" };
	extern int menu_attack_size;
	extern std::vector< std::string> attack_choices;
	static const int menu_defense_size = 6;
	static std::string defense_choices[] = { "block from left","block from above","block from right","block from below","block frontal attack","don't block bro, don't do it!" };
	static const int menu_useitem_size = 3;
	static std::string useitem_choices[] = { "???","???","???" };
	static const int con_log_StartingPos = 4 * menu_ch_sel_width;

	/* Szoba globals */
	static const int MAX_RARITY = 10; //Max ritkasága egy itemnek
    // Szoba Vízszintes pontok
	static std::vector<std::vector<POINTS>> POINTS_LEFT = {
		{ { 0,0   },{ 190, 106 },{ 316, 180 } },
		{ { 0, 180},{ 190, 230 },{ 316, 268 } },
		{ { 0, 360},{ 190, 360 },{ 316, 360 } },
		{ { 0, 540},{ 190, 483 },{ 316, 450 } },
		{ { 0, 720},{ 190, 611 },{ 316, 537 } }
	};
	static std::vector<std::vector<POINTS>> POINTS_MIDDLE = {
		{ { 316,180},{ 638,180 },{ 958,180 } },
		{ { 316,268},{ 638,268 },{ 958,268 } },
		{ { 316,360},{ 638,360 },{ 958,360 } },
		{ { 316,450},{ 638,450 },{ 958,450 } },
		{ { 316,537},{ 638,537 },{ 958,537 } }
	};
	static std::vector<std::vector<POINTS>> POINTS_RIGHT = {
		{ { 958,180 },{ 1090,106 },{ 1280,  0 } },
		{ { 958,268 },{ 1090,230 },{ 1280,180 } },
		{ { 958,360 },{ 1090,360 },{ 1280,360 } },
		{ { 958,450 },{ 1090,483 },{ 1280,540 } },
		{ { 958,537 },{ 1090,611 },{ 1280,720 } }
	};
	static std::vector<std::vector<POINTS>> POINTS_TOP = {
		{ {   0, 0   },{ 638, 0   },{ 1028, 0 } },
		{ { 190, 106 },{ 638, 106 },{ 1090, 106 } },
		{ { 316, 180 },{ 638, 180 },{  958, 180 } }
	};
	static std::vector<std::vector<POINTS>> POINTS_BOTTOM = {
		{ { 316, 537 },{ 638, 537 },{ 1028, 537 } },
		{ { 190, 611 },{ 638, 611 },{ 1090, 611 } },
		{ {   0, 720 },{ 638, 720 },{ 1280, 720 } }
	};

	/* Global functions */
	std::vector<std::string> split(const std::string &s, char delim); /**< Split string by tokens */
	void ClearBox(SIZES TopLeft, SIZES BoxSize);
	/**< Clear Polygon Box @param lu left-upper @param ru right-upper @param rl right-lower @param ll left-lower*/
	void ClearPolygonBox(const POINTS& lu, const POINTS& ru, const POINTS& rl, const POINTS& ll);
	void Pic2ASCIIandWrite(std::string PicName, SIZES TopLeft, SIZES BoxSize);
	/**< Warps and prints image, start destiny Points from TOPLEFT clockwise */
	void Pic2ASCIIWarpandWrite(std::string PicName, std::vector<POINTS> PointsTo);
	void Pic2ASCII(std::string PicName, SIZES BoxSize, ASCII_IMAGE &ASCII);
}

#endif