#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

const std::string getCurrentDateTime();
void FatalError(const std::string& ErrorText);
void ErrorOccured(const std::string& ErrorText);
void DebugLog(const std::string& DebugText);
void Exitting();

#endif // LOGGING_H_INCLUDED
