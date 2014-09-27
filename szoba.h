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
public: //Ideiglenes public -> 18.sor eredeti public (main.cpp->asd.f.size()-nál szükséges a publikus elérés)
    std::vector <std::string> f;
    std::vector <std::string> w2;
    std::vector <std::string> w3;
    std::vector <std::string> w4;
    std::vector <std::string> c;
//public:
    szoba();
    std::vector<std::string> padlo();
};

#endif // SZOBA_H_INCLUDED
