#ifndef _DUILIB_H__
#define _DUILIB_H__

#include <UIlib.h>
using namespace DuiLib;

#ifdef _DEBUG
#	ifdef _UNICODE
#		pragma comment(lib, "DuiLib_ud.lib")
#	else
#		pragma comment(lib, "DuiLib_d.lib")
#	endif
#else
#	ifdef _UNICODE
#		pragma comment(lib, "DuiLib_u.lib")
#	else
#		pragma comment(lib, "DuiLib.lib")
#	endif
#endif

#endif /*_DUILIB_H__*/