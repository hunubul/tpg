/**
 * @file main.cpp
 * @brief main függvény
 */

#include <fstream>
#include <time.h>
#include "Image2ASCIIcolors.h"
#include "character.h"
#include "level.h"
#include "settings.h"
#include "logging.h"
#include "globals.h"

using namespace globals;
using namespace std;

void BeolvasEnemyk();
void LoadingScreen();

//4:3  - 1024x768
//16:9 - 1280x720

int main(int argc, char *argv[]) {
	srand((unsigned int)time(NULL));
	TCODSystem::getCurrentResolution(&ConsoleWidth, &ConsoleHeight);
	// ConsoleWidth = 1024;
	// ConsoleHeight = 768;
	ConsoleWidth /= FontX;
	ConsoleHeight /= FontY;
#ifdef DEBUG
	ConsoleHeight -= 10;
#endif // DEBUG
	//UpperBoxSiz  = (SIZES){ConsoleWidth*634/1280,ConsoleHeight*174/720};
	UpperBoxSiz = SIZES{ ConsoleWidth,ConsoleHeight * 174 / 720 };
	//SideBoxSiz   = (SIZES){ConsoleWidth*316/1280,ConsoleHeight*355/720};
	SideBoxSiz = SIZES{ ConsoleWidth * 316 / 1280,ConsoleHeight };
	//BottomBoxSiz = (SIZES){ConsoleWidth*634/1280,ConsoleHeight*180/720};
	BottomBoxSiz = SIZES{ ConsoleWidth,ConsoleHeight * 180 / 720 };
	MiddleBoxSiz = SIZES{ ConsoleWidth * 628 / 1280,ConsoleHeight * 354 / 720 };
	UpperBoxPos = SIZES{ (ConsoleWidth - UpperBoxSiz.X) / 2,0 };
	SideBoxPosLeft = SIZES{ 0,(ConsoleHeight - SideBoxSiz.Y) / 2 };
	SideBoxPosRight = SIZES{ ConsoleWidth - SideBoxSiz.X,(ConsoleHeight - SideBoxSiz.Y) / 2 };
	BottomBoxPos = SIZES{ (ConsoleWidth - BottomBoxSiz.X) / 2,ConsoleHeight - BottomBoxSiz.Y };
	MiddleBoxPos = SIZES{ (ConsoleWidth - MiddleBoxSiz.X) / 2,(ConsoleHeight - MiddleBoxSiz.Y) / 2 };
	TCODConsole::initRoot(ConsoleWidth, ConsoleHeight, "TPG");
	TCODSystem::setFps(FPS);
#ifndef DEBUG
	TCODConsole::setFullscreen(true);
#endif // DEBUG
	try {
		LoadingScreen();
		BeolvasEnemyk();
		level asd(17, 17, 8, 16, 80); /* A pálya inicializálása */
		chest Chest; /* Chest inicializálás */
		asd.engine(); /* A fõ loop */
	}
	catch (const exception& ex) { FatalError(ex.what()); }
	Exitting();
	return 0;
}

void BeolvasEnemyk() {
	char c;
	ADIR defdir = ANONE;
	ADIR atcdir = ANONE;
	string dir = "";
	ifstream in("dolgok/enemy.txt");
	if (in.fail()) FatalError("dolgok/enemy.txt was not found!");
	while (in.good()) {
		string name = "";
		getline(in, name, ',');
		int defense;
		in >> defense;
		in.get(c); //discard ,
		int offense;
		in >> offense;
		in.get(c); //discard ,
		getline(in, dir, ','); //DEF
		if (dir == "LEFT") defdir = ALEFT;
		else if (dir == "MID")  defdir = AMID;
		else if (dir == "UP")  defdir = AUP;
		else if (dir == "RIGHT")defdir = ARIGHT;
		else if (dir == "DOWN") defdir = ADOWN;
		else if (dir == "NONE") defdir = ANONE;
		getline(in, dir, '\n'); //ATTCK
		if (dir == "LEFT") atcdir = ALEFT;
		else if (dir == "MID")  atcdir = AMID;
		else if (dir == "UP")  atcdir = AUP;
		else if (dir == "RIGHT")atcdir = ARIGHT;
		else if (dir == "DOWN") atcdir = ADOWN;
		else if (dir == "NONE") atcdir = ANONE;
		enemy en(name, defense, offense, defdir, atcdir);
		enemies.push_back(en);
	}
}

void LoadingScreen() {
	level::Pic2ASCII("Editing/loading", { 0, 0 }, { ConsoleWidth, ConsoleHeight });
	TCODConsole::root->flush();
}