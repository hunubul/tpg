#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* Define this to enable DEBUG MODE */
#define DEBUG

#include "Image2ASCIIcolors.h"
#include "character.h"

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
	static const int menu_attack_size = 5;
	static std::vector< std::string> attack_choices = { "from left","from above","from right","from below","frontal attack" };
	static const int menu_defense_size = 5;
	static std::string defense_choices[] = { "block from left","block from above","block from right","block from below","block frontal attack" };
	static const int menu_useitem_size = 3;
	static std::string useitem_choices[] = { "???","???","???" };
	static const int con_log_StartingPos = 4 * menu_ch_sel_width;
	static std::string WeaponArtName;

	/* Szoba globals */
	static const int MAX_RARITY = 10; //Max ritkasága egy itemnek

	/* Global functions */
	std::vector<std::string> split(const std::string &s, char delim); /**< Split string by tokens */
	void ClearBox(SIZES TopLeft, SIZES BoxSize);
	void Pic2ASCIIandWrite(std::string PicName, SIZES TopLeft, SIZES BoxSize);
	void Pic2ASCII(std::string PicName, SIZES BoxSize, ASCII_IMAGE &ASCII);
}

#endif