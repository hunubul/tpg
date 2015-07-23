#ifndef SELECTIONMENU_H_INCLUDED
#define SELECTIONMENU_H_INCLUDED
#include <stdio.h>
#include <iostream>
#include <vector>
#include <windows.h>
class SelectionMenu
{
    void WriteOut(short j);
    void ClearOut(short j);
public:
    int SelectionMenuOut(std::vector<std::string> options);
};

#endif // SELECTIONMENU_H_INCLUDED
