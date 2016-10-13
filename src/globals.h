#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* Define this to enable DEBUG MODE */
#define DEBUG

#include "character.h"
#include <vector>

namespace globals {
	/** @brief enum az aktuális falnak */
	typedef enum { MIDDLE_WALL, LEFT_WALL, RIGHT_WALL, CEILING_WALL, FLOOR_WALL } WALL;

	/* Globals */
	static const std::string IMAGE_PATH = "images/";
	static const std::string DOLGOK_PATH = "dolgok/";
	static bool limitFPSvsync = false;
	static bool limitFPSmanually = false;
	static const int FPS_manual_limit = 60;
	static const std::string ERR_LOG_PATH = "error.log";

	//extern CHAR_SET CharSet;
	//extern int ConsoleWidthPixels, ConsoleHeightPixels;
	//extern int ConsoleWidth, ConsoleHeight;
	//static const int FontX = 8;
	//static const int FontY = 8;
	//extern SIZES UpperBoxSiz, UpperBoxPos;
	//extern SIZES SideBoxSiz, SideBoxPosLeft, SideBoxPosRight;
	//extern SIZES BottomBoxSiz, BottomBoxPos;
	//extern SIZES MiddleBoxSiz, MiddleBoxPos;
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
	//extern std::vector<std::vector<POINTS>> POINTS_LEFT;
	//extern std::vector<std::vector<POINTS>> POINTS_MIDDLE;
	//extern std::vector<std::vector<POINTS>> POINTS_RIGHT;
	//extern std::vector<std::vector<POINTS>> POINTS_TOP;
	//extern std::vector<std::vector<POINTS>> POINTS_BOTTOM;

	/* Global functions */
	std::vector<std::string> split(const std::string &s, char delim); /**< Split string by tokens */
	//void ClearBox(SIZES TopLeft, SIZES BoxSize);
	/**< Clear Polygon Box @param lu left-upper @param ru right-upper @param rl right-lower @param ll left-lower*/
	//void ClearPolygonBox(const POINTS& lu, const POINTS& ru, const POINTS& rl, const POINTS& ll);
	//void Pic2ASCIIandWrite(std::string PicName, SIZES TopLeft, SIZES BoxSize);
	/**< Warps and prints image, start destiny Points from TOPLEFT clockwise */
	//void Pic2ASCIIWarpandWrite(std::string PicName, WALL wall, std::vector<POINTS> PointsTo);
	//void Pic2ASCII(std::string PicName, SIZES BoxSize, ASCII_IMAGE &ASCII);
	//inline double PerspektivaKozeprolY(POINTS A, double x);
}

#endif