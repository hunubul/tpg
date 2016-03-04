#include <iostream>
#include "SelectionMenu.h"

using namespace std;

int SelectionMenu::SelectionMenuOut(vector<string> options)
{
    short N=options.size();
    short j = 0;
    bool returnpressed = false;
    for (short i=0; i<N; i++)
    {
        //if (i==j)
            //cout << "->" << options[i] << endl;
        //else
            //cout << "  " << options[i] << endl;
    }
    while (!returnpressed)
    {
        if(GetAsyncKeyState(VK_UP) & 1)
        {
            ClearOut(j);
            j--;
            if(0>j)
                j=N-1;
            WriteOut(j);
        }
        else if(GetAsyncKeyState(VK_DOWN) & 1)
        {
            ClearOut(j);
            j++;
            if(N-1<j)
                j=0;
            WriteOut(j);
        }
        else if(GetAsyncKeyState(VK_RETURN) & 1)
            returnpressed=true;
    }
    return j;
}
void SelectionMenu::WriteOut(short j)
{
    COORD coo = {0,j};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coo);
    cout << "->";
}
void SelectionMenu::ClearOut(short j)
{
    COORD coo = {0,j};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coo);
    cout << "  ";
}
