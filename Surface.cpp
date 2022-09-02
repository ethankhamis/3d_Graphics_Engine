#define WinD
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#include "Surface.h"
#include <algorithm>
#include "WinDef.h"
namespace Gdiplus
{
    using std::min;
    using std::max;
}
#include <gdiplus.h>
#include <memory>
#include <sstream>

namespace gdp = Gdiplus; 

#pragma comment(lib, "gdiplus.lib")

Surface::Exception::Exception(unsigned int line, const wchar_t* file, std::wstring note) noexcept
    :
    ExceptionHandler(line, file),
    note(std::move(note))
{
}

const wchar_t* Surface::Exception::whatw() const noexcept
{
    std::wostringstream woss;
    woss << ExceptionHandler::whatw() << std::endl
        << L"[Note] " << FetchNote();
    buffer_w = woss.str();
    return buffer_w.c_str();
}

const wchar_t* Surface::Exception::FetchErrorType() const noexcept
{
    return L"Graphics Exception";
}

const std::wstring& Surface::Exception::FetchNote() const noexcept
{
    return note;
}

Surface::Surface(unsigned int w, unsigned int h) noexcept
    :
    pBuffer(std::make_unique<Colour[]>(w * h)),
    width(w),
    height(h)
{
}

Surface::Surface(Surface&& src) noexcept
    :
    pBuffer(std::move(src.pBuffer)),
    width(src.width),
    height(src.height)
{
}

Surface& Surface::operator=(Surface&& donor) noexcept
{
    width = donor.width;
    height = donor.height;
    pBuffer = std::move(donor.pBuffer);
    donor.pBuffer = nullptr;
    return *this;
}

Surface::~Surface()
{}

void Surface::Wipe(Colour fillValue) noexcept
{
    memset(pBuffer.get(), fillValue.dword, width * height * sizeof(Colour));
}

void Surface::ApplyPixel(unsigned int x, unsigned int y, Colour c) noexcept(!Debug)
{
    gfxAsserts(x,y);
    pBuffer[y * width + x] = c;
}

Colour Surface::FetchPixel(unsigned int x, unsigned int y) const noexcept(!Debug)
{
    gfxAsserts(x, y);
    return pBuffer[y * width + x];
}

unsigned int Surface::FetchWidth() const noexcept
{
    return width;
}

unsigned int Surface::FetchHeight() const noexcept
{
    return height;
}

Colour* Surface::FetchpBuffer() noexcept
{
    return pBuffer.get();
}


const Colour* Surface::FetchpBuffer() const noexcept
{
    return pBuffer.get();
}

const Colour* Surface::FetchpBufferConst() const noexcept
{
    return pBuffer.get();
}

Surface Surface::WithFile(const std::wstring& filename)
{
    UINT w = NULL;
    UINT h = NULL;
    UINT pitch = NULL;
    std::unique_ptr<Colour[]> pBuffer = nullptr;

    {
        gdp::Bitmap bitmap(filename.c_str());
        if (bitmap.GetLastStatus() != gdp::Status::Ok)
        {
            std::wstringstream wss;
            wss << L"Loading image {" << filename << L"}: has failed to load.";
            throw Exception(__LINE__, WFILE, wss.str());
        }

        h = bitmap.GetHeight();
        pBuffer = std::make_unique<Colour[]>(w * h);

        for (UINT y = 0; y < h; y++)
        {
            for (UINT x = 0; x < w; x++)
            {
                gdp::Color c;
                bitmap.GetPixel(x, y, &c);
                pBuffer[y * pitch + x] = c.GetValue();
            }
        }
    }
    return Surface(w,h,std::move(pBuffer));
}

void Surface::SaveFile(const std::wstring& filename) const
{

    auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void
    {
        unsigned int n = NULL;          // number of image encoders
        unsigned int size = NULL;       // size of the image encoder array in bytes

        gdp::ImageCodecInfo* pImageCodecInfo = nullptr;

        gdp::GetImageEncodersSize(&n, &size);
        if (size == NULL)
        {
            std::wstringstream wss;
            wss << L"Saving surface to [" << filename << L"]: failed to get encoder; size == 0.";
            throw Exception(__LINE__, WFILE, wss.str());
        }

        pImageCodecInfo = (gdp::ImageCodecInfo*)(malloc(size));
        if (pImageCodecInfo == nullptr)
        {
            std::wstringstream wss;
            wss << L"Saving surface to [" << filename << L"]: failed to get encoder; failed to allocate memory.";
            throw Exception(__LINE__, WFILE, wss.str());
        }

        GetImageEncoders(n, size, pImageCodecInfo);

        for (UINT j = NULL; j < n; ++j)
        {
            if (wcscmp(pImageCodecInfo[j].MimeType, format) == NULL)
            {
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return;
            }
        }

        free(pImageCodecInfo);
        std::wstringstream wss;
        wss << L"Saving surface to [" << filename <<
            L"]: failed to get encoder; failed to find matching encoder.";
        throw Exception(__LINE__, WFILE, wss.str());
    };

    CLSID bmpID;
    GetEncoderClsid(L"image/bmp", &bmpID);


    gdp::Bitmap bitmap(width, height, width * sizeof(Colour), PixelFormat32bppARGB, (BYTE*)pBuffer.get());
    if (bitmap.Save(filename.c_str(), &bmpID, nullptr) != gdp::Status::Ok)
    {
        std::wstringstream wss;
        wss << L"Saving surface to [" << filename << L"]: failed to save.";
        throw Exception(__LINE__, WFILE, wss.str());
    }return;
}

void Surface::Copy(const Surface& src) noexcept(!Debug)
{
    assert(width == src.width);
    assert(height == src.height);
    memcpy
    (
        pBuffer.get(),
        src.pBuffer.get(),
        width * height *
        sizeof(Colour)
    );
}

void Surface::gfxAsserts(unsigned int x, unsigned int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < width);
    assert(y < height);
}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Colour[]> pBufferParam) noexcept
:
    width(width),
    height(height),
    pBuffer(std::move(pBufferParam))
{
}
