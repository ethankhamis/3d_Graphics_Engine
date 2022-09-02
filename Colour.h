#pragma once
struct Colour
{
	unsigned int dword;

	Colour() noexcept : dword(){}
	
	constexpr Colour(const Colour& colour) noexcept
		:dword(colour.dword){}

	constexpr Colour(unsigned int dword) noexcept
		:dword(dword){}

	constexpr Colour (unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept
		: dword( (x<<24u)|(r<<16u)|(g<<8u)|b ){}

	constexpr Colour(Colour c, unsigned char x) noexcept
		: dword( x << 24u | c.dword){}

	Colour& operator= (Colour c) noexcept
	{
		dword = c.dword;
		return *this;
	}

	constexpr unsigned char FetchX() const noexcept
	{
		return dword >> 24u;
	}

	constexpr unsigned char FetchA() const noexcept
	{
		return FetchX();
	}

	constexpr unsigned char FetchR() const noexcept
	{
		return (dword >> 8u) & 255u;
	}

	constexpr unsigned char FetchB() const noexcept
	{
		return dword & 255u;
	}

	void ApplyX(unsigned char x) noexcept
	{
		dword = (dword & 16777215u) | (x << 24u);
	}

	void ApplyA(unsigned char a) noexcept
	{
		ApplyX(a);
	}

	void ApplyR(unsigned char r) noexcept
	{
		dword = (dword & 4278255615u) | (r << 16u);
	}

	void ApplyG(unsigned char g) noexcept
	{
		dword = (dword & 4294902015u) | (g << 8u);
	}

	void ApplyB(unsigned char b) noexcept
	{
		dword = (dword & 4294967040u) | b;
	}
};