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
		item();

};

class ures :public item {
public:
	ures();
};

class weapon:public item {
public:
	weapon();
	std::vector<item> beolvas();
};

class shield:public item {
public:
	shield();
};

class armor:public item {
public:
	armor();
};

class headpiece:public armor {
public:
	headpiece();
};

class chestpiece:public armor {
public:
	chestpiece();
};

class gloves:public armor {
public:
	gloves();
};

class pants:public armor {
public:
	pants();
};

class legs:public armor {
public:
	legs();
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