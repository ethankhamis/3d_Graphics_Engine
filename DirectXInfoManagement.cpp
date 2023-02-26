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
	// taken from
	// https://github.com/FFmpeg/FFmpeg/blob/master/libavutil/hwcontext_d3d11va.c
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// call LoadLibraryExW on the dll storing the DXGIGetDebugInterface function
	const HMODULE hModDxgiDebug = LoadLibraryExW(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr){throw THROW_WINDOW_LAST_EXCEPTION();}
	// fetch address of DXGIGetDebugInterface in dll
	const DXGIGetDebugInterface DxgiGetDebugInterface = 
		reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr){throw THROW_WINDOW_LAST_EXCEPTION();}
	HRESULT hr;
	// store info using the debug interface
	THROW_GFX_EXCEPTION_NO_DXINFOMANAGER(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pIDxGfxInfoQueue));
	
}

void DxGfxInfoMng::Apply() noexcept
{
	// store the messages using GUID type DXGI_DEBUG_ALL
	nextMsg = pIDxGfxInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxGfxInfoMng::FetchMessages() const
{
	std::vector<std::string> messages;

	const auto end = pIDxGfxInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	// loop first messagee to last message
	for (auto i = nextMsg; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		// get size of message i in bytes
		THROW_GFX_EXCEPTION_NO_DXINFOMANAGER(pIDxGfxInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// allocate memory for message
		std::unique_ptr<byte[], std::default_delete<byte[]>> bytes = std::make_unique<byte[]>(messageLength);
		DXGI_INFO_QUEUE_MESSAGE* pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message
		THROW_GFX_EXCEPTION_NO_DXINFOMANAGER(pIDxGfxInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		//emplace description into std::vector of messages
		messages.emplace_back(pMessage->pDescription);
	}
	//return vector of messages
	return messages;

}