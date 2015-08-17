#include "Image2ASCIIcolors.h"
#include "character.h"

/* Globals */
extern const std::string IMAGE_PATH="images/";
extern const int FPS=60;
extern const std::string ERR_LOG="error.log";

int ConsoleWidth,ConsoleHeight;
extern const int FontX=8;
extern const int FontY=8;
SIZES UpperBoxSiz, UpperBoxPos;
SIZES SideBoxSiz,  SideBoxPosLeft, SideBoxPosRight;
SIZES BottomBoxSiz,BottomBoxPos;
SIZES MiddleBoxSiz,MiddleBoxPos;
player p1;
std::vector<enemy> enemies;

/* Harc globals */
extern const int menu_height=10; //Menü magassága
extern const int menu_ch_sel_width=20; //Kijelölés szélessége
extern const int menu_harc_size=3; //Size of menu_choices
std::string harc_menu_choices[]= {"Attack","Use item","Rest"};
extern const int menu_attack_size=5;
std::string attack_choices[]= {"from left","from above","from right","from below","frontal attack"};
extern const int menu_defense_size=5;
std::string defense_choices[]= {"block from left","block from above","block from right","block from below","block frontal attack"};
extern const int menu_useitem_size=3;
std::string useitem_choices[]= {"???","???","???"};
extern const int con_log_StartingPos=4*menu_ch_sel_width;

/* Szoba globals */
extern const int MAX_RARITY = 10; //Max ritkasága egy itemnek
