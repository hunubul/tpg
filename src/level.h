#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include "room.h"

class level {
private:
    int MaxRoomX,MaxRoomY;
    int MidX,MidY;
    int MaxCounter;
    int posX,posY;
    int most; //Irany típusú int
    void merre(IRANY honnan);
    void writearrow();
public:
    std::vector<std::vector<int> > Map;
    std::vector<std::vector<int> > FOV;
    std::vector<std::vector<szoba> > terkep; //egyértelmû
    level(int Max_X,int Max_Y,int MidX,int MidY,int MaxCounter);
    void engine();
	void ManageEvents();
    void writeout();
    void WriteOutMiniMap();

    void RoomWriteout();
    void WriteOutBoxes();
    void WriteMiddleBox(std::vector<std::string> aktFal);
    void WriteLeftBox(std::vector<std::string> aktFal);
    void WriteRightBox(std::vector<std::string> aktFal);

	void WriteWarpMiddleBox(std::vector<std::string> aktFal);
	void WriteWarpLeftBox(std::vector<std::string> aktFal);
	void WriteWarpRightBox(std::vector<std::string> aktFal);
};
#endif
