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

bool character::damage(int DMG) {
    double dmgMult=(rand()%80+81)/100.0;
    hp-=(int)round((DMG/defense)*dmgMult);
    if(hp<0) hp=0;
    if(dmgMult>1.49999) return true;
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

player::player() : character(100,100,4,4) {
	wearing.weapon;
	wearing.shield;
}

enemy::enemy(std::string name,int defense,int offense,ADIR def,ADIR atc) :
    character(100,100,defense,offense),def(def),atc(atc),name(name) {
    enemy::BoxSize={ConsoleWidth*911/1280,ConsoleHeight*666/720};
    enemy::TopLeft={ConsoleWidth-enemy::BoxSize.X,0};
	int asd = rand() % 5;
	if (asd == 0) wearing.weapon;
	if (asd == 1) wearing.shield;
    CONSOLEINFO Con;
    SUBSECTION subsec;
    CHAR_SET CharSet;
    std::string PicPath = IMAGE_PATH+name+".png";
    /*-----------Initializing CONSOLEINFO-----------*/
    Con.FontSize.X = FontX;
    Con.FontSize.Y = FontY;
    Con.CharAmount.X = BoxSize.X;
    Con.CharAmount.Y = BoxSize.Y;
    Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
    Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
    /*----------------------------------------------*/
    CharSetImporter(&CharSet,"8x8terminal.dat");
    CalculateWeights(&CharSet); /* Calculating charset weights... */
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
