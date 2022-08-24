#pragma once

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
//
// macro specifically for importing infomanager into local scope
// this.GetInfoManager(Graphics& gfx) must exist
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; DxGfxInfoMng& infomanager = FetchInfoManager((gfx))
#endif

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
