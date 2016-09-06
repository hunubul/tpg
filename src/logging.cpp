#include <string>
#include <fstream>
#include <time.h>
#include "logging.h"
#include "globals.h"
#include "openGL/initOpenGL.h"

using namespace globals;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string getCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void FatalError(std::string ErrorText) {
    std::ofstream out(ERR_LOG.c_str(),std::ofstream::out|std::ofstream::app);
    std::string PrintText="[";
    PrintText.append(getCurrentDateTime());
    PrintText.append("] A fatal error occured!");
    //TCODConsole::root->clear();
	std::string windowText = PrintText + " Exitting...";
    //TCODConsole::root->print(0,0,windowText.c_str());
    //TCODConsole::root->flush();
    PrintText.append(" "+ErrorText+" Exitting...");
	fprintf(stdout,"%s",PrintText.c_str());
    out << PrintText << std::endl;
    Exitting();
    out.close();
    //TCODConsole::root->waitForKeypress(true);
    exit(1);
}
void ErrorOccured(std::string ErrorText) {
    std::ofstream out(ERR_LOG.c_str(),std::ofstream::out|std::ofstream::app);
    std::string PrintText="[";
    PrintText.append(getCurrentDateTime());
    PrintText.append("] An error occured! "+ErrorText);
    out << PrintText << std::endl;
    out.close();
}
void Exitting() {
    std::ofstream out(ERR_LOG.c_str(),std::ofstream::out|std::ofstream::app);
    std::string PrintText="*******************************************************************";
    out << PrintText << std::endl;
    out.close();

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteVertexArrays(1, &VAO_FrameBuff);
	glDeleteBuffers(1, &VBO_FrameBuff);
	glDeleteVertexArrays(1, &VAOfront);
	glDeleteBuffers(1, &VBOfront);
	glDeleteVertexArrays(1, &VAOleft);
	glDeleteBuffers(1, &VBOleft);
	glDeleteVertexArrays(1, &VAOright);
	glDeleteBuffers(1, &VBOright);
	glDeleteVertexArrays(1, &VAOfloor);
	glDeleteBuffers(1, &VBOfloor);
	glDeleteVertexArrays(1, &VAOceiling);
	glDeleteBuffers(1, &VBOceiling);
	//gl3fonsDelete(fs);

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
