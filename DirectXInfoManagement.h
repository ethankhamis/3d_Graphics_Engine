#pragma once
#include "WinDef.h"
#include <vector>
#include <string>
#define NULLUNSIGNED 0u

struct DxGfxInfoMng
{
	DxGfxInfoMng();
	~DxGfxInfoMng();
	DxGfxInfoMng(const DxGfxInfoMng&) = delete;
	DxGfxInfoMng& operator=(const DxGfxInfoMng&) = delete;
public:
	std::vector<std::wstring> FetchMessages() const;
	void Apply() noexcept;
private:
	unsigned long long nextMsg = NULLUNSIGNED;
	struct IDXGIInfoQueue* pIDxGfxInfoQueue = nullptr;

};