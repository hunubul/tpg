#include <fstream>
#include <stdlib.h>
#include "itemek.h"
#include "globals.h"

using namespace std;

item::item() { broken = false; }

bool item::IsBroken(int asd) {
	if (asd <= 0) return true;
	else return false;
}

void item::ures() {
	name = "EMPTY";
	dmgx = 1;
	defx = 1;
	stcons = 10;
	defstam = 5;
	durdmg = 0;
}
//------------------fegyverek generálása
weapon::weapon() {
	vector<item> all = beolvas();
	item gen;
	vector<string> temp;
	int asd;
	asd = rand() % all.size();
	gen = all[asd];
	temp = globals::split(gen.name, ' ');
	wtype = gen.wtype;
	stcons = gen.stcons;
	durability = gen.durability;
	durdmg = gen.durdmg;
	asd = rand() % 2;
	if (asd == 0) { material = gen.material; durability += rand() % 100; durdmg += rand() % 10; }
	else material = "iron"; durability += (rand() % 200) + 100; durdmg += (rand() % 20) + 10;
	int harmad;
	if (material == "iron") { asd = (rand() % 15) + 25; dmgx = asd / 10; harmad = (int) (asd - 25) / 5; }
	else asd = (rand() % 15) + 15; dmgx = asd / 10; harmad = (int) (asd - 15) / 5;
	switch (harmad){
	case 0:
		name = temp[0] + " " + material + " " + wtype;
		temp[0] += " ";
		break;
	case 1:
		name = material + " " + wtype;
		temp[0] = "";
		break;
	case 2:
		name = temp[1] + " " + material + " " + wtype;
		temp[0] = temp[1] + " ";
		break;
	}
	asd = rand() % 100;
	if (asd == 99) {
		material = "steel";
		name = temp[0] + material + " " + wtype;
		dmgx = dmgx*1.1;
		durability = (int)(durability*1.3);
		durdmg = (int)(durdmg*1.2);
	}
}


//------------------fegyverek beolvasása
//fegyver típus
//legrosszabb és legjobb állapot
//ha gagyi, akkor miből van
//mennyi staminát fogyaszt 1 ütés
//mennyit sebez páncél durabilityjére, shield, vagy shield hiányában a fegyver durabilitijére sikeres védés esetén
//durability, sikeres védéskor megy le, talán ütésenként is sebződjön ez az érték, de szerintem ne
vector<item> weapon::beolvas() {
	ifstream wStream("dolgok/weapon.txt");
	vector<item> out;
	while (wStream.good()) {
		string stemp="";
		wStream >> ws;
		item wtemp;
		vector<string> vtemp;
		getline(wStream,stemp,'\n');
		if (stemp != "") {
			vtemp = globals::split(stemp, ',');
			wtemp.wtype = vtemp[0];
			wtemp.name = vtemp[1];
			wtemp.material = vtemp[2];
			wtemp.stcons = stoi(vtemp[3]);
			wtemp.durdmg = stoi(vtemp[4]);
			wtemp.durability = stoi(vtemp[5]);
			out.push_back(wtemp);
		}
	}
	return out;
}
//------------------shieldek generálása, nincs beolvasás hozzá
//az stcons itt sikeres védésnél használt staminát jelent
shield::shield() {
	int asd = rand() % 3;
	switch (asd)
	{
	case 0:
		stype = "parry";
		name = "parry shield";
		dmgx = ((rand()%11)+90)/100;
		defstam = 5;
		durability = 80;
		break;
	case 1:
		stype = "shield";
		name = stype;
		dmgx = ((rand() % 21) + 85) / 100;
		defstam = 10;
		durability = 160;
		break;
	case 2:
		stype = "tower";
		name = "tower shield";
		dmgx = ((rand() % 21) + 60) / 100;
		defstam = 1;
		durability = 320;
		break;
	}
	asd = rand() % 3;
	switch (asd)
	{
	case 0:
		material = "wooden";
		name = material + " " + name;
		break;
	case 1:
		material = "iron";
		name = material + " " + name;
		dmgx = dmgx*0.9;
		defstam += 3;
		durability = (durability*1.25);
		break;
	case 2:
		material = "steel";
		name = material + " " + name;
		dmgx = dmgx*0.9;
		defstam += 5;
		durability = (durability*1.5);
		break;
	}
	durability += (rand() % 5) * (rand() % 10);
}

