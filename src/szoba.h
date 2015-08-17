/**
 * @file szoba.h
 * @brief fõ header fileok
 */
#ifndef SZOBA_H_INCLUDED
#define SZOBA_H_INCLUDED

#include <vector>
#include <string>

/** @brief enum az iránynak */
typedef enum {LEFT=3,UP=0,RIGHT=1,DOWN=2} IRANY;
/** @brief flagek arra hogy a szobából merrre nyílik ajtó */
typedef enum {DOOR_NONE=0b0000,DOOR_LEFT=0b1000,DOOR_UP=0b0100,DOOR_RIGHT=0b0010,DOOR_DOWN=0b0001} DOOR_DIRECTION;
/** @brief enum a mob típusoknak */
typedef enum {HOSTILE,NEUTRAL} MOB_TYPE;
/** @brief szoba class beolvassa az adatokat és vektorban tárolja õket */
class szoba {
private: //Függvények
    std::vector<std::string> padlo();   /**< padlo beolvasó függvény*/
    std::vector<std::string> faltolt(); /**< fal beolvasó függvény */
    std::vector<std::string> plafon();  /**< plafon beolvasó függvény */
    void write4real(IRANY irany);
protected:  //azert nem private, hogy orokolheto legyen
public:
    std::vector<std::string> f;  /**< floor, azaz padlo, a szoba közepe */
    std::vector<std::string> west;  /**< west wall, baloldali fal */
    std::vector<std::string> east;  /**< east wall, jobboldali fal */
    std::vector<std::string> north; /**< north wall, szemben lévõ fal */
    std::vector<std::string> south; /**< south wall, hátul lévõ fal */
    std::vector<std::string> c;     /**< ceiling, azaz plafon */
    szoba(DOOR_DIRECTION direction);  /**< Konstruktor */
    ~szoba(); /**< Destruktor */


    //void print_f (std::ostream &out);
    void writeout(IRANY irany); /**< Vektor kiírató függvény cout-ra, [in] irany */
};

#endif // SZOBA_H_INCLUDED
