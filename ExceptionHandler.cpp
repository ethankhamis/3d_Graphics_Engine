#include "ExceptionHandler.h"
#include <sstream>
#include <algorithm>

ExceptionHandler::ExceptionHandler( unsigned int curLine, const wchar_t* fName) noexcept
	:
	curLine(curLine),
	fName(fName) {}

const wchar_t* ExceptionHandler::whatw() const noexcept
{
	std::wstringstream wss;


	wss 
		<< FetchErrorType()
		<< std::endl <<
		FetchErrorWString();
	buffer_w = wss.str().c_str();

	return buffer_w.c_str();
}

const wchar_t* ExceptionHandler::FetchErrorType() const noexcept
{
	return L"Exception Caught";
}

const std::wstring& ExceptionHandler::FetchFileName() const noexcept
{
	return fName;
}

unsigned int ExceptionHandler::FetchLine() const noexcept
{
	return curLine;
}

std::wstring ExceptionHandler::FetchErrorWString() const noexcept
{
	std::wstringstream wss;

	wss
		<< L"{File: " << fName << L" }" 
		<< std::endl
		<< L"{Line: " << curLine << L" }";
	return wss.str();

}
