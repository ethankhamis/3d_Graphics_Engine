
#include <string>
#include <iterator>
#include <algorithm>

namespace convert
{
	std::wstring make_wstring(const char* s);
	std::string make_string(const std::wstring ws);
};
