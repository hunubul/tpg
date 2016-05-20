#include <stdexcept>
#include "level.h"
#include "lodepng\lodepng.h"
#include "globals.h"
#include "harc.h"

using namespace globals;
using namespace std;

/**
 * @brief level konstruktor
 * @param Max_X Az egész szoba világ szélessége
 * @param Max_Y Az egész szoba világ magassága
 * @param MidX Ettõl az X koordinátától kezdi a szobák létrehozását
 * @param MidY Ettõl az Y koordinátától kezdi a szobák létrehozását
 * @param MaxCounter Összesen mennyi szobát szeretnénk
 */
level::level(int Max_X, int Max_Y, int MidX, int MidY, int MaxCounter) : MidX(MidX), MidY(MidY), MaxCounter(MaxCounter) {
	MaxRoomX = Max_X;
	MaxRoomY = Max_Y;
	posX = MidX;
	posY = MidY;
	bool allow; //Ez azt csekkolja, hogy folyosók jöjjenek létre
	int counter, rnd;
	for (int i = 0; i < MaxRoomY; i++) {
		vector<int> temp;
		for (int j = 0; j < MaxRoomX; j++) {
			temp.push_back(0);
		}
		Map.push_back(temp);
		FOV.push_back(temp);
	}
	counter = 2;
	Map[posX][posY] = 1; //Középső szoba lesz
	//Map[posX][posY + 1] = 1; //Középső szoba lesz
	//Ez úgy mûködik, hogy az eddig átjárható szobákból próbál új szobákat létrehozni
	while (counter != MaxCounter) {
		allow = false;
		rnd = rand() % 4;
		switch (rnd) {
		case 0: //Left
			if (posX != 0) {
				posX--;
				if (posY - 1 >= 0 && posY + 1 < MaxRoomY&&Map[posX][posY - 1] == 0 && Map[posX][posY + 1] == 0) allow = true;
				else if (Map[posX][posY] == 0) posX++; //Ha nem megfelelõ a feltétel akkor visszamegyünk
			}
			break;
		case 1: //Up
			if (posY != 0) {
				posY--;
				if (posX - 1 >= 0 && posX + 1 < MaxRoomX&&Map[posX - 1][posY] == 0 && Map[posX + 1][posY] == 0) allow = true;
				else if (Map[posX][posY] == 0) posY++;
			}
			break;
		case 2: //Right
			if (posX + 1 != MaxRoomX) {
				posX++;
				if (posY - 1 >= 0 && posY + 1 < MaxRoomY&&Map[posX][posY - 1] == 0 && Map[posX][posY + 1] == 0) allow = true;
				else if (Map[posX][posY] == 0) posX--;
			}
			break;
		case 3: //Down
			if (posY + 1 != MaxRoomY) {
				posY++;
				if (posX - 1 >= 0 && posX + 1 < MaxRoomX&&Map[posX - 1][posY] == 0 && Map[posX + 1][posY] == 0) allow = true;
				else if (Map[posX][posY] == 0) posY--;
			}
			break;
		}
		if (allow&&Map[posX][posY] == 0) {
			Map[posX][posY] = 1;
			counter++;
		}
	}
	//boost::posix_time::ptime timetmp=boost::posix_time::microsec_clock::local_time();
	int direction; //Flagek használatával, a szoba kostruktorában meghatározhatjuk hogy melyik falra kell ajtó
	for (int i = 0; i < MaxRoomX; i++) { //tényleges, követhetõ térkép csinálása
		vector<szoba> temp;
		for (int j = 0; j < MaxRoomY; j++) {
			direction = DOOR_NONE;
			if (i - 1 >= 0 && Map[i - 1][j] == 1) direction |= DOOR_LEFT;
			if (j - 1 >= 0 && Map[i][j - 1] == 1) direction |= DOOR_UP;
			if (i + 1 < MaxRoomX&&Map[i + 1][j] == 1) direction |= DOOR_RIGHT;
			if (j + 1 < MaxRoomY&&Map[i][j + 1] == 1) direction |= DOOR_DOWN;
			szoba Szoba((DOOR_DIRECTION)direction);
			temp.push_back(Szoba);
		}
		terkep.push_back(temp);
	}
	//GenTimeSzobaFeltoltes = boost::posix_time::microsec_clock::local_time() - timetmp;
}
/**
 * @brief a fõ loop, nyíl input kezelés
 */
