#include <string>
#include <fstream>
#include <time.h>
#include "libtcod.hpp"
#include "logging.h"

extern const std::string ERR_LOG;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string getCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void FatalError(std::string ErrorText) {
    std::ofstream out(ERR_LOG.c_str(),std::ofstream::out|std::ofstream::app);
    std::string PrintText="[";
    PrintText.append(getCurrentDateTime());
    PrintText.append("] A fatal error occured! ");
    PrintText.append(ErrorText+" Exitting...");
    TCODConsole::root->clear();
    TCODConsole::root->print(0,0,PrintText.c_str());
    TCODConsole::root->flush();
    out << PrintText << std::endl;
    Exitting();
    out.close();
    TCODConsole::root->waitForKeypress(true);
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
}