armor::armor() {
	
}
headpiece::headpiece() {
	int asd = rand() % 3;
	durability = 100;
	switch (asd)
	{
	case 0:
		name = "Leather cap";
		defx = 1.1;
		break;
	case 1:
		name = "Mail hood";
		durability = (durability*1.75);
		defx = 1.3;
		break;
	case 2:
		name = "Steel helmet";
		durability = (durability*2.25);
		defx = 1.5;
		break;
	}
	durability += (rand() % 5) * (rand() % 10);
}
chestpiece::chestpiece() {
	int asd = rand() % 3;
	durability = 180;
	switch (asd)
	{
	case 0:
		name = "Leather armor";
		defx = 1.2;
		break;
	case 1:
		name = "Chainmail";
		durability = (durability*1.75);
		defx = 1.5;
		break;
	case 2:
		name = "Steel plate armor";
		durability = (durability*2.25);
		defx = 1.75;
		break;
	}
	durability += (rand() % 5) * (rand() % 10);
}
gloves::gloves() {
	int asd = rand() % 3;
	durability = 120;
	switch (asd)
	{
	case 0:
		name = "Leather gloves";
		defx = 1.1;
		break;
	case 1:
		name = "Chain gloves";
		durability = (durability*1.5);
		defx = 1.25;
		break;
	case 2:
		name = "Steel gauntlets";
		durability = (durability*1.75);
		defx = 1.4;
		break;
	}
	durability += (rand() % 5) * (rand() % 10);
}
pants::pants() {
	int asd = rand() % 3;
	durability = 180;
	switch (asd)
	{
	case 0:
		name = "Leather pants";
		defx = 1.15;
		break;
	case 1:
		name = "Chain leggings";
		durability = (durability*1.25);
		defx = 1.35;
		break;
	case 2:
		name = "Steel plate leggings";
		durability = (durability*1.5);
		defx = 1.6;
		break;
	}
	durability += (rand() % 5) * (rand() % 10);

}
legs::legs() {
	int asd = rand() % 3;
	durability = 150;
	switch (asd)
	{
	case 0:
		name = "Leather boots";
		defx = 1.05;
		break;
	case 1:
		name = "Chain boots";
		durability = (durability*1.5);
		defx = 1.15;
		break;
	case 2:
		name = "Steel boots";
		durability = (durability*1.75);
		defx = 1.25;
		break;
	}
	durability += (rand() % 5) * (rand() % 10);
}
//------------------DurDmg függvények-----------------
void weapon::SubWeaponDur(int SubDur) {
	durability -= SubDur;
	IsBroken(durability);
}
void shield::SubShieldDur(int SubDur) {
	durability -= SubDur;
	IsBroken(durability);
}
void headpiece::SubHeadpieceDur(int SubDur) {
	durability -= SubDur;
	IsBroken(durability);
}
void chestpiece::SubArmorDur(int SubDur) {
	durability -= SubDur;
	IsBroken(durability);
}
void gloves::SubGlovesDur(int SubDur) {
	durability -= SubDur;
	IsBroken(durability);
}
void pants::SubPantsDur(int SubDur) {
	durability -= SubDur;
	IsBroken(durability);
}
void legs::SubLegsDur(int SubDur) {
	durability -= SubDur;
	IsBroken(durability);
}
//---------------------ÜRES ITEMEK--------------------
emptyweapon::emptyweapon() {ures();}
emptyshield::emptyshield() {ures();}
emptyheadpiece::emptyheadpiece() {ures();}
emptychestpiece::emptychestpiece(){ures();}
emptygloves::emptygloves() {ures();}
emptypants::emptypants() {ures();}
emptylegs::emptylegs() {ures();}
///*--------------Konstruktorok kezdete--------------*/
//chest::chest() {
//    items=beolvas();
//    /*vector<ITEMS>::iterator it;
//    for(it=items.begin(); it!=items.end(); it++)
//    {
//        cout << "Name: ";
//        cout << (*it).name << endl;
//        cout << "\tType: ";
//        cout << (*it).type << endl;
//        cout << "\tPickupDescr: ";
//        cout << (*it).pickupDescr << endl;
//        cout << "\tHelpDescr: ";
//        cout << (*it).helpDescr << endl;
//        cout << "\tValue: ";
//        cout << (*it).value << endl;
//    }*/
//}
//chest::~chest() {
//
//}
///*--------------Konstruktorok vege--------------*/
///*--------------beolvas fgv-ek--------------*/
//vector<ITEMS> chest::beolvas() {
//    ifstream chStream ("dolgok/chest_items.txt");
//    vector<ITEMS> itemek;
//    ITEMS temp_items;
//    int pos = 0;
//    while (chStream.good()) {
//        string temp="";
//        chStream>>ws; //Összes WhiteSpace karaktert elvet az elsõ nem WS karakterig
//        getline(chStream,temp,','); //mindenhova getline kell valszleg
//        while(temp.length() >= 1&&temp[temp.length()-1]=='\t') /*Remove tabs*/ temp.erase(temp.length()-1);
//        pos++;
//        if(pos == 1) {
//            temp_items.name = temp;
//            temp_items.type = UNKNOWN;
//            temp_items.pickupDescr = "";
//            temp_items.dmgType = "";
//            temp_items.multiplier = 0;
//        }
//        else if(pos == 2) temp_items.type = temp;
//        else if(pos == 3) temp_items.pickupDescr = temp;
//        else if(pos == 4) temp_items.dmgType = temp;
//        else if(pos == 5) {
//            temp_items.multiplier = atoi(temp.c_str());
//            itemek.push_back(temp_items);
//            pos = 0;
//        }
//    }
//    return itemek;
//}
///*ITEM_TYPE chest::type_id(string input)
//{
//    if(input=="consumables")return CONSUMABLE;
//    if(input=="ring")       return RING;
//    if(input=="necklace")   return NECKLACE;
//    if(input=="helmet")     return HELMET;
//    if(input=="armor")      return ARMOR;
//    if(input=="gloves")     return GLOVES;
//    if(input=="pants")      return PANTS;
//    if(input=="boots")      return BOOTS;
//    if(input=="weapon")     return WEAPON;
//    return UNKNOWN;
//}*/
///*--------------beolvas fgv-ek vege--------------*/