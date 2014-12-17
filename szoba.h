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
//public:
    std::vector <std::string> f;  //floor, azaz padlo, a szoba közepe
    std::vector <std::string> west; //wall, szembe fal
    std::vector <std::string> east; //wall, baloldali fal
    std::vector <std::string> north; //wall, jobboldali fal
    std::vector <std::string> south;  //ceiling, azaz plafon
    std::vector <std::string> c;
public:
    szoba(); //not default ctor
    
    
    void print_f (std::ostream &out);
};

#endif // SZOBA_H_INCLUDED
