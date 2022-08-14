#include "ExceptionHandler.h"
#include <sstream>

ExceptionHandler::ExceptionHandler(const char* fName, unsigned int curLine) noexcept
	:
	curLine(curLine),
	fName(fName) {}

const char* ExceptionHandler::what() const noexcept
{
	std::ostringstream oss;

	oss 
		<< FetchErrorType()
		<< std::endl <<
		FetchStartString();
	buffer_t = oss.str().c_str(); 
	return oss.str().c_str(); /*as 'return buffer_t' doesnt work*/
}

const char* ExceptionHandler::FetchErrorType() const noexcept
{
	return "Exception Caught";
}

const std::string& ExceptionHandler::FetchFileName() const noexcept
{
	return fName;
}

unsigned int ExceptionHandler::FetchLine() const noexcept
{
	return curLine;
}

std::string ExceptionHandler::FetchStartString() const noexcept
{
	std::ostringstream oss;

	oss
		<< "{File: " << fName << " }" 
		<< std::endl
		<< "{Line: " << curLine;
	return oss.str();

}
