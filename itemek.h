#ifndef ITEMEK_H_INCLUDED
#define ITEMEK_H_INCLUDED

/** @brief enum kinek lehet ilyen iteme */
typedef enum {CHEST,ENEMY,PLAYER} ITEM_OWNER;
/** @brief enum az item típusoknak */
typedef enum {CONSUMABLE,RING,NECKLACE,HELMET,ARMOR,GLOVES,PANTS,BOOTS,WEAPON,UNKNOWN} ITEM_TYPE;

/** @brief struktúra az itemeknek */
typedef struct {
    std::string name; /**< Az item neve */
    //ITEM_OWNER owner; /**< Az item gazdája */
    std::string type;   /**< Az item típusa */
    std::string pickupDescr; /**< Felvételkor az item leírás */
    std::string helpDescr;   /**< Helpben az item leírása */
    int value;
} ITEMS;

/** @brief chest class beolvassa az adatokat és vektorban tárolja õket */
class chest {
private:
    std::vector<ITEMS> beolvas(); /**< Itemek beolvasása */
    //ITEM_TYPE type_id(std::string input); /**< Itemek beazonosítása */
protected:
    std::vector<ITEMS> items; /**< Összes itemek a fájlból */
public:
    chest();  /**< Konstruktor */
    ~chest(); /**< Destruktor */
};

#endif // ITEMEK_H_INCLUDED
