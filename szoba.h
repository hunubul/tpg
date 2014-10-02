#ifndef SZOBA_H_INCLUDED
#define SZOBA_H_INCLUDED
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
class szoba
{
protected:  //azert nem private, hogy orokolheto legyen
//public: //Ideiglenes public -> 18.sor eredeti public (main.cpp->asd.f.size()-nál szükséges a publikus elérés)
    std::vector <std::string> f; //floor, azaz padlo, a szoba közepe
    //eltűntek ezeknek a leírásai, hogy melyik micsoda, gondolok itt a w2-3-4,re
    //melyik a szembe, jobbra, mert ezek ki voltak írva még múltkor
    std::vector <std::string> w2;
    std::vector <std::string> w3;
    std::vector <std::string> w4;
    std::vector <std::string> c; //ceiling, azaz plafon
//public:

    std::vector<std::string> padlo();
    std::vector<std::string> fal2();
    /*std::vector<std::string> fal3();
    std::vector<std::string> fal4();
    std::vector<std::string> plafon();*/

public:
    szoba(); //not default ctor

    void print_f (std::ostream &out);
};

#endif // SZOBA_H_INCLUDED
