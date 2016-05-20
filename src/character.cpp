#include "Image2ASCIIcolors.h"
#include "lodepng.h"
#include "character.h"
#include "level.h"
#include "logging.h"
#include "globals.h"

using namespace globals;

SIZES enemy::BoxSize;
SIZES enemy::TopLeft;

character::character(int maxhp,int maxstam,int defense,int offense) :
    maxhp(maxhp),hp(maxhp),maxstam(maxstam),stam(maxstam),defense(defense),offense(offense) {}

void character::BrokenInv() {
	if (wearing.weapon.broken) { wearing.weapon= (weapon)emptyweapon(); }
	if (wearing.shield.broken) { wearing.shield = (shield)emptyshield(); }
	if (wearing.chestpiece.broken) { wearing.chestpiece = (chestpiece)emptychestpiece(); }
	if (wearing.headpiece.broken) { wearing.headpiece = (headpiece)emptyheadpiece(); }
	if (wearing.gloves.broken) { wearing.gloves = (gloves)emptygloves(); }
	if (wearing.pants.broken) { wearing.pants = (pants)emptypants(); }
	if (wearing.legs.broken) { wearing.legs = (legs)emptylegs(); }
}

bool character::damage(double DMG) {
    double asd=(rand()%80+81)/100.0;
    hp-=(int)round((asd/getDefValue())*DMG);
    if(hp<0) hp=0;
    if(asd>1.49999) return true;
    return false;
}

void character::addHP(int pHp) {
    hp+=pHp;
    if(hp>100) hp=100;
}

void character::addStamina(int pStam) {
    stam+=pStam;
    if(stam>100) stam=100;
}

void character::subStamina(int pStam) {
    stam-=pStam;
    if(stam<0) stam=0;
}

double character::getDefValue() {
	return wearing.chestpiece.defx * wearing.gloves.defx * wearing.headpiece.defx * wearing.legs.defx * wearing.pants.defx;
}

player::player() : character(100,100,4,4) {
	wearing.weapon;
	wearing.shield;
	wearing.chestpiece = (chestpiece)emptychestpiece();
	wearing.gloves = (gloves)emptygloves();
	wearing.pants = (pants)emptypants();
	wearing.legs = (legs)emptylegs();
	wearing.headpiece = (headpiece)emptyheadpiece();
}

enemy::enemy(std::string name,int defense,int offense,ADIR def,ADIR atc) :
    character(100,100,defense,offense),def(def),atc(atc),name(name) {
    enemy::BoxSize={ConsoleWidth*911/1280,ConsoleHeight*666/720};
    enemy::TopLeft={ConsoleWidth-enemy::BoxSize.X,0};
	wearing.weapon = (weapon)emptyweapon();
	wearing.shield = (shield)emptyshield();
	wearing.chestpiece = (chestpiece)emptychestpiece();
	wearing.headpiece = (headpiece)emptyheadpiece();
	wearing.gloves = (gloves)emptygloves();
	wearing.pants = (pants)emptypants();
	wearing.legs = (legs)emptylegs();
	int asd = rand() % 5;
	if (asd == 0) wearing.weapon;
	if (asd == 1) wearing.shield;
    CONSOLEINFO Con;
    SUBSECTION subsec;
    std::string PicPath = IMAGE_PATH+name+".png";
    /*-----------Initializing CONSOLEINFO-----------*/
    Con.FontSize.X = FontX;
    Con.FontSize.Y = FontY;
    Con.CharAmount.X = BoxSize.X;
    Con.CharAmount.Y = BoxSize.Y;
    Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
    Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
    /*----------------------------------------------*/
    int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
    if(!err) {
        CalculatePNGSizes(&PNG,&subsec,Con);
        PreciseProcessPNG(&PNG,subsec,CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
        free(PNG.Image);
        PNG.Image=NULL;
	} else {
		std::string PicPath = IMAGE_PATH + "Editing/missing_enemy.png";
		err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
		if (!err) {
			CalculatePNGSizes(&PNG, &subsec, Con);
			PreciseProcessPNG(&PNG, subsec, CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
			free(PNG.Image);
			PNG.Image = NULL;
		} else FatalError("Missing missing_enemy");
	}
}

enemy::~enemy() {
//    free(PNG.Image);
//    free(PNG.ASCII_Image);
//    free(PNG.Weight);
}
