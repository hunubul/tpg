/**
 * @file szoba.cpp
 * @brief fõ implementációk
 */

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "szoba.h"
#include "globals.h"

/*ELVILEG MINDEN FELÜLET MEGVAN, DE CSAK A SZEMBE MEG PADLÓ VAN BERAKVA, A BEOLVASANDÓ FÁJLOKAT MÉG BUHERÁLNI KÉNE*/

using namespace globals;
using namespace std;

/*--------------Konstruktorok kezdete--------------*/
szoba::szoba(DOOR_DIRECTION direction) {
	if (direction != DOOR_NONE) {
		int asd;
		int rarity; //Ritkasága egy itemnek
		/* A padlón levõ dolgok */
		vector<string> fv = padlo();
		asd = rand() % 2 + 1;   //hány tárgy legyen max választva
		for (int i = 0; i < asd; i++) { //sorsolás
			int bsd = rand() % fv.size();
			f.push_back(fv[bsd]);
			fv.erase(fv.begin() + bsd);
		}
		/* A bal falon levõ dolgok */
		if ((direction&DOOR_LEFT) == 0) {
			vector<string> westv = faltolt();
			asd = 1 + rand() % 4;   //hány tárgy legyen max választva, sorsolás
			while ((int)west.size() != asd) {
				int bsd = rand() % westv.size();
				vector<string> splitted = split(westv[bsd].c_str(),',');
				if (splitted.size() < 2) throw "Not enough data";
				string Name = splitted[0];
				rarity = stoi( splitted[1] );
				if (rand() % rarity < MAX_RARITY) {
					west.push_back(Name);
					westv.erase(westv.begin() + bsd);
				}
			}
		}
		else {
			west.push_back("LEFT DOOR");
		}
		/* A szembe falon levõ dolgok */
		if ((direction&DOOR_UP) == 0) {
			vector<string> northv = faltolt();
			asd = 1 + rand() % 4;   //hány tárgy legyen max választva
			for (int i = 0; i < asd; i++) { //sorsolás
				int bsd = rand() % northv.size();
				vector<string> splitted = split(northv[bsd].c_str(), ',');
				if (splitted.size() < 2) throw "Not enough data";
				string Name = splitted[0];
				rarity = stoi(splitted[1]);
				north.push_back(Name);
				northv.erase(northv.begin() + bsd);
			}
		}
		else {
			north.push_back("FRONT DOOR");
		}
		/* A jobb falon levõ dolgok */
		if ((direction&DOOR_RIGHT) == 0) {
			vector<string> eastv = faltolt();
			asd = 1 + rand() % 4;   //hány tárgy legyen max választva
			for (int i = 0; i < asd; i++) { //sorsolás
				const int bsd = rand() % eastv.size();
				vector<string> splitted = split(eastv[bsd].c_str(), ',');
				if (splitted.size() < 2) throw "Not enough data";
				string Name = splitted[0];
				rarity = stoi(splitted[1]);
				east.push_back(Name);
				eastv.erase(eastv.begin() + bsd);
			}
		}
		else {
			east.push_back("RIGHT DOOR");
		}
		/* A hátsó falon levõ dolgok */
		if ((direction&DOOR_DOWN) == 0) {
			vector<string> southv = faltolt();
			asd = 1 + rand() % 4;   //hány tárgy legyen max választva
			for (int i = 0; i < asd; i++) { //sorsolás
				const int bsd = rand() % southv.size();
				vector<string> splitted = split(southv[bsd].c_str(), ',');
				if (splitted.size() < 2) throw "Not enough data";
				string Name = splitted[0];
				rarity = stoi(splitted[1]);
				south.push_back(Name);
				southv.erase(southv.begin() + bsd);
			}
		}
		else {
			south.push_back("BACK DOOR");
		}
		/* plafonon levő dolgok */
		vector<string> cv = plafon();
		asd = rand() % 3;   //hány tárgy legyen max választva
		for (int i = 0; i < asd; i++) { //sorsolás
			int bsd = rand() % cv.size();
			c.push_back(cv[bsd]);
			cv.erase(cv.begin() + bsd);
		}
	}
}
szoba::~szoba() {

}
/*--------------Konstruktorok vege--------------*/

/*--------------beolvas fgv-ek--------------*/
vector<string> szoba::padlo() {
	ifstream floorf("dolgok/f.txt");
	vector <string> floorv;
	while (floorf.good()) {
		string temp = "";
		floorf >> ws; //Összes WhiteSpace karaktert elvet az elsõ nem WS karakterig
		getline(floorf, temp, '\n'); //mindenhova getline kell valszleg
		if (temp != "") {
			floorv.push_back(temp);
		}
	}
	return floorv;
}
vector<string> szoba::faltolt() {
	ifstream wf("dolgok/wall.txt");
	vector <string> wv;
	while (wf.good()) {
		string temp = "";
		wf >> ws;
		getline(wf, temp, '\n');
		if (temp != "") {
			wv.push_back(temp);
		}
	}
	return wv;
}
vector<string> szoba::plafon() {
	ifstream cf("dolgok/c.txt");
	vector <string> c;
	while (cf.good()) {
		string temp = "";
		cf >> ws;
		getline(cf, temp, '\n');
		if (temp != "") {
			c.push_back(temp);
		}
	}
	return c;
}
/*--------------beolvas fgv-ek vege--------------*/


