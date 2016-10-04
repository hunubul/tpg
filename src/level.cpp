// OpenGL headers
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <stdexcept>
#include "level.h"
#include "globals.h"
#include "harc.h"
#include "openGL/initOpenGL.h"
#include "openGL/OpenGLRender.h"
#include "definitions/Colors.h"

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
}
/**
 * @brief a fõ loop, nyíl input kezelés
 */
void level::engine() {
	posX = MidX;
	posY = MidY;
	most = UP;
	
	//unsigned char* texture1Arr = new unsigned char[screenWidth *screenHeight * 3];
	
	// Game loop
	while (!quit) {
		ManageEvents();

		// Set frame time
		frameLimiter();

		// Check and call events
		Do_Movement();

		// Place and transform 3D to screen
		render3Dmodels();

		/*GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
		printf("Error: %x\n", err);
		}*/

		// Render subsections
		setDirtyParams();
		RenderSubsection(dirtyX[0], dirtyY[0], dirtyWidth[0], dirtyHeight[0]);
		RenderSubsection(dirtyX[1], dirtyY[1], dirtyWidth[1], dirtyHeight[1]);

		// Draw with framebuffer shader on screen
		drawBufferShader();

		// Update and render UI
		UpdateUI();
		DrawUIGL();

		// Update and render Font text
		UpdateText();
		DrawTextGL();

		// Swap the buffers
		SDL_GL_SwapWindow(window);
	}
}

void level::ManageEvents() {
	int lehet;

	while (SDL_PollEvent(&sdlEvent) != 0) {
		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym) {
			// Esc button is pressed
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_f:
				if (ASCIION) {
					ASCIION = false;
				} else {
					ASCIION = true;
					firstRender = true;
				}
				break;
#ifdef DEBUG
			case SDLK_g:
				HarcGUI(enemies[0]);
				break;
#endif
			case SDLK_LEFT:
				lehet = (most + 3) % 4;
				merre((IRANY)lehet);
				break;
			case SDLK_UP:
				lehet = (most + 0) % 4;
				merre((IRANY)lehet);
				break;
			case SDLK_RIGHT:
				lehet = (most + 1) % 4;
				merre((IRANY)lehet);
				break;
			case SDLK_DOWN:
				lehet = (most + 2) % 4;
				merre((IRANY)lehet);
				break;
			}
			if (sdlEvent.key.keysym.sym >= 0 && sdlEvent.key.keysym.sym < 1024)
				keys[sdlEvent.key.keysym.sym] = true;
			break;

		case SDL_KEYUP:
			if (sdlEvent.key.keysym.sym >= 0 && sdlEvent.key.keysym.sym < 1024)
				keys[sdlEvent.key.keysym.sym] = false;
			break;

		case SDL_MOUSEMOTION:
			camera.ProcessMouseMovement((GLfloat)sdlEvent.motion.xrel, -(GLfloat)sdlEvent.motion.yrel);
			break;

		case SDL_MOUSEWHEEL:
			camera.ProcessMouseScroll((GLfloat)sdlEvent.wheel.y);
			break;

		case SDL_QUIT:
			quit = true;
			break;

		default:
			break;

		}
	}
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
	newRoom = true;
}

/**
 * @brief kiírja az egész térképet
 */
