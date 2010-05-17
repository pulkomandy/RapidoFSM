///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : ZBaseDefs.h
// Creation : 13/06/2007
// Author : Cedric Guillemet
// Description :
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZBASEDEFS_H__
#define ZBASEDEFS_H__

#ifdef WIN32
#include <windows.h>
#include <stdlib.h>
#endif


typedef unsigned long tulong;
typedef long tlong;
typedef double tdouble;

typedef char tchar;
typedef unsigned char tuchar;

typedef short tshort;
typedef unsigned short tushort;

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;

typedef unsigned short TClassID;

typedef unsigned int uint;

#define    FLOAT_EPSILON    float(1.192092896e-07)    // Smallest positive number x, such that x+1.0 is not equal to 1.0
const float        RealEpsilon=FLOAT_EPSILON*16.f;


#define ZMAX(a,b)            (((a) > (b)) ? (a) : (b))
#define ZMIN(a,b)            (((a) < (b)) ? (a) : (b))

#include <vector>
#include <list>
#include <map>
#include <string>

#define tarray std::vector
#define tlist std::list


#define foreach( i, c, type )\
  if (!c.empty()) for( std::vector<type>::iterator i = c.begin(); i != c.end(); ++i )

#define foreach_const( i, c, type )\
  if (!c.empty()) for( std::vector<type>::const_iterator i = c.begin(); i != c.end(); ++i )

#define foreachmap( i, c, type )\
  if (!c.empty()) for( std::map<type>::iterator i = c.begin(); i != c.end(); ++i )

#define foreachmap_const( i, c, type )\
  if (!c.empty()) for( std::map<type>::const_iterator i = c.begin(); i != c.end(); ++i )

void Zexit(int aRet);


#ifdef WIN32
typedef CRITICAL_SECTION ZCriticalSection_t;
typedef HWND WindowHandle_t;
typedef HANDLE ThreadHandle_t;

inline char* GetCurrentDirectory(int bufLength, char *pszDest)
{
    return (char*)GetCurrentDirectoryA(bufLength, pszDest);
}

#endif

#ifdef LINUX
#include <pthread.h>

typedef pthread_mutex_t ZCriticalSection_t;
typedef int WindowHandle_t;
typedef pthread_t ThreadHandle_t;

#define TRUE 1
#define FALSE 0

typedef struct RECT
{
    int left, right, bottom, top;
} RECT;


#define _MAX_PATH 256
#define MAX_PATH _MAX_PATH

#define stricmp strcasecmp
#define _stricmp strcasecmp
#define sscanf_s sscanf
#define sprintf_s snprintf
#define OutputDebugString printf
#define OutputDebugStringA printf

inline void strlwr(char *)
{
}
#include<iostream>

#include<iomanip>

#include<fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

inline char* GetCurrentDirectory(int bufLength, char *pszDest)
{
    return getcwd(pszDest, bufLength);
}

inline void ZeroMemory(void *pDest, int aSize)
{
    memset(pDest, 0, aSize);
}

inline unsigned long GetCurrentThreadId()
{
    return (unsigned long) pthread_self();//getpid();
}

#endif

#ifdef MAC_OS
#import <CoreServices/CoreServices.h>

typedef MPCriticalRegionID ZCriticalSection_t;
typedef int WindowHandle_t;
typedef pthread_t ThreadHandle_t;

#define TRUE 1
#define FALSE 0

typedef struct RECT
	{
		int left, right, bottom, top;
	} RECT;


#define _MAX_PATH 256
#define MAX_PATH _MAX_PATH

#define stricmp strcasecmp
#define _stricmp strcasecmp
#define sscanf_s sscanf
#define sprintf_s snprintf
#define OutputDebugString printf
#define OutputDebugStringA printf

inline void strlwr(char *pszBuf)
{
}
#include<iostream>

#include<iomanip>

#include<fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

inline char* GetCurrentDirectory(int bufLength, char *pszDest)
{
    return getcwd(pszDest, bufLength);
}

inline void ZeroMemory(void *pDest, int aSize)
{
    memset(pDest, 0, aSize);
}

inline unsigned long GetCurrentThreadId()
{
    return (unsigned long) pthread_self();//getpid();
}

// DX compatibility
typedef void IDirect3DDevice9;
typedef void IDirect3DTexture9;



#endif

#endif
