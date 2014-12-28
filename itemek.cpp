#include "main.h"
using namespace std;

/*--------------Konstruktorok kezdete--------------*/
chest::chest() {
    items=beolvas();
    /*vector<ITEMS>::iterator it;
    for(it=items.begin(); it!=items.end(); it++)
    {
        cout << "Name: ";
        cout << (*it).name << endl;
        cout << "\tType: ";
        cout << (*it).type << endl;
        cout << "\tPickupDescr: ";
        cout << (*it).pickupDescr << endl;
        cout << "\tHelpDescr: ";
        cout << (*it).helpDescr << endl;
        cout << "\tValue: ";
        cout << (*it).value << endl;
    }*/
}
chest::~chest() {

}
/*--------------Konstruktorok vege--------------*/
/*--------------beolvas fgv-ek--------------*/
vector<ITEMS> chest::beolvas() {
    ifstream chStream ("dolgok/chest_items.txt");
    vector<ITEMS> itemek;
    ITEMS temp_items;
    int state = 0;
    while (chStream.good()) {
        string temp="";
        chStream>>ws; //Összes WhiteSpace karaktert elvet az elsõ nem WS karakterig
        getline(chStream,temp,','); //mindenhova getline kell valszleg
        while(temp[temp.length()-1]=='\t') /*Remove tabs*/ temp.erase(temp.length()-1);
        state++;
        if(state == 1) {
            temp_items.name = temp;
            temp_items.type = UNKNOWN;
            temp_items.pickupDescr = "";
            temp_items.helpDescr = "";
            temp_items.value = 0;
        }
        if(state == 2) temp_items.type = temp;
        if(state == 3) temp_items.pickupDescr = temp;
        if(state == 4) temp_items.helpDescr = temp;
        if(state == 5) {
            temp_items.value = atoi(temp.c_str());
            itemek.push_back(temp_items);
            state = 0;
        }
    }
    return itemek;
}
/*ITEM_TYPE chest::type_id(string input)
{
    if(input=="consumables")return CONSUMABLE;
    if(input=="ring")       return RING;
    if(input=="necklace")   return NECKLACE;
    if(input=="helmet")     return HELMET;
    if(input=="armor")      return ARMOR;
    if(input=="gloves")     return GLOVES;
    if(input=="pants")      return PANTS;
    if(input=="boots")      return BOOTS;
    if(input=="weapon")     return WEAPON;
    return UNKNOWN;
}*/
/*--------------beolvas fgv-ek vege--------------*/
