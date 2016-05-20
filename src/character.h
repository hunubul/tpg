#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <vector>
#include "Image2ASCIIcolors.h"
#include "itemek.h"

typedef enum {ALEFT=4,AMID=0,AUP=1,ARIGHT=2,ADOWN=3,ANONE=-1} ADIR; //preferált támadási irány

typedef struct {
    std::string effect; //blind bleed poison satöbbi
    int roundsleft; //mennyi körig van még fent az effect
} STATUS;

typedef struct {
	weapon weapon;
	shield shield;
	headpiece headpiece;
	chestpiece chestpiece;
	gloves gloves;
	pants pants;
	legs legs;
} INVENTORY;

class character {
protected:
	int maxhp; //egyértelmű
	int hp; //jelenlegi hp
	int maxstam; //maximum stamina
	int stam; //jelenlegi stamina
	std::vector<STATUS> cstatus; //blind bleed etc ide, struktúra feljebb
public:
	std::vector<item> inventory; //táska tartalma
	INVENTORY wearing; //amit hord, ez külön kezelendő inventorytól
	int defense; //védelmi érték
	int offense; //támadó érték
	character(int maxhp, int maxstam, int defense, int offense); //konstruktor, jelenleg üres
	int getStamina() { return stam; }
	void addStamina(int pStam);
	void subStamina(int pStam);
	int getHP() { return hp; }
	void addHP(int pHp);
	bool damage(double DMG);
	double getDefValue();
	int wsc() { return wearing.weapon.getWeaponStaminaConsumption(); };
	int ssc() { return wearing.shield.getShieldStaminaConsumption(); };
	void BrokenInv();
};

class player : public character {
public:
    player();
};

class enemy : public character {
public:
    ADIR def; //preferált védekezési oldal
    ADIR atc; //preferált támadási oldal
    std::string name;
    IMAGE PNG;
    static SIZES BoxSize;
    static SIZES TopLeft;
    enemy(std::string name,int defense,int offense,ADIR def,ADIR atc);
    ~enemy();
};

#endif
