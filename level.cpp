#include "main.h"

using namespace std;
int MaxRoomX,MaxRoomY;

/**
 * @brief level konstruktor
 * @param Max_X Az egész szoba világ szélessége
 * @param Max_Y Az egész szoba világ magassága
 * @param MidX Ettõl az X koordinátától kezdi a szobák létrehozását
 * @param MidY Ettõl az Y koordinátától kezdi a szobák létrehozását
 * @param MaxCounter Összesen mennyi szobát szeretnénk
 */
level::level(int Max_X,int Max_Y,int MidX,int MidY,int MaxCounter) : MidX(MidX),MidY(MidY),MaxCounter(MaxCounter) {
    MaxRoomX = Max_X;
    MaxRoomY = Max_Y;
    posX = MidX;
    posY = MidY;
    bool allow; //Ez azt csekkolja, hogy folyosók jöjjenek létre
    int counter,rnd;
    for(int i=0; i<MaxRoomY; i++) {
        vector<int> temp;
        for(int j=0; j<MaxRoomX; j++) {
            temp.push_back(0);
        }
        Map.push_back(temp);
    }
    counter=2;
    Map[posX][posY]=1;
    Map[posX][posY+1]=1;
    //Ez úgy mûködik, hogy az eddig átjárható szobákból próbál új szobákat létrehozni
    while(counter!=MaxCounter) {
        allow=false;
        rnd=rand()%4;
        switch(rnd) {
        case 0: //Left
            if(posX!=0) {
                posX--;
                if(posY-1>=0&&posY+1<MaxRoomY&&Map[posX][posY-1]==0&&Map[posX][posY+1]==0) allow=true;
                else if(Map[posX][posY]==0) posX++; //Ha nem megfelelõ a feltétel akkor visszamegyünk
            }
            break;
        case 1: //Up
            if(posY!=0) {
                posY--;
                if(posX-1>=0&&posX+1<MaxRoomX&&Map[posX-1][posY]==0&&Map[posX+1][posY]==0) allow=true;
                else if(Map[posX][posY]==0) posY++;
            }
            break;
        case 2: //Right
            if(posX+1!=MaxRoomX) {
                posX++;
                if(posY-1>=0&&posY+1<MaxRoomY&&Map[posX][posY-1]==0&&Map[posX][posY+1]==0) allow=true;
                else if(Map[posX][posY]==0) posX--;
            }
            break;
        case 3: //Down
            if(posY+1!=MaxRoomY) {
                posY++;
                if(posX-1>=0&&posX+1<MaxRoomX&&Map[posX-1][posY]==0&&Map[posX+1][posY]==0) allow=true;
                else if(Map[posX][posY]==0) posY--;
            }
            break;
        }
        if(allow&&Map[posX][posY]==0) {
            Map[posX][posY]=1;
            counter++;
        }
    }
    int direction; //Flagek használatával, a szoba kostruktorában meghatározhatjuk hogy melyik falra kell ajtó
    for (int i=0; i<MaxRoomX; i++) { //tényleges, követhetõ térkép csinálása
        vector<szoba> temp;
        for (int j=0; j<MaxRoomY; j++) {
            direction=DOOR_NONE;
            if(i-1>=0      &&Map[i-1][j]==1) direction|=DOOR_LEFT;
            if(j-1>=0      &&Map[i][j-1]==1) direction|=DOOR_UP;
            if(i+1<MaxRoomX&&Map[i+1][j]==1) direction|=DOOR_RIGHT;
            if(j+1<MaxRoomY&&Map[i][j+1]==1) direction|=DOOR_DOWN;
            szoba Szoba((DOOR_DIRECTION)direction);
            temp.push_back(Szoba);
        }
        terkep.push_back(temp);
    }
}
/**
 * @brief a fõ loop, nyíl input kezelés
 */
void level::engine() {
    posX=MidX;
    posY=MidY;
    most=UP;
    int lehet;
    TCOD_key_t Choice;
    do {
        writeout(posX,posY);
        Choice = TCODConsole::root->waitForKeypress(true);
        switch(Choice.vk) { //Irány változó típusból adódik
        case TCODK_LEFT:
            lehet=(most+3)%4;
            merre((IRANY)lehet);
            break;
        case TCODK_UP:
            lehet=(most+0)%4;
            merre((IRANY)lehet);
            break;
        case TCODK_RIGHT:
            lehet=(most+1)%4;
            merre((IRANY)lehet);
            break;
        case TCODK_DOWN:
            lehet=(most+2)%4;
            merre((IRANY)lehet);
            break;
        default:
            break;
        }
    } while(!TCODConsole::isWindowClosed()&&Choice.vk!=TCODK_ESCAPE);
}
void level::merre(IRANY honnan) {
    switch(honnan) {
    case UP:
        if(posY!=0&&Map[posX][posY-1]==1) {
            posY--;
            most=UP;
        }
        break;
    case RIGHT:
        if(posX+1!=MaxRoomX&&Map[posX+1][posY]==1) {
            posX++;
            most=RIGHT;
        }
        break;
    case DOWN:
        if(posY+1!=MaxRoomY&&Map[posX][posY+1]==1) {
            posY++;
            most=DOWN;
        }
        break;
    case LEFT:
        if(posX!=0&&Map[posX-1][posY]==1) {
            posX--;
            most=LEFT;
        }
        break;
    }
}
/**
 * @brief kiírja az egész térképet, + az aktív szoba tartalmát
 * @param posX jelenleg hol állunk (X koord)
 * @param posY jelenleg hol állunk (Y koord)
 */
void level::writeout(int posX,int posY) {
    TCODConsole::root->clear();
    for (int i=0; i<MaxRoomX; i++) {
        for (int j=0; j<MaxRoomY; j++) {
            writearrow();
            if(posX==i&&posY==j) TCODConsole::root->setDefaultForeground(TCODColor::red);
            TCODConsole::root->print(i,j,"%d",Map[i][j]);
            TCODConsole::root->setDefaultForeground(TCODColor::white);
        }
    }
    terkep[posX][posY].writeout((IRANY)most);
    TCODConsole::root->flush();
}
void level::writearrow() {
    switch(most) {
    case UP:
        TCODConsole::root->print(MaxRoomX+3,0,"/\\ ");
        TCODConsole::root->print(MaxRoomX+3,1,"|| ");
        break;
    case RIGHT:
        TCODConsole::root->print(MaxRoomX+3,0,"-->");
        TCODConsole::root->print(MaxRoomX+3,1,"  ");
        break;
    case DOWN:
        TCODConsole::root->print(MaxRoomX+3,0,"|| ");
        TCODConsole::root->print(MaxRoomX+3,1,"\\/");
        break;
    case LEFT:
        TCODConsole::root->print(MaxRoomX+3,0,"<--");
        TCODConsole::root->print(MaxRoomX+3,1,"  ");
        break;
    }
}
