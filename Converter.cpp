#include "Converter.h"

std::wstring convert::make_wstring(const char* s)
{
	std::string  cs(s);
	std::wstring ws;
	copy(cs.begin(), cs.end(), back_inserter(ws));
	return ws;
}

std::string convert::make_string(const std::wstring ws)
{
	std::wstring  wide_s(ws);
	std::string s;
	copy(s.begin(), s.end(), back_inserter(wide_s));
	return s;
}
