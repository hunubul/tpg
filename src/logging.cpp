#include <string>
#include <fstream>
#include <time.h>
#include "logging.h"
#include "globals.h"
#include "openGL/initOpenGL.h"

using namespace globals;
bool loggedSomething = false;

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

void FatalError(const std::string& ErrorText) {
	loggedSomething = true;
    std::ofstream out(ERR_LOG_PATH.c_str(),std::ofstream::out|std::ofstream::app);
    std::string PrintText="[";
    PrintText.append(getCurrentDateTime());
    PrintText.append("] A fatal error occured! - ");
    PrintText.append(ErrorText);
	PrintText.append("\nExitting...");
    out << PrintText << std::endl;
    out.close();
    Exitting();

#ifdef DEBUG
	std::cout << PrintText << std::endl;
	std::cout << "Press enter to exit...";
	std::cin.ignore(); // Wait for keypress
#endif
}
void ErrorOccured(const std::string& ErrorText) {
	loggedSomething = true;
    std::ofstream out(ERR_LOG_PATH.c_str(),std::ofstream::out|std::ofstream::app);
    std::string PrintText="[";
    PrintText.append(getCurrentDateTime());
    PrintText.append("] An error occured! - ");
	PrintText.append(ErrorText);
    out << PrintText << std::endl;
    out.close();

#ifdef DEBUG
	std::cout << PrintText << std::endl;
#endif
}
void DebugLog(const std::string& DebugText) {
#ifdef DEBUG
	std::string PrintText = "[";
	PrintText.append(getCurrentDateTime());
	PrintText.append("] DEBUG INFO - ");
	PrintText.append(DebugText);
	std::cout << PrintText << std::endl;
#endif
}
void Exitting() {
	if (loggedSomething) {
		std::ofstream out(ERR_LOG_PATH.c_str(), std::ofstream::out | std::ofstream::app);
		std::string PrintText = "*******************************************************************";
		out << PrintText << std::endl;
		out.close();
	}

	// Properly de-allocate all resources once they've outlived their purpose
	if (initialisedGL) {
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
	}

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
