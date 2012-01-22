#ifndef __INC_SHAREHEADERS_H__
#define __INC_SHAREHEADERS_H__
#define WINVER 0x0501 //0x0501=Windows XP
#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x0501
//#define _WIN32_IE 0x0600 //IE”š”­‚µ‚ë
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include <cstring>
#include <ctime>
#include <cstdlib>

#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <GdiPlus.h>
#include <WinInet.h> 
#include <CommCtrl.h>

#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Gdiplus.lib")
#pragma comment(lib,"WinInet.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#endif