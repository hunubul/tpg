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
        FOV.push_back(temp);
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
    RoomWriteout();
    WriteOutBoxes();
    for (int i=0; i<MaxRoomX; i++) {
        for (int j=0; j<MaxRoomY; j++) {
            if(posX==i&&posY==j) {
                FOV[i][j]=1;
                TCODConsole::root->setDefaultForeground(TCODColor::red);
            }
#ifdef DEBUG
            writearrow();
            if(Map[i][j]==1) TCODConsole::root->print(i,j,"#");
            else TCODConsole::root->print(i,j," ");
#else
            if(FOV[i][j]==1) {
                TCODConsole::root->print(i,j,"#");
                TCODConsole::root->setDefaultForeground(TCODColor::white);
                if(i-1>=0      &&Map[i-1][j]==1&&FOV[i-1][j]!=1) TCODConsole::root->print(i-1,j  ,"?");
                if(j-1>=0      &&Map[i][j-1]==1&&FOV[i][j-1]!=1) TCODConsole::root->print(i  ,j-1,"?");
                if(i+1<MaxRoomX&&Map[i+1][j]==1&&FOV[i+1][j]!=1) TCODConsole::root->print(i+1,j  ,"?");
                if(j+1<MaxRoomY&&Map[i][j+1]==1&&FOV[i][j+1]!=1) TCODConsole::root->print(i  ,j+1,"?");
            } else if(TCODConsole::root->getChar(i,j)!='?') TCODConsole::root->print(i,j," ");
#endif // DEBUG
            TCODConsole::root->setDefaultForeground(TCODColor::white);
        }
    }
    //terkep[posX][posY].writeout((IRANY)most);
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
void level::RoomWriteout() {
    CONSOLEINFO Con;
    IMAGE PNG;
    SUBSECTION subsec;
    CHAR_SET CharSet;
    /*-----------Initializing CONSOLEINFO-----------*/
    Con.FontSize.X = FontX;
    Con.FontSize.Y = FontY;
    Con.CharAmount.X = ConsoleWidth;
    Con.CharAmount.Y = ConsoleHeight;
    Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
    Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
    /*----------------------------------------------*/
    CharSetImporter(&CharSet,"8x14asciichars.dat");
    CalculateWeights(&CharSet); /* Calculating charset weights... */
    PNG.ASCII_Color = NULL;
    lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, "images/Room.png");
    PNG.HeightTile = Con.CharAmount.Y;
    PNG.WidthTile = Con.CharAmount.X;
    subsec.height=(double)PNG.Height/PNG.HeightTile; /* Calculating SUBSECTION in pixels */
    subsec.width =(double)PNG.Width/PNG.WidthTile; /* Calculating SUBSECTION in pixels */
    ProcessPNG(&PNG,subsec); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
    IMAGE2ASCII(&PNG,CharSet);
    WriteOut(PNG,false,"");
    free(PNG.Image);
    free(PNG.ASCII_Image);
    free(PNG.Weight);
}
void level::WriteOutBoxes() {
    SIZES TopLeft;
    SIZES BoxSize;
    //*** Ceiling ***
    if(szoba::c.size() == 1) {
        TopLeft.X = ConsoleWidth*0.25;
        TopLeft.Y = 0;
        BoxSize.X = ConsoleWidth*0.5;
        BoxSize.Y = ConsoleHeight*0.24;

    } else if(szoba::c.size() == 2) {
        //Elso dolog
        TopLeft.X = ConsoleWidth*0.25;
        TopLeft.Y = 0;
        BoxSize.X = (ConsoleWidth*0.5)/2;
        BoxSize.Y = ConsoleHeight*0.24;

        //Masodik dolog
        TopLeft.X = ConsoleWidth*0.25+(ConsoleWidth*0.5)/2;
        TopLeft.Y = 0;
        BoxSize.X = (ConsoleWidth*0.5)/2;
        BoxSize.Y = ConsoleHeight*0.24;

    }
}
void level::Pic2ASCII(string PicName,SIZES TopLeft,SIZES BoxSize) {
    CONSOLEINFO Con;
    IMAGE PNG;
    SUBSECTION subsec;
    CHAR_SET CharSet;
    string PicPath = "images/"+PicName;
    /*-----------Initializing CONSOLEINFO-----------*/
    Con.FontSize.X = FontX;
    Con.FontSize.Y = FontY;
    Con.CharAmount.X = BoxSize.X;
    Con.CharAmount.Y = BoxSize.Y;
    Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
    Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
    /*----------------------------------------------*/
    CharSetImporter(&CharSet,"8x14asciichars.dat");
    CalculateWeights(&CharSet); /* Calculating charset weights... */
    PNG.ASCII_Color = NULL;
    lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, PicPath.c_str());
    CalculatePNGSizes(&PNG,&subsec,Con);
    ProcessPNG(&PNG,subsec); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
    IMAGE2ASCII(&PNG,CharSet);
    WriteOutPic(PNG, TopLeft, BoxSize);
    free(PNG.Image);
    free(PNG.ASCII_Image);
    free(PNG.Weight);
}
void level::WriteOutPic(IMAGE PNG,SIZES TopLeft,SIZES BoxSize) {
    unsigned i,j;
    unsigned VerLeftover = (BoxSize.Y-PNG.HeightTile)/2; //Függőleges maradék amennyit fent és lent kihagy
    unsigned HorLeftover = (BoxSize.X-PNG.WidthTile)/2; //Vízszintes maradék amennyit balra és jobbra kihagy
    for (i=0; i<(unsigned)BoxSize.Y; i++) {
        for (j=0; j<(unsigned)BoxSize.X; j++) {
            TCODConsole::root->print(TopLeft.X+j,TopLeft.Y+i," ");
        }
    }
    for (i=0; i<PNG.HeightTile; i++) {
        for (j=0; j<PNG.WidthTile; j++) {
            if(PNG.ASCII_Color!=NULL) TCODConsole::root->setDefaultForeground(PNG.ASCII_Color[j+i*PNG.WidthTile]);
            TCODConsole::root->print(TopLeft.X+HorLeftover+j,TopLeft.Y+VerLeftover+i,"%c",PNG.ASCII_Image[j+i*PNG.WidthTile]);
        }
    }
    TCODConsole::root->setDefaultForeground(TCODColor::white);
    TCODConsole::root->flush();
}
void level::CalculatePNGSizes(IMAGE* PNG,SUBSECTION* subsec,CONSOLEINFO Con) {
    /* FONTSIZE.Y*subsection_scale*height_tile=height ; FONTSIZE.X*subsection_scale*width_tile=width */
    if((double)PNG->Height/PNG->Width >= (double)(Con.Size.Y)/Con.Size.X) {
        /* then height_tile is fixed = ConsoleHeight-2 (to fit in console screen) */
        PNG->HeightTile = Con.CharAmount.Y;
        subsec->scale=(double)PNG->Height/(PNG->HeightTile*Con.FontSize.Y);
        PNG->WidthTile=PNG->Width/(Con.FontSize.X*subsec->scale);
    } else {
        /* then width_tile is fixed = ConsoleHeight-1 (to fit in console screen) */
        PNG->WidthTile = Con.CharAmount.X;
        subsec->scale=(double)PNG->Width/(PNG->WidthTile*Con.FontSize.X);
        PNG->HeightTile=PNG->Height/(Con.FontSize.Y*subsec->scale);
    }

    subsec->height=Con.FontSize.Y*subsec->scale; /* Calculating SUBSECTION in pixels */
    subsec->width =Con.FontSize.X*subsec->scale; /* Calculating SUBSECTION in pixels */

    /*        height_tile = height/14; unsigned subsection_height=14;
     *        width_tile = width/8;    unsigned subsection_height=8; */
}