/*--------------public fv-ek KEZDETE ------------*/

/** Split string by tokens
	@param s string to tokenize
	@param delim char delimeter */
vector<string> szoba::split(const string &s, char delim) {
	vector<string> elems;
	string temp;
	for each (char var in s) {
		if (var == delim) {
			elems.push_back(temp);
			temp.clear();
		} else temp.push_back(var);
	}
	if (elems.size() == 0) return vector<string>(1,"Error");
	return elems;
}

void szoba::writeout(IRANY irany) {
	//    vector<string>::iterator it;
	//    int y=1;
	//    /* A plafonon levõ dolgok */
	//    TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"Ceiling:");
	//    y+=2;
	//    for(it=c.begin(); it!=c.end(); it++)         {
	//        TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//        y++;
	//    }
	//    /* A szembe levõ dolgok */
	//    y=ConsoleHeight/2-35;
	//    TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"Front wall:");
	//    /* A baloldali falon levõ dolgok */
	//    y=ConsoleHeight/2-20;
	//    TCODConsole::root->printEx(0,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_LEFT,"Left wall:");
	//    /* A jobboldali falon levõ dolgok */
	//    y=ConsoleHeight/2-20;
	//    TCODConsole::root->printEx(ConsoleWidth-1,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_RIGHT,"Right wall:");
	//    /* A padlón levõ dolgok */
	//    y=ConsoleHeight/2+5;
	//    TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"Floor:");
	//    y+=2;
	//    for(it=f.begin(); it!=f.end(); it++)         {
	//        TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//        y++;
	//    }
	//    /* A hátul levõ dolgok */
	//    y=ConsoleHeight/2+15;
	//    TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"Behind wall:");
	//    write4real(irany);
}

void szoba::write4real(IRANY irany) {
	//    vector<string>::iterator it;
	//    int y;
	//    switch(irany) {
	//    case UP:
	//        y=ConsoleHeight/2-33; //Teteje
	//        for(it=north.begin(); it!=north.end(); it++) {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Balra
	//        for(it=west.begin(); it!=west.end(); it++)   {
	//            TCODConsole::root->printEx(4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_LEFT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2+17; //Alja
	//        for(it=south.begin(); it!=south.end(); it++) {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Jobbra
	//        for(it=east.begin(); it!=east.end(); it++)   {
	//            TCODConsole::root->printEx(ConsoleWidth-4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_RIGHT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        break;
	//    case RIGHT:
	//        y=ConsoleHeight/2-33; //Teteje
	//        for(it=east.begin(); it!=east.end(); it++) {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Balra
	//        for(it=north.begin(); it!=north.end(); it++)   {
	//            TCODConsole::root->printEx(4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_LEFT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2+17; //Alja
	//        for(it=west.begin(); it!=west.end(); it++) {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Jobbra
	//        for(it=south.begin(); it!=south.end(); it++)   {
	//            TCODConsole::root->printEx(ConsoleWidth-4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_RIGHT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        break;
	//    case DOWN:
	//        y=ConsoleHeight/2-33; //Teteje
	//        for(it=south.begin(); it!=south.end(); it++)   {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Balra
	//        for(it=east.begin(); it!=east.end(); it++) {
	//            TCODConsole::root->printEx(4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_LEFT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2+17; //Alja
	//        for(it=north.begin(); it!=north.end(); it++)   {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Jobbra
	//        for(it=west.begin(); it!=west.end(); it++) {
	//            TCODConsole::root->printEx(ConsoleWidth-4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_RIGHT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        break;
	//    case LEFT:
	//        y=ConsoleHeight/2-33; //Teteje
	//        for(it=west.begin(); it!=west.end(); it++) {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Balra
	//        for(it=south.begin(); it!=south.end(); it++)   {
	//            TCODConsole::root->printEx(4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_LEFT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2+17; //Alja
	//        for(it=east.begin(); it!=east.end(); it++) {
	//            TCODConsole::root->printEx(ConsoleWidth/2,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_CENTER,"%s",(*it).c_str());
	//            y++;
	//        }
	//        y=ConsoleHeight/2-18; //Jobbra
	//        for(it=north.begin(); it!=north.end(); it++)   {
	//            TCODConsole::root->printEx(ConsoleWidth-4,MaxRoomY+y,TCOD_BKGND_NONE,TCOD_RIGHT,"%s",(*it).c_str());
	//            y++;
	//        }
	//        break;
	//    }
}
/*--------------public fv-ek VEGE ------------*/
