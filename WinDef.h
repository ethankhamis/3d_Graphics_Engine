#pragma once
//#define _WIN32_WINNT 1532
#define NTDDI_VERSION NTDDI_VISTA 
#define _WIN32_WINNT _WIN32_WINNT_VISTA 
/* ASSURES WINDOWS 7+ AS TARGET */
#include <sdkddkver.h> //sdk version
/* THIS CODE IS IMPLEMENTED TO SPEED UP BUILD-TIME */
/* IT ALSO ENSURES NO NAMESPACES WITHIN C++ ARE OVERWRITTEN */

#include <Windows.h>

#ifndef WinD
#define NODRAWTEXT
#define NOPROFILER 
#define NOGDICAPMASKS
#define NOATOM
#define NOICONS
#define NOSYSCOMMANDS
#define NOIMAGE
#define WIN32_LEAN_AND_MEAN
#define NOMETAFILE
#define NORASTEROPS
#define NOCOLOR
#define NOKERNEL
#define NOMENUS
#define NOCTLMGR
#define NOSYSMETRICS
#define NODEFERWINDOWPOS
#define NOSCROLL
#define NOSOUND
#define NOOPENFILE
#define NOTEXTMETRIC
#define OEMRESOURCE
#define NOWH
#define NOMEMMGR
#define NOCOMM
#define NOHELP
#define NOSERVICE
#define NOCLIPBOARD
#define NOMCX
#define NOKANJI
#define NORPC
#define NONLS
#define NOPROXYSTUB
#define NOTAPE
#endif

#define NOMINMAX
#define STRICT