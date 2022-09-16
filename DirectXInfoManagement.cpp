#pragma once

#include "DirectXInfoManagement.h"
#include "Win.h"
#include "Graphics.h"
#include <dxgidebug.h>
#include "ThrowMacros.h"
#include <memory>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma comment(lib, "dxguid.lib")


DxGfxInfoMng::DxGfxInfoMng()
{
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryExW(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw EHWND_LAST_EXCEPT();
	}
	
	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
	{
		throw EHWND_LAST_EXCEPT();
	}
	
	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pIDxGfxInfoQueue));
	
}

void DxGfxInfoMng::Apply() noexcept
{
	nextMsg = pIDxGfxInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxGfxInfoMng::FetchMessages() const
{
	std::vector<std::string> messages;


	const auto end = pIDxGfxInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = nextMsg; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		// get size of message i in bytes
		GFX_THROW_NOINFO(pIDxGfxInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// allocate memory for message
		std::unique_ptr bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push description into std::vector
		GFX_THROW_NOINFO(pIDxGfxInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, pMessage, &messageLength));

		messages.emplace_back(pMessage->pDescription);
	}

	return messages;

}