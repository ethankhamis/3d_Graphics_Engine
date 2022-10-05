#pragma once
#include "WinDef.h"
#include <string>
#include <assert.h>
#include <memory>
#include "ExceptionHandler.h"
#include "Colour.h"
#include "debugdefs.h"

struct Surface
{
	struct Exception : public ExceptionHandler
	{
		Exception(unsigned int line, const wchar_t* file, std::wstring note) noexcept;
		const wchar_t* whatw() const noexcept override;
		const wchar_t* FetchErrorType() const noexcept override;
		const std::wstring& FetchNote() const noexcept;
	private:
		std::wstring note;
	};
public:
	Surface(unsigned int w, unsigned int h) noexcept;
	Surface(Surface&& src) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& donor) noexcept;
	Surface& operator=(const Surface&) = delete;
	~Surface();
public:
	void Wipe(Colour fillValue) noexcept;
	void ApplyPixel(unsigned int x, unsigned int y, Colour c) noexcept_unless;
	Colour FetchPixel(unsigned int x, unsigned int y) const noexcept_unless;
	unsigned int FetchWidth() const noexcept;
	unsigned int FetchHeight() const noexcept;
	Colour* FetchpBuffer() noexcept;
	const Colour* FetchpBuffer() const noexcept;
	const Colour* FetchpBufferConst() const noexcept;
	static Surface WithFile(const std::wstring& filename);
	void SaveFile(const std::wstring& filename) const;
	void Copy(const Surface& src) noexcept_unless;
private:
	void gfxAsserts(unsigned int x, unsigned int y) const;
private:
	Surface(unsigned int width, unsigned int height, std::unique_ptr<Colour[]> pBufferParam) noexcept;
private:
	std::unique_ptr<Colour[]> pBuffer;
	unsigned int width;
	unsigned int height;
};