void level::engine() {
	posX = MidX;
	posY = MidY;
	most = UP;
	int lehet;
	int prevX = posX;
	int prevY = posY;
	TCOD_key_t key;
	TCOD_mouse_t mouse;
	TCOD_event_t event;
	bool procNeighb=true; //Process Neighbours
	writeout();
	do {
		if (prevX != posX || prevY != posY) writeout();
		prevX = posX;
		prevY = posY;
		event = TCODSystem::waitForEvent(TCOD_EVENT_ANY, &key, &mouse, true);
		TCODSystem::sleepMilli(1000 / FPS);
		//HarcGUI(enemies[0]);
		if (event == TCOD_EVENT_KEY_PRESS) {
			switch (key.vk) { //Irány változó típusból adódik
			case TCODK_LEFT:
				lehet = (most + 3) % 4;
				merre((IRANY)lehet);
				break;
			case TCODK_UP:
				lehet = (most + 0) % 4;
				merre((IRANY)lehet);
				break;
			case TCODK_RIGHT:
				lehet = (most + 1) % 4;
				merre((IRANY)lehet);
				break;
			case TCODK_DOWN:
				lehet = (most + 2) % 4;
				merre((IRANY)lehet);
				break;
			default:
				break;
			}
#ifdef DEBUG
			if (key.c=='d') {
				HarcGUI(enemies[0]);
			}
#endif
		}
		
		else if (event == TCOD_EVENT_MOUSE_PRESS) {
			if (mouse.lbutton) {

			}
			else if (mouse.rbutton) {

			}
		}
	} while (!TCODConsole::isWindowClosed() && key.vk != TCODK_ESCAPE);
}
void level::merre(IRANY honnan) {
	switch (honnan) {
	case UP:
		if (posY != 0 && Map[posX][posY - 1] == 1) {
			posY--;
			most = UP;
		}
		break;
	case RIGHT:
		if (posX + 1 != MaxRoomX&&Map[posX + 1][posY] == 1) {
			posX++;
			most = RIGHT;
		}
		break;
	case DOWN:
		if (posY + 1 != MaxRoomY&&Map[posX][posY + 1] == 1) {
			posY++;
			most = DOWN;
		}
		break;
	case LEFT:
		if (posX != 0 && Map[posX - 1][posY] == 1) {
			posX--;
			most = LEFT;
		}
		break;
	}
}
/**
 * @brief kiírja az egész térképet, + az aktív szoba tartalmát
 */
