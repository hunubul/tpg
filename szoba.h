#ifndef SZOBA_H_INCLUDED
#define SZOBA_H_INCLUDED
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <limits.h>
class szoba
{
protected:  //azert nem private, hogy orokolheto legyen
//public:
    std::vector <std::string> f;  //floor, azaz padlo, a szoba közepe
    std::vector <std::string> w2; //wall, szembe fal
    std::vector <std::string> w3; //wall, baloldali fal
    std::vector <std::string> w4; //wall, jobboldali fal
    std::vector <std::string> c;  //ceiling, azaz plafon
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
