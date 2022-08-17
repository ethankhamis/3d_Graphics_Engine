#pragma once
#include <string>
#include <exception>

class ExceptionHandler : public std::exception
{
private:
	unsigned int curLine;
	std::wstring fName; 
public:
	ExceptionHandler( unsigned int curLine, const wchar_t* fName) noexcept;
	virtual const wchar_t* whatw() const noexcept;
	virtual const wchar_t* FetchErrorType() const noexcept;
	const  std::wstring& FetchFileName() const noexcept;
	unsigned int FetchLine() const noexcept;
	std::wstring FetchErrorWString() const noexcept;

protected:
	mutable std::wstring buffer_w;
};