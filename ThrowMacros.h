#pragma once
//macro specifically for using wide-string macro
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
// macro specifically for importing infomanager into local scope
#ifdef NDEBUG
#define DEF_INFOMANAGER(gfx) HRESULT hr
#else
#define DEF_INFOMANAGER(gfx) HRESULT hr; DxGfxInfoMng& infomanager = FetchInfoManager((gfx))
#endif
//macro for handling graphics exceptions depending on whether in debug mode
#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HResultException( __LINE__,WFILE,(hr),infomanager.FetchMessages() )
#define GFX_THROW_INFO(hrcall) infomanager.Apply(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,WFILE,(hr),infomanager.FetchMessages() )
#define GFX_THROW_INFO_ONLY(call) infomanager.Apply(); (call); {auto m = infomanager.FetchMessages(); if(!m.empty()) {throw Graphics::InfoException( __LINE__,WFILE,m);}}
#else
#define GFX_EXCEPT(hr) Graphics::HResultException( __LINE__,WFILE,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,WFILE,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif
// HRESULT must exist in scope macros to work
#define GFX_EXCEPT_NOINFO(hr) Graphics::HResultException( __LINE__,WFILE,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HResultException( __LINE__,WFILE,hr )

// macro for throwing windows exceptions
#define WINDOW_EXCEPT( hr ) Wnd::HResultException( __LINE__,WFILE,(hr) )
#define WINDOW_LAST_EXCEPT() Wnd::HResultException( __LINE__,WFILE,GetLastError() )
#define WINDOW_NOGFX_EXCEPT() Wnd::NoGfxException(__LINE__, WFILE)