#pragma once
#include <string>
#include <exception>

class ExceptionHandler : protected std::exception
{
private:
	unsigned int curLine;
	std::string fName; 
public:
	ExceptionHandler(const char* fName, unsigned int curLine) noexcept;
	const char* what() const noexcept override; //override std::exception's 'what()' func
	virtual const char* FetchErrorType() const noexcept;
	const  std::string& FetchFileName() const noexcept;
	unsigned int FetchLine() const noexcept;
	std::string FetchStartString() const noexcept;
protected:
	mutable std::string buffer_t;
};