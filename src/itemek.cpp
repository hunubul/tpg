#include <fstream>
#include <stdlib.h>
#include "itemek.h"
#include "globals.h"

using namespace std;


item::item(){}
armor::armor(){}
//------------------fegyverek generálása
weapon::weapon() {
	vector<item> all = beolvas();
	item gen;
	vector<string> temp;
	temp = globals::split(gen.name, ' ');
	int asd;
	asd = rand() % all.size();
	gen = all[asd];
	wtype = gen.wtype;
	stcons = gen.stcons;
	durability = gen.durability;
	durdmg = gen.durdmg;
	asd = rand() % 2;
	if (asd == 0) { material = gen.material; durability += rand() % 100; durdmg += rand() % 10; }
	else material = "iron"; durability += (rand() % 200) + 100; durdmg += (rand() % 20) + 10;
	int harmad;
	if (material == "iron") { asd = (rand() % 16) + 20; dmgx = asd / 10; harmad = (asd - 20) / 3; }
	else asd = (rand() % 16) + 10; dmgx = asd / 10; harmad = (asd - 10) / 3;
	switch (harmad){
	case 0:
		name = temp[1] + " " + material + " " + wtype;
		break;
	case 1:
		name = material + " " + wtype;
		break;
	case 2:
		name = temp[2] + " " + material + " " + wtype;
		break;
	}

}
//------------------fegyverek beolvasása
vector<item> weapon::beolvas() {
	ifstream wStream("dolgok/weapon.txt");
	vector<item> out;
	while (wStream.good()) {
		string stemp;
		weapon wtemp;
		vector<string> vtemp;
		getline(wStream,stemp,'\n');
		vtemp = globals::split(stemp, ',');
		wtemp.wtype = vtemp[0];
		wtemp.name = vtemp[1];
		wtemp.material = vtemp[2];
		wtemp.stcons = stoi(vtemp[3]);
		wtemp.durdmg = stoi(vtemp[4]);
		wtemp.durability = stoi(vtemp[5]);
		out.push_back(wtemp);
	}
	return out;
}
shield::shield() {
}
headpiece::headpiece() {


}
chestpiece::chestpiece() {


}
gloves::gloves() {


}
pants::pants() {


}
boots::boots() {


}
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