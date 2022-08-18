#pragma once
#include "WinDef.h"
#include <wrl.h>
#include <dxgidebug.h>
#include <vector>
#include <string>
#define NULLUNSIGNED 0u

struct DxGfxInfoMng
{
	DxGfxInfoMng();
	~DxGfxInfoMng() = default;
	DxGfxInfoMng(const DxGfxInfoMng&) = delete;
	DxGfxInfoMng& operator=(const DxGfxInfoMng&) = delete;
public:
	std::vector<std::string> FetchMessages() const;
	void Apply() noexcept;
private:
	unsigned long long nextMsg = NULLUNSIGNED;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pIDxGfxInfoQueue;

};