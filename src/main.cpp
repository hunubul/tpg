/**
 * @file main.cpp
 * @brief main függvény
 */

#include <fstream>
#include <time.h>
#include "character.h"
#include "level.h"
#include "logging.h"
#include "globals.h"
#include "openGL/initOpenGL.h"

using namespace globals;
using namespace std;

void BeolvasEnemyk();

//4:3  - 1024x768
//16:9 - 1280x720

int main(int argc, char *argv[]) {
	srand((unsigned int)time(NULL));
	initOpenGL();
	try {
		BeolvasEnemyk();
		level asd(17, 17, 8, 16, 80); /* A pálya inicializálása */
		//chest Chest; /* Chest inicializálás */
		asd.engine(); /* A fő loop */
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