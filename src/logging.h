#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

const std::string getCurrentDateTime();
void FatalError(std::string ErrorText);
void ErrorOccured(std::string ErrorText);
void Exitting();

#endif // LOGGING_H_INCLUDED
