///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : ZString.cpp
// Creation : 07/07/2001
// Author : Cedric Guillemet
// Description :
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
// Includes ///////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZString.h"
//#include "ZFile.h"
#ifdef WIN32
#include <direct.h>
#include <shlobj.h>
#endif


// Globals ////////////////////////////////////////////////////////////////////////////////////////

tstring::tstring( int i)
{
    stringData = NULL;
    length     = 0;
    Printf("%d",i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// char * + string

const tstring    operator +    (const tchar * chaine, const tstring & str)
{

    tstring tmpS;

    tmpS.length = 0;

    if(str.stringData)
    {
        tmpS.length = str.length;
    }

    if(chaine)
    {
        tmpS.length += (tulong)strlen(chaine);
    }

    tmpS.stringData = (tchar *)malloc((tmpS.length + 1) * sizeof(tchar));
    tmpS.stringData[0] = 0;

    if(chaine)
    {
        strcpy(tmpS.stringData, chaine);
    }

    if(str.stringData)
    {
        strcat(tmpS.stringData, str.stringData);
    }

    return tmpS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//! @param path        the file path.
//! @param filename    the file name.
//! @param file        a file pointer. If NULL the function open the file "path + filename".
//! @return                PSM_OK if the function succeed, PSM_ERR if it fail.
/*
void tstring::LoadFile(ZFile & file)
{
    file.Read(&length, sizeof(length));

    if(length > 0)
    {
        if(stringData)
        {
            stringData = (tchar *)realloc(stringData, (length + 1) * sizeof(tchar));
        }
        else
        {
            stringData = (tchar *)malloc((length + 1) * sizeof(tchar));
        }

        file.Read(stringData, length);

        stringData[length]= '\0';
    }
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////

void tstring::LoadMem(char **pBuf)
{
    //file.Read(&length, sizeof(length));
	length = *(tulong*)(*pBuf);
	(*pBuf) += sizeof(tulong);

    if(length > 0)
    {
        if(stringData)
        {
            stringData = (tchar *)realloc(stringData, (length + 1) * sizeof(tchar));
        }
        else
        {
            stringData = (tchar *)malloc((length + 1) * sizeof(tchar));
        }

        //file.Read(stringData, length);
		memcpy(stringData, *pBuf, length);

        stringData[length]= '\0';
		*pBuf += length;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
void tstring::LoadFileZeroed(ZFile & file)
{
    char temp[1024];
    int relIdx = 0;

    while(1)
    {
        file.Read(&temp[relIdx], 1);
        if (temp[relIdx] == 0) break;

        relIdx ++;

        if (relIdx == 1023)
        {
            temp[relIdx] = 0;
            *this += temp;
            relIdx = 0;
        }
    };

    if (relIdx)
    {
        temp[relIdx] = 0;
        *this += temp;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//! @param file        a file pointer. If NULL the function open the file "path + filename".
//! @return                PSM_OK if the function succeed
//!                        PSM_ERR if it fail.

void tstring::SaveFile(ZFile & file) const
{
    file.Write(&length, sizeof(length));
    if(length > 0)
    {
        file.Write(stringData, length);
    }
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
// Replace /./ and /../ for use in big files

void tstring::SimplifyPath()
{
	char npath[MAX_PATH+1];
	uint curc = 0;
	int av = 0;

	while(curc<length)
	{
		if (stringData[curc] == '.')
		{
			if ( (stringData[curc+1] == '/') || (stringData[curc+1] == '\\') )
			{
				// don't do anything
				curc += 2;
				if ( (stringData[curc] == '/') || (stringData[curc] == '\\') )
					curc++;
			}
			else if (stringData[curc+1] == '.')
			{
				if ( (stringData[curc+2] == '/') || (stringData[curc+2] == '\\') )
				{
					// should get back

					curc += 3;
					av-=2;
					while(npath[av] != '/' && npath[av] != '\\' && av>0)
					{
						av --;
					}
					if (av>0)
						av++;

				}
			}
			else
			{
				// . is part of the name -> extension
				npath[av++] = stringData[curc++];
			}
		}
		else
		{
			npath[av++] = stringData[curc++];
		}
	}
	npath[av] = 0;

	*this = npath;

	ToUpper();
	Replace("\\","/");
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void tstring::ComputeRelativeWD()
{
	tstring wp;
	char tmps[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, tmps);//_getcwd(NULL, 0);
	wp = tmps;
	tstring newp = "./";
	newp += &this->stringData[wp.Length()+1];
	*this = newp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
const char *tstring::GetScreenShotDirectory()
{
	return GetPictureDirectoy();
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