void level::UpdateText() {
	if (newRoom) {
		//TCODConsole::root->clear();
		//RoomWriteout();
		//WriteOutBoxes();
		WriteOutMiniMap();
#ifdef DEBUG
		writearrow();
#endif // DEBUG
		//terkep[posX][posY].writeout((IRANY)most);
		//TCODConsole::root->flush();
		newRoom = false;
	}
}
void level::WriteOutMiniMap() {
	Font miniMap(fontZigPath, FONTCOLOR_WHITE);
	miniMap.fontAlign = ALIGN_LEFT;
	miniMap.setFontSize(11.0f);
	miniMap.pen_x = 0.0f;
	miniMap.pen_y = (float)screenHeight;
	//miniMap.changeBackgroundColor(FONTCOLOR_BLACK);
	for (int j = 0; j < MaxRoomX; j++) {
		for (int i = 0; i < MaxRoomY; i++) {
			if (posX == i&&posY == j) {
				FOV[i][j] = 1;
				miniMap.changeFontColor(FONTCOLOR_RED);
			}
#ifdef DEBUG
			if (Map[i][j] == 1) miniMap.append("#");
			else miniMap.append(" ");
#else
			if (FOV[i][j] == 1) {
				miniMap.append("#");
				miniMap.changeFontColor(FONTCOLOR_WHITE);
				if (i - 1 >= 0 && Map[i - 1][j] == 1 && FOV[i - 1][j] != 1) TCODConsole::root->print(i - 1, j, "?");
				if (j - 1 >= 0 && Map[i][j - 1] == 1 && FOV[i][j - 1] != 1) TCODConsole::root->print(i, j - 1, "?");
				if (i + 1 < MaxRoomX&&Map[i + 1][j] == 1 && FOV[i + 1][j] != 1) TCODConsole::root->print(i + 1, j, "?");
				if (j + 1 < MaxRoomY&&Map[i][j + 1] == 1 && FOV[i][j + 1] != 1) TCODConsole::root->print(i, j + 1, "?");
			}
			else if (TCODConsole::root->getChar(i, j) != '?') miniMap.append(" ");
#endif // DEBUG
			if (posX == i&&posY == j) {
				miniMap.changeFontColor(FONTCOLOR_WHITE);
			}
		}
		miniMap.append("|"); //║
		miniMap.NewLine();
	}
	// Drawing frame
	for (int i = 0; i < MaxRoomX; i++) {
		miniMap.append("-"); //═
	}
	miniMap.append("-"); //╝
	texts[MINIMAP_TEXT] = miniMap;
}
void level::writearrow() {
	Font arrow(fontZigPath, FONTCOLOR_WHITE);
	arrow.fontAlign = ALIGN_CENTER;
	arrow.setFontSize(12.0f);
	arrow.pen_x = 200.0f;
	arrow.pen_y = (float)screenHeight;
	switch (most) {
	case UP:
		arrow.append("/\\\n");
		arrow.append("|| ");
		break;
	case RIGHT:
		arrow.append("-->\n");
		arrow.append("  ");
		break;
	case DOWN:
		arrow.append("|| ");
		arrow.append("\\/\n");
		break;
	case LEFT:
		arrow.append("<--\n");
		arrow.append("   ");
		break;
	}
	texts[ARROW_TEXT] = arrow;
}
void level::RoomWriteout() {
	//CONSOLEINFO Con;
	//IMAGE PNG;
	//SUBSECTION subsec;
	//SIZES TopLeft = SIZES{ 0,0 };
	//SIZES BoxSize = SIZES{ ConsoleWidth,ConsoleHeight };
	///*-----------Initializing CONSOLEINFO-----------*/
	//Con.FontSize.X = FontX;
	//Con.FontSize.Y = FontY;
	//Con.CharAmount.X = ConsoleWidth;
	//Con.CharAmount.Y = ConsoleHeight;
	//Con.Size.X = Con.FontSize.X * Con.CharAmount.X;
	//Con.Size.Y = Con.FontSize.Y * Con.CharAmount.Y;
	///*----------------------------------------------*/
	//const string path = IMAGE_PATH + "Room.png";
	//int err = lodepng_decode32_file(&PNG.Image, &PNG.Width, &PNG.Height, path.c_str());
	//PNG.HeightTile = Con.CharAmount.Y;
	//PNG.WidthTile = Con.CharAmount.X;
	//subsec.height = (double)PNG.Height / PNG.HeightTile; /* Calculating SUBSECTION in pixels */
	//subsec.width = (double)PNG.Width / PNG.WidthTile; /* Calculating SUBSECTION in pixels */
	//if (!err) {
	//	PreciseProcessPNG(&PNG, subsec, CharSet); /*ProcessingPNG [in]:PNGImage,SUBSECTION,[out]: PNG_WEIGHT */
	//	WriteOutPic(&PNG, TopLeft, BoxSize);
	//	free(PNG.Image);
	//	free(PNG.ASCII_Image);
	//	free(PNG.ASCII_Color);
	//}
	//else {
	//	string ErrorText = "/" + IMAGE_PATH + "Room.png was not found!";
	//	throw runtime_error(ErrorText);
	//}
}
//void level::WriteOutBoxes() {
//	SIZES TopLeft;
//	SIZES BoxSize;
//	szoba aktSzoba = terkep[posX][posY];
//	//*** Ceiling ***
//	if (aktSzoba.c.size() == 0) {
//		TopLeft = SIZES{ UpperBoxPos.X, UpperBoxPos.Y };
//		BoxSize = SIZES{ UpperBoxSiz.X, UpperBoxSiz.Y };
//		//ClearBox(TopLeft, BoxSize);
//	}
//	else if (aktSzoba.c.size() == 1) {
//		TopLeft = SIZES{ UpperBoxPos.X, UpperBoxPos.Y };
//		BoxSize = SIZES{ UpperBoxSiz.X, UpperBoxSiz.Y };
//		Pic2ASCIIWarpandWrite(aktSzoba.c[0], CEILING_WALL, {POINTS_TOP[0][0],POINTS_TOP[2][0],POINTS_TOP[2][2],POINTS_TOP[0][2]} );
//		//Pic2ASCIIandWrite(aktSzoba.c[0], TopLeft, BoxSize);
//	}
//	else if (aktSzoba.c.size() == 2) {
//		//Elso dolog
//		TopLeft = SIZES{ UpperBoxPos.X  , UpperBoxPos.Y };
//		BoxSize = SIZES{ UpperBoxSiz.X / 2, UpperBoxSiz.Y };
//		//Pic2ASCIIandWrite(aktSzoba.c[0], TopLeft, BoxSize);
//		Pic2ASCIIWarpandWrite(aktSzoba.c[0], CEILING_WALL, { POINTS_TOP[0][0],POINTS_TOP[1][0],POINTS_TOP[1][2],POINTS_TOP[0][2] });
//		//Masodik dolog
//		TopLeft = SIZES{ UpperBoxPos.X + UpperBoxSiz.X / 2,UpperBoxPos.Y };
//		BoxSize = SIZES{ UpperBoxSiz.X / 2,UpperBoxSiz.Y };
//		//Pic2ASCIIandWrite(aktSzoba.c[1], TopLeft, BoxSize);
//		Pic2ASCIIWarpandWrite(aktSzoba.c[1], CEILING_WALL, { POINTS_TOP[1][0],POINTS_TOP[2][0],POINTS_TOP[2][2],POINTS_TOP[1][2] });
//	}
//	//*** Middle ***
//	if (most == LEFT) {
//		WriteWarpMiddleBox(aktSzoba.west);
//	}
//	else if (most == UP) {
//		WriteWarpMiddleBox(aktSzoba.north);
//	}
//	else if (most == RIGHT) {
//		WriteWarpMiddleBox(aktSzoba.east);
//	}
//	else if (most == DOWN) {
//		WriteWarpMiddleBox(aktSzoba.south);
//	}
//	//*** Left Side ***
//	if (most == LEFT) {
//		WriteWarpLeftBox(aktSzoba.south);
//	}
//	else if (most == UP) {
//		WriteWarpLeftBox(aktSzoba.west);
//	}
//	else if (most == RIGHT) {
//		WriteWarpLeftBox(aktSzoba.north);
//	}
//	else if (most == DOWN) {
//		WriteWarpLeftBox(aktSzoba.east);
//	}
//	//*** Right Side ***
//	if (most == LEFT) {
//		WriteWarpRightBox(aktSzoba.north);
//	}
//	else if (most == UP) {
//		WriteWarpRightBox(aktSzoba.east);
//	}
//	else if (most == RIGHT) {
//		WriteWarpRightBox(aktSzoba.south);
//	}
//	else if (most == DOWN) {
//		WriteWarpRightBox(aktSzoba.west);
//	}
//	//*** Bottom ***
//	if (aktSzoba.f.size() == 0) {
//		TopLeft = SIZES{ BottomBoxPos.X,BottomBoxPos.Y };
//		BoxSize = SIZES{ BottomBoxSiz.X,BottomBoxSiz.Y };
//		//ClearBox(TopLeft, BoxSize);
//	}
//	else if (aktSzoba.f.size() == 1) {
//		TopLeft = SIZES{ BottomBoxPos.X,BottomBoxPos.Y };
//		BoxSize = SIZES{ BottomBoxSiz.X,BottomBoxSiz.Y };
//		//Pic2ASCIIandWrite(aktSzoba.f[0], TopLeft, BoxSize);
//		Pic2ASCIIWarpandWrite(aktSzoba.f[0], FLOOR_WALL, { POINTS_BOTTOM[0][0],POINTS_BOTTOM[2][0],POINTS_BOTTOM[2][2],POINTS_BOTTOM[0][2] });
//	}
//	else if (aktSzoba.f.size() == 2) {
//		//Elso dolog
//		TopLeft = SIZES{ BottomBoxPos.X, BottomBoxPos.Y };
//		BoxSize = SIZES{ BottomBoxSiz.X / 2, BottomBoxSiz.Y };
//		//Pic2ASCIIandWrite(aktSzoba.f[0], TopLeft, BoxSize);
//		Pic2ASCIIWarpandWrite(aktSzoba.f[0], FLOOR_WALL, { POINTS_BOTTOM[0][0],POINTS_BOTTOM[1][0],POINTS_BOTTOM[1][2],POINTS_BOTTOM[0][2] });
//		//Masodik dolog
//		TopLeft = SIZES{ BottomBoxPos.X + BottomBoxSiz.X / 2, BottomBoxPos.Y };
//		BoxSize = SIZES{ BottomBoxSiz.X / 2, BottomBoxSiz.Y };
//		//Pic2ASCIIandWrite(aktSzoba.f[1], TopLeft, BoxSize);
//		Pic2ASCIIWarpandWrite(aktSzoba.f[1], FLOOR_WALL, { POINTS_BOTTOM[1][0],POINTS_BOTTOM[2][0],POINTS_BOTTOM[2][2],POINTS_BOTTOM[1][2] });
//	}
//}
//void level::WriteMiddleBox(vector<string> aktFal) {
//	SIZES TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y };
//	SIZES BoxSize = SIZES{ MiddleBoxSiz.X, MiddleBoxSiz.Y };
//	switch (aktFal.size()) {
//	case 1:
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		break;
//	case 2:
//		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		break;
//	case 3:
//		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y };
//		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y / 2 };
//		TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y + MiddleBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
//		break;
//	case 4:
//		BoxSize = SIZES{ MiddleBoxSiz.X / 2, MiddleBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y + MiddleBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
//		TopLeft = SIZES{ MiddleBoxPos.X + MiddleBoxSiz.X / 2, MiddleBoxPos.Y + MiddleBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[3], TopLeft, BoxSize);
//		break;
//	default:
//		ClearBox(TopLeft, BoxSize);
//		break;
//	}
//}
//void level::WriteLeftBox(vector<string> aktFal) {
//	SIZES TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y };
//	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
//	switch (aktFal.size()) {
//	case 1:
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		break;
//	case 2:
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		break;
//	case 3:
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
//		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
//		TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y + SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
//		break;
//	case 4:
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y + SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosLeft.X + SideBoxSiz.X / 2, SideBoxPosLeft.Y + SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[3], TopLeft, BoxSize);
//		break;
//	default:
//		ClearBox(TopLeft, BoxSize);
//		break;
//	}
//}
//void level::WriteRightBox(vector<string> aktFal) {
//	SIZES TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y };
//	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
//	switch (aktFal.size()) {
//	case 1:
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		break;
//	case 2:
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		break;
//	case 3:
//		BoxSize = { SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y };
//		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
//		TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y + SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
//		break;
//	case 4:
//		BoxSize = SIZES{ SideBoxSiz.X / 2, SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[0], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y };
//		Pic2ASCIIandWrite(aktFal[1], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y + SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[2], TopLeft, BoxSize);
//		TopLeft = SIZES{ SideBoxPosRight.X + SideBoxSiz.X / 2, SideBoxPosRight.Y + SideBoxSiz.Y / 2 };
//		Pic2ASCIIandWrite(aktFal[3], TopLeft, BoxSize);
//		break;
//	default:
//		ClearBox(TopLeft, BoxSize);
//		break;
//	}
//}
//
//void level::WriteWarpMiddleBox(vector<string> aktFal) {
//	SIZES TopLeft = SIZES{ MiddleBoxPos.X, MiddleBoxPos.Y };
//	SIZES BoxSize = SIZES{ MiddleBoxSiz.X, MiddleBoxSiz.Y };
//	switch (aktFal.size()) {
//	case 1:
//		Pic2ASCIIWarpandWrite(aktFal[0], MIDDLE_WALL, {POINTS_MIDDLE[0][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][4],POINTS_MIDDLE[0][4] });
//		break;
//	case 2:
//		Pic2ASCIIWarpandWrite(aktFal[0], MIDDLE_WALL, { POINTS_MIDDLE[0][0],POINTS_MIDDLE[1][0],POINTS_MIDDLE[1][4],POINTS_MIDDLE[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[1], MIDDLE_WALL, { POINTS_MIDDLE[1][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][4],POINTS_MIDDLE[1][4] });
//		break;
//	case 3:
//		Pic2ASCIIWarpandWrite(aktFal[0], MIDDLE_WALL, { POINTS_MIDDLE[0][0],POINTS_MIDDLE[1][0],POINTS_MIDDLE[1][4],POINTS_MIDDLE[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[1], MIDDLE_WALL, { POINTS_MIDDLE[1][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][2],POINTS_MIDDLE[1][2] });
//		Pic2ASCIIWarpandWrite(aktFal[2], MIDDLE_WALL, { POINTS_MIDDLE[1][2],POINTS_MIDDLE[2][2],POINTS_MIDDLE[2][4],POINTS_MIDDLE[1][4] });
//		break;
//	case 4:
//		Pic2ASCIIWarpandWrite(aktFal[0], MIDDLE_WALL, { POINTS_MIDDLE[0][0],POINTS_MIDDLE[1][0],POINTS_MIDDLE[1][2],POINTS_MIDDLE[0][2] });
//		Pic2ASCIIWarpandWrite(aktFal[1], MIDDLE_WALL, { POINTS_MIDDLE[0][2],POINTS_MIDDLE[1][2],POINTS_MIDDLE[1][4],POINTS_MIDDLE[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[2], MIDDLE_WALL, { POINTS_MIDDLE[1][0],POINTS_MIDDLE[2][0],POINTS_MIDDLE[2][2],POINTS_MIDDLE[1][2] });
//		Pic2ASCIIWarpandWrite(aktFal[3], MIDDLE_WALL, { POINTS_MIDDLE[1][2],POINTS_MIDDLE[2][2],POINTS_MIDDLE[2][4],POINTS_MIDDLE[1][4] });
//		break;
//	default:
//		ClearBox(TopLeft, BoxSize);
//		break;
//	}
//}
//void level::WriteWarpLeftBox(vector<string> aktFal) {
//	SIZES TopLeft = SIZES{ SideBoxPosLeft.X, SideBoxPosLeft.Y };
//	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
//	switch (aktFal.size()) {
//	case 1:
//		Pic2ASCIIWarpandWrite(aktFal[0], LEFT_WALL, { POINTS_LEFT[0][0],POINTS_LEFT[2][0],POINTS_LEFT[2][4],POINTS_LEFT[0][4] });
//		break;
//	case 2:
//		Pic2ASCIIWarpandWrite(aktFal[0], LEFT_WALL, { POINTS_LEFT[0][0],POINTS_LEFT[1][0],POINTS_LEFT[1][4],POINTS_LEFT[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[1], LEFT_WALL, { POINTS_LEFT[1][0],POINTS_LEFT[2][0],POINTS_LEFT[2][4],POINTS_LEFT[1][4] });
//		break;
//	case 3:
//		Pic2ASCIIWarpandWrite(aktFal[0], LEFT_WALL, { POINTS_LEFT[0][0],POINTS_LEFT[1][0],POINTS_LEFT[1][4],POINTS_LEFT[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[1], LEFT_WALL, { POINTS_LEFT[1][0],POINTS_LEFT[2][0],POINTS_LEFT[2][2],POINTS_LEFT[1][2] });
//		Pic2ASCIIWarpandWrite(aktFal[2], LEFT_WALL, { POINTS_LEFT[1][2],POINTS_LEFT[2][2],POINTS_LEFT[2][4],POINTS_LEFT[1][4] });
//		break;
//	case 4:
//		Pic2ASCIIWarpandWrite(aktFal[0], LEFT_WALL, { POINTS_LEFT[0][0],POINTS_LEFT[1][0],POINTS_LEFT[1][2],POINTS_LEFT[0][2] });
//		Pic2ASCIIWarpandWrite(aktFal[1], LEFT_WALL, { POINTS_LEFT[0][2],POINTS_LEFT[1][2],POINTS_LEFT[1][4],POINTS_LEFT[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[2], LEFT_WALL, { POINTS_LEFT[1][0],POINTS_LEFT[2][0],POINTS_LEFT[2][2],POINTS_LEFT[1][2] });
//		Pic2ASCIIWarpandWrite(aktFal[3], LEFT_WALL, { POINTS_LEFT[1][2],POINTS_LEFT[2][2],POINTS_LEFT[2][4],POINTS_LEFT[1][4] });
//		break;
//	default:
//		ClearBox(TopLeft, BoxSize);
//		break;
//	}
//}
//void level::WriteWarpRightBox(vector<string> aktFal) {
//	SIZES TopLeft = SIZES{ SideBoxPosRight.X, SideBoxPosRight.Y };
//	SIZES BoxSize = SIZES{ SideBoxSiz.X, SideBoxSiz.Y };
//	switch (aktFal.size()) {
//	case 1:
//		Pic2ASCIIWarpandWrite(aktFal[0], RIGHT_WALL, { POINTS_RIGHT[0][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][4],POINTS_RIGHT[0][4] });
//		break;
//	case 2:
//		Pic2ASCIIWarpandWrite(aktFal[0], RIGHT_WALL, { POINTS_RIGHT[0][0],POINTS_RIGHT[1][0],POINTS_RIGHT[1][4],POINTS_RIGHT[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[1], RIGHT_WALL, { POINTS_RIGHT[1][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][4],POINTS_RIGHT[1][4] });
//		break;
//	case 3:
//		Pic2ASCIIWarpandWrite(aktFal[0], RIGHT_WALL, { POINTS_RIGHT[0][0],POINTS_RIGHT[1][0],POINTS_RIGHT[1][4],POINTS_RIGHT[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[1], RIGHT_WALL, { POINTS_RIGHT[1][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][2],POINTS_RIGHT[1][2] });
//		Pic2ASCIIWarpandWrite(aktFal[2], RIGHT_WALL, { POINTS_RIGHT[1][2],POINTS_RIGHT[2][2],POINTS_RIGHT[2][4],POINTS_RIGHT[1][4] });
//		break;
//	case 4:
//		Pic2ASCIIWarpandWrite(aktFal[0], RIGHT_WALL, { POINTS_RIGHT[0][0],POINTS_RIGHT[1][0],POINTS_RIGHT[1][2],POINTS_RIGHT[0][2] });
//		Pic2ASCIIWarpandWrite(aktFal[1], RIGHT_WALL, { POINTS_RIGHT[0][2],POINTS_RIGHT[1][2],POINTS_RIGHT[1][4],POINTS_RIGHT[0][4] });
//		Pic2ASCIIWarpandWrite(aktFal[2], RIGHT_WALL, { POINTS_RIGHT[1][0],POINTS_RIGHT[2][0],POINTS_RIGHT[2][2],POINTS_RIGHT[1][2] });
//		Pic2ASCIIWarpandWrite(aktFal[3], RIGHT_WALL, { POINTS_RIGHT[1][2],POINTS_RIGHT[2][2],POINTS_RIGHT[2][4],POINTS_RIGHT[1][4] });
//		break;
//	default:
//		ClearBox(TopLeft, BoxSize);
//		break;
//	}
//}

void level::UpdateUI() {

}