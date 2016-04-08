#ifndef ITEMEK_H_INCLUDED
#define ITEMEK_H_INCLUDED

#include <string>
#include <vector>

class item {
public:
		std::string name; //item neve
		std::string material; //item anyaga, pl fa, fém etc., fegyver és shield mindenképp, talán armornál is
		std::string wtype; //weapon type, névhez is kell, meg speciális effektekhez, támadáshoz
		std::string stype; //shield type, névhez is kell, meg speciális effektekhez, támadáshoz, parry, normál és nagy shieldek vannak
		double dmgx; //dmg multiplier, sebzés szorzó, fegyver+shield egyaránt
		double defx; //armornál védelmi szorzó
		int stcons; //stamina consumption, stamina fogyasztás ütésenként, fegyver
		int defstam; //stamina consumption, stamina fogyasztás védekezésenként, shield
		int durability; //egyértelmű, mindennek van
		int durdmg; //fegyvernél sebzés a durabilityre
		bool broken;
		item();
		void ures();
		bool IsBroken(int asd);
};

class weapon:public item {
public:
	weapon();
	std::vector<item> beolvas();
	int getWeaponStaminaConsumption() { return stcons; };
	int getDamageX() { return dmgx; };
	void SubWeaponDur(int subdur);
};

class shield:public item {
public:
	shield();
	int getShieldStaminaConsumption() { return defstam; };
	void SubShieldDur(int subdur);
};


class armor:public item {
public:
	armor();
};

class headpiece:public armor {
public:
	headpiece();
	void SubHeadpieceDur(int subdur);
};

class chestpiece:public armor {
public:
	chestpiece();
	void SubArmorDur(int subdur);
};

class gloves:public armor {
public:
	gloves();
	void SubGlovesDur(int subdur);
};

class pants:public armor {
public:
	pants();
	void SubPantsDur(int subdur);
};

class legs:public armor {
public:
	legs();
	void SubLegsDur(int subdur);
};

//---------------------ÜRES ITEMEK--------------------
class emptyweapon :public weapon {
public:
	emptyweapon();
};
class emptyshield :public shield {
public:
	emptyshield();
};

class emptyheadpiece :public headpiece {
public:
	emptyheadpiece();
};

class emptychestpiece :public chestpiece {
public:
	emptychestpiece();
};

class emptygloves :public gloves {
public:
	emptygloves();
};

class emptypants :public pants {
public:
	emptypants();
};

class emptylegs :public legs {
public:
	emptylegs();
};

///** @brief enum kinek lehet ilyen iteme */
////typedef enum {CHEST,ENEMY,PLAYER} ITEM_OWNER;
///** @brief enum az item típusoknak */
//typedef enum {CONSUMABLE,RING,NECKLACE,HELMET,ARMOR,GLOVES,PANTS,BOOTS,WEAPON,UNKNOWN} ITEM_TYPE;
//
///** @brief struktúra az itemeknek */
//typedef struct {
//    std::string name; /**< Az item neve */
//    //ITEM_OWNER owner; /**< Az item gazdája */
//    std::string type;   /**< Az item típusa */
//    std::string pickupDescr; /**< Felvételkor az item leírás */
//    std::string dmgType;   /**< Helpben az item leírása */
//    int multiplier;
//} ITEMS;
//
///** @brief chest class beolvassa az adatokat és vektorban tárolja õket */
//class chest {
//private:
//    std::vector<ITEMS> beolvas(); /**< Itemek beolvasása */
//    //ITEM_TYPE type_id(std::string input); /**< Itemek beazonosítása */
//protected:
//    std::vector<ITEMS> items; /**< Összes itemek a fájlból */
//public:
//    chest();  /**< Konstruktor */
//    ~chest(); /**< Destruktor */
//};
//
#endif // ITEMEK_H_INCLUDED