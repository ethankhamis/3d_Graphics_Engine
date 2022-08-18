#pragma once
//#define _WIN32_WINNT 1532
#define NTDDI_VERSION NTDDI_VISTA 
#define _WIN32_WINNT _WIN32_WINNT_VISTA 

/* ASSURES WINDOWS 7+ AS TARGET */

#include <sdkddkver.h> //sdk version
#include <Windows.h>


/* THIS CODE IS IMPLEMENTED TO SPEED UP BUILD-TIME */
/* IT ALSO ENSURES NO NAMESPACES WITHIN C++ ARE OVERWRITTEN */

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
#define NOMINMAX 
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
#define STRICT
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