void level::writeout() {
	TCODConsole::root->clear();
	//boost::posix_time::ptime timetmp=boost::posix_time::microsec_clock::local_time();
	RoomWriteout();
	WriteOutBoxes();
	WriteOutMiniMap();
#ifdef DEBUG
	//WriteOutGenTime(timetmp);
#endif // DEBUG
	//terkep[posX][posY].writeout((IRANY)most);
	TCODConsole::root->flush();
}
void level::WriteOutMiniMap() {
	for (int i = 0; i < MaxRoomX; i++) {
		for (int j = 0; j < MaxRoomY; j++) {
			if (posX == i&&posY == j) {
				FOV[i][j] = 1;
				TCODConsole::root->setDefaultForeground(TCODColor::red);
			}
#ifdef DEBUG
			writearrow();
			if (Map[i][j] == 1) TCODConsole::root->print(i, j, "#");
			else TCODConsole::root->print(i, j, " ");
#else
			if (FOV[i][j] == 1) {
				TCODConsole::root->print(i, j, "#");
				TCODConsole::root->setDefaultForeground(TCODColor::white);
				if (i - 1 >= 0 && Map[i - 1][j] == 1 && FOV[i - 1][j] != 1) TCODConsole::root->print(i - 1, j, "?");
				if (j - 1 >= 0 && Map[i][j - 1] == 1 && FOV[i][j - 1] != 1) TCODConsole::root->print(i, j - 1, "?");
				if (i + 1 < MaxRoomX&&Map[i + 1][j] == 1 && FOV[i + 1][j] != 1) TCODConsole::root->print(i + 1, j, "?");
				if (j + 1 < MaxRoomY&&Map[i][j + 1] == 1 && FOV[i][j + 1] != 1) TCODConsole::root->print(i, j + 1, "?");
			}
			else if (TCODConsole::root->getChar(i, j) != '?') TCODConsole::root->print(i, j, " ");
#endif // DEBUG
			TCODConsole::root->setDefaultForeground(TCODColor::white);
		}
	}
	// Drawing frame
	for (int i = 0; i < MaxRoomX; i++) {
		TCODConsole::root->print(i, MaxRoomY, "%c", TCOD_CHAR_DHLINE);
	}
	for (int j = 0; j < MaxRoomY; j++) {
		TCODConsole::root->print(MaxRoomX, j, "%c", TCOD_CHAR_DVLINE);
	}
	TCODConsole::root->print(MaxRoomX, MaxRoomY, "%c", TCOD_CHAR_DSE);
}
void level::writearrow() {
	switch (most) {
	case UP:
		TCODConsole::root->print(MaxRoomX + 3, 0, "/\\ ");
		TCODConsole::root->print(MaxRoomX + 3, 1, "|| ");
		break;
	case RIGHT:
		TCODConsole::root->print(MaxRoomX + 3, 0, "-->");
		TCODConsole::root->print(MaxRoomX + 3, 1, "  ");
		break;
	case DOWN:
		TCODConsole::root->print(MaxRoomX + 3, 0, "|| ");
		TCODConsole::root->print(MaxRoomX + 3, 1, "\\/");
		break;
	case LEFT:
		TCODConsole::root->print(MaxRoomX + 3, 0, "<--");
		TCODConsole::root->print(MaxRoomX + 3, 1, "  ");
		break;
	}
}
void level::RoomWriteout() {
	CONSOLEINFO Con;
	IMAGE PNG;
	SUBSECTION subsec;
	SIZES TopLeft = SIZES{ 0,0 };
	SIZES BoxSize = SIZES{ ConsoleWidth,ConsoleHeight };
	/*-----------Initializing CONSOLEINFO-----------*/
	Con.FontSize.X = FontX;
	Con.FontSize.Y = FontY;
	Con.CharAmount.X = ConsoleWidth;
	Con.CharAmount.Y = ConsoleHeight;
	Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
	Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
	/*----------------------------------------------*/
	const string path = IMAGE_PATH + "Room.png";
	int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, path.c_str());
	PNG.HeightTile = Con.CharAmount.Y;
	PNG.WidthTile = Con.CharAmount.X;
	subsec.height = (double)PNG.Height / PNG.HeightTile; /* Calculating SUBSECTION in pixels */
	subsec.width = (double)PNG.Width / PNG.WidthTile; /* Calculating SUBSECTION in pixels */
	if (!err) {
		PreciseProcessPNG(&PNG, subsec, CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
		WriteOutPic(&PNG, TopLeft, BoxSize);
		free(PNG.Image);
		free(PNG.ASCII_Image);
		free(PNG.ASCII_Color);
	}
	else {
		string ErrorText = "/" + IMAGE_PATH + "Room.png was not found!";
		throw runtime_error(ErrorText);
	}
}
void level::WriteOutBoxes() {
	SIZES TopLeft;
	SIZES BoxSize;
	szoba aktSzoba = terkep[posX][posY];
	//*** Ceiling ***
	if (aktSzoba.c.size() == 0) {
		TopLeft = SIZES{ UpperBoxPos.X, UpperBoxPos.Y };
		BoxSize = SIZES{ UpperBoxSiz.X, UpperBoxSiz.Y };
		//ClearBox(TopLeft, BoxSize);
	}
	else if (aktSzoba.c.size() == 1) {
		TopLeft = SIZES{ UpperBoxPos.X, UpperBoxPos.Y };
		BoxSize = SIZES{ UpperBoxSiz.X, UpperBoxSiz.Y };
		Pic2ASCIIWarpandWrite(aktSzoba.c[0], {POINTS_TOP[0][0],POINTS_TOP[2][0],POINTS_TOP[2][2],POINTS_TOP[0][2]} );
		//Pic2ASCIIandWrite(aktSzoba.c[0], TopLeft, BoxSize);
	}
	else if (aktSzoba.c.size() == 2) {
		//Elso dolog
		TopLeft = SIZES{ UpperBoxPos.X  , UpperBoxPos.Y };
		BoxSize = SIZES{ UpperBoxSiz.X / 2, UpperBoxSiz.Y };
		//Pic2ASCIIandWrite(aktSzoba.c[0], TopLeft, BoxSize);
		Pic2ASCIIWarpandWrite(aktSzoba.c[0], { POINTS_TOP[0][0],POINTS_TOP[1][0],POINTS_TOP[1][2],POINTS_TOP[0][2] });
		//Masodik dolog
		TopLeft = SIZES{ UpperBoxPos.X + UpperBoxSiz.X / 2,UpperBoxPos.Y };
		BoxSize = SIZES{ UpperBoxSiz.X / 2,UpperBoxSiz.Y };
		//Pic2ASCIIandWrite(aktSzoba.c[1], TopLeft, BoxSize);
		Pic2ASCIIWarpandWrite(aktSzoba.c[1], { POINTS_TOP[1][0],POINTS_TOP[2][0],POINTS_TOP[2][2],POINTS_TOP[1][2] });
	}
	//*** Middle ***
	if (most == LEFT) {
		WriteWarpMiddleBox(aktSzoba.west);
	}
	else if (most == UP) {
		WriteWarpMiddleBox(aktSzoba.north);
	}
	else if (most == RIGHT) {
		WriteWarpMiddleBox(aktSzoba.east);
	}
	else if (most == DOWN) {
		WriteWarpMiddleBox(aktSzoba.south);
	}
	//*** Left Side ***
	if (most == LEFT) {
		WriteWarpLeftBox(aktSzoba.south);
	}
	else if (most == UP) {
		WriteWarpLeftBox(aktSzoba.west);
	}
	else if (most == RIGHT) {
		WriteWarpLeftBox(aktSzoba.north);
	}
	else if (most == DOWN) {
		WriteWarpLeftBox(aktSzoba.east);
	}
	//*** Right Side ***
	if (most == LEFT) {
		WriteWarpRightBox(aktSzoba.north);
	}
	else if (most == UP) {
		WriteWarpRightBox(aktSzoba.east);
	}
	else if (most == RIGHT) {
		WriteWarpRightBox(aktSzoba.south);
	}
	else if (most == DOWN) {
		WriteWarpRightBox(aktSzoba.west);
	}
	//*** Bottom ***
	if (aktSzoba.f.size() == 0) {
		TopLeft = SIZES{ BottomBoxPos.X,BottomBoxPos.Y };
		BoxSize = SIZES{ BottomBoxSiz.X,BottomBoxSiz.Y };
		//ClearBox(TopLeft, BoxSize);
	}
	else if (aktSzoba.f.size() == 1) {
		TopLeft = SIZES{ BottomBoxPos.X,BottomBoxPos.Y };
		BoxSize = SIZES{ BottomBoxSiz.X,BottomBoxSiz.Y };
		//Pic2ASCIIandWrite(aktSzoba.f[0], TopLeft, BoxSize);
		Pic2ASCIIWarpandWrite(aktSzoba.f[0], { POINTS_BOTTOM[0][0],POINTS_BOTTOM[2][0],POINTS_BOTTOM[2][2],POINTS_BOTTOM[0][2] });
	}
	else if (aktSzoba.f.size() == 2) {
		//Elso dolog
		TopLeft = SIZES{ BottomBoxPos.X, BottomBoxPos.Y };
		BoxSize = SIZES{ BottomBoxSiz.X / 2, BottomBoxSiz.Y };
		//Pic2ASCIIandWrite(aktSzoba.f[0], TopLeft, BoxSize);
		Pic2ASCIIWarpandWrite(aktSzoba.f[0], { POINTS_BOTTOM[0][0],POINTS_BOTTOM[1][0],POINTS_BOTTOM[1][2],POINTS_BOTTOM[0][2] });
		//Masodik dolog
		TopLeft = SIZES{ BottomBoxPos.X + BottomBoxSiz.X / 2, BottomBoxPos.Y };
		BoxSize = SIZES{ BottomBoxSiz.X / 2, BottomBoxSiz.Y };
		//Pic2ASCIIandWrite(aktSzoba.f[1], TopLeft, BoxSize);
		Pic2ASCIIWarpandWrite(aktSzoba.f[1], { POINTS_BOTTOM[1][0],POINTS_BOTTOM[2][0],POINTS_BOTTOM[2][2],POINTS_BOTTOM[1][2] });
	}
}
void level::WriteMiddleBox(vector<string> aktFal) {
	SIZES TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y };
	SIZES BoxSize = SIZES{ MiddleBoxSiz.X, MiddleBoxSiz.Y };
	switch (aktFal.size()) {
	case 1:
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		break;
	case 2:
		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		break;
	case 3:
		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y };
		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y / 2 };
		TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y + MiddleBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
		break;
	case 4:
		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y + MiddleBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y + MiddleBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[3], TopLeft, BoxSize);
		break;
	default:
		ClearBox(TopLeft, BoxSize);
		break;
	}
}
void level::WriteLeftBox(vector<string> aktFal) {
	SIZES TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y };
	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
	switch (aktFal.size()) {
	case 1:
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		break;
	case 2:
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		break;
	case 3:
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
		TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y + SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
		break;
	case 4:
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y + SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y + SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[3], TopLeft, BoxSize);
		break;
	default:
		ClearBox(TopLeft, BoxSize);
		break;
	}
}
void level::WriteRightBox(vector<string> aktFal) {
	SIZES TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y };
	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
	switch (aktFal.size()) {
	case 1:
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		break;
	case 2:
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		break;
	case 3:
		BoxSize = { SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
		TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y + SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
		break;
	case 4:
		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y };
		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y + SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y + SideBoxSiz.Y / 2 };
		Pic2ASCIIandWrite(aktFal[3], TopLeft, BoxSize);
		break;
	default:
		ClearBox(TopLeft, BoxSize);
		break;
	}
}

void level::WriteWarpMiddleBox(vector<string> aktFal) {
	SIZES TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y };
	SIZES BoxSize = SIZES{ MiddleBoxSiz.X, MiddleBoxSiz.Y };
	switch (aktFal.size()) {
	case 1:
		Pic2ASCIIWarpandWrite(aktFal[0], {POINTS_MIDDLE[0][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][4],POINTS_MIDDLE[0][4] });
		break;
	case 2:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_MIDDLE[0][0],POINTS_MIDDLE[1][0],POINTS_MIDDLE[1][4],POINTS_MIDDLE[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_MIDDLE[1][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][4],POINTS_MIDDLE[1][4] });
		break;
	case 3:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_MIDDLE[0][0],POINTS_MIDDLE[1][0],POINTS_MIDDLE[1][4],POINTS_MIDDLE[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_MIDDLE[1][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][2],POINTS_MIDDLE[1][2] });
		Pic2ASCIIWarpandWrite(aktFal[2], { POINTS_MIDDLE[1][2],POINTS_MIDDLE[2][2],POINTS_MIDDLE[2][4],POINTS_MIDDLE[1][4] });
		break;
	case 4:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_MIDDLE[0][0],POINTS_MIDDLE[1][0],POINTS_MIDDLE[1][2],POINTS_MIDDLE[0][2] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_MIDDLE[0][2],POINTS_MIDDLE[1][2],POINTS_MIDDLE[1][4],POINTS_MIDDLE[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[2], { POINTS_MIDDLE[1][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][2],POINTS_MIDDLE[1][2] });
		Pic2ASCIIWarpandWrite(aktFal[3], { POINTS_MIDDLE[1][2],POINTS_MIDDLE[2][2],POINTS_MIDDLE[2][4],POINTS_MIDDLE[1][4] });
		break;
	default:
		ClearBox(TopLeft, BoxSize);
		break;
	}
}
void level::WriteWarpLeftBox(vector<string> aktFal) {
	SIZES TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y };
	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
	switch (aktFal.size()) {
	case 1:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_LEFT[0][0],POINTS_LEFT[2][0],POINTS_LEFT[2][4],POINTS_LEFT[0][4] });
		break;
	case 2:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_LEFT[0][0],POINTS_LEFT[1][0],POINTS_LEFT[1][4],POINTS_LEFT[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_LEFT[1][0],POINTS_LEFT[2][0],POINTS_LEFT[2][4],POINTS_LEFT[1][4] });
		break;
	case 3:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_LEFT[0][0],POINTS_LEFT[1][0],POINTS_LEFT[1][4],POINTS_LEFT[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_LEFT[1][0],POINTS_LEFT[2][0],POINTS_LEFT[2][2],POINTS_LEFT[1][2] });
		Pic2ASCIIWarpandWrite(aktFal[2], { POINTS_LEFT[1][2],POINTS_LEFT[2][2],POINTS_LEFT[2][4],POINTS_LEFT[1][4] });
		break;
	case 4:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_LEFT[0][0],POINTS_LEFT[1][0],POINTS_LEFT[1][2],POINTS_LEFT[0][2] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_LEFT[0][2],POINTS_LEFT[1][2],POINTS_LEFT[1][4],POINTS_LEFT[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[2], { POINTS_LEFT[1][0],POINTS_LEFT[2][0],POINTS_LEFT[2][2],POINTS_LEFT[1][2] });
		Pic2ASCIIWarpandWrite(aktFal[3], { POINTS_LEFT[1][2],POINTS_LEFT[2][2],POINTS_LEFT[2][4],POINTS_LEFT[1][4] });
		break;
	default:
		ClearBox(TopLeft, BoxSize);
		break;
	}
}
void level::WriteWarpRightBox(vector<string> aktFal) {
	SIZES TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y };
	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
	switch (aktFal.size()) {
	case 1:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_RIGHT[0][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][4],POINTS_RIGHT[0][4] });
		break;
	case 2:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_RIGHT[0][0],POINTS_RIGHT[1][0],POINTS_RIGHT[1][4],POINTS_RIGHT[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_RIGHT[1][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][4],POINTS_RIGHT[1][4] });
		break;
	case 3:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_RIGHT[0][0],POINTS_RIGHT[1][0],POINTS_RIGHT[1][4],POINTS_RIGHT[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_RIGHT[1][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][2],POINTS_RIGHT[1][2] });
		Pic2ASCIIWarpandWrite(aktFal[2], { POINTS_RIGHT[1][2],POINTS_RIGHT[2][2],POINTS_RIGHT[2][4],POINTS_RIGHT[1][4] });
		break;
	case 4:
		Pic2ASCIIWarpandWrite(aktFal[0], { POINTS_RIGHT[0][0],POINTS_RIGHT[1][0],POINTS_RIGHT[1][2],POINTS_RIGHT[0][2] });
		Pic2ASCIIWarpandWrite(aktFal[1], { POINTS_RIGHT[0][2],POINTS_RIGHT[1][2],POINTS_RIGHT[1][4],POINTS_RIGHT[0][4] });
		Pic2ASCIIWarpandWrite(aktFal[2], { POINTS_RIGHT[1][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][2],POINTS_RIGHT[1][2] });
		Pic2ASCIIWarpandWrite(aktFal[3], { POINTS_RIGHT[1][2],POINTS_RIGHT[2][2],POINTS_RIGHT[2][4],POINTS_RIGHT[1][4] });
		break;
	default:
		ClearBox(TopLeft, BoxSize);
		break;
	}
}