#define WinD
#include "GfxDeviceInterface+Mng.h"
#include "WinDef.h"
#include <algorithm>


namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>

ULONG_PTR GDIpManager::token = NULL;
int GDIpManager::nReference = NULL;

namespace gdi = Gdiplus;
GDIpManager::GDIpManager()
{
	if (nReference++ == 0)
	{
		Gdiplus::GdiplusStartupInput input; // initialise input

		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&token, &input, nullptr);
	}
}

GDIpManager::~GDIpManager()
{
	if (--nReference == 0)
	{
		Gdiplus::GdiplusShutdown(token);
	}
}
