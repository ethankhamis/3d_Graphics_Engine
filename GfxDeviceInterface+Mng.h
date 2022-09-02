#pragma once
#include "WinDef.h"

struct GDIpManager
{
	GDIpManager();
	~GDIpManager();
private:
	static int nReference;
	static unsigned long token;
};