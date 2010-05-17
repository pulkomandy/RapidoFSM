///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : ZString.h
// Creation : 07/07/2001
// Author : Cedric Guillemet
// Description : String Class
// Reliability : 8/10
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

#ifndef ZSTRING_H__
#define ZSTRING_H__

// Includes ///////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "ZBaseDefs.h"
#include <memory.h>
#include <stdlib.h>



inline void strcpy_s(char *dst, int dslLength, const char *src)
{
    strcpy(dst, src);
}

inline void strcat_s(char *dst, int dslLength, const char *src)
{
    strcat(dst, src);
}
 inline void _splitpath_s(const char *szBuf, char *szDrive, int aDrive, char *szPath, int aPath, char *szFile, int aFile, char *szExt, int aExt)
 {
     //_splitpath(szBuf, szDrive, szPath, szFile, szExt);
 }
///////////////////////////////////////////////////////////////////////////////////////////////////

class ZFile;

// Class //////////////////////////////////////////////////////////////////////////////////////////
//! String class.
//! Use this class to manipulate an array of characters.

class    tstring
{
    public:
        tchar * stringData;        //!< The array of characters
        tulong  length;            //!< The length of the string

    public:
        tstring()
        {
            stringData = NULL;
            length     = 0;
        }

        tstring(const tstring &str)
        {
            if(str.stringData)
            {
                length = (tulong)strlen(str.stringData);
                stringData = (tchar *)malloc(((length + 1) * sizeof(tchar)));

                memcpy(stringData, str.stringData, length);
                stringData[length] = '\0';
            }
            else
            {
                stringData = (tchar *)malloc(sizeof(tchar));

                length     = 0;
                stringData[length] = '\0';
            }

        }
        tstring(const tchar *str)
        {
            if(str)
            {
                length = (tulong)strlen(str);
                stringData = (tchar *)malloc((length + 1) * sizeof(tchar));

                memcpy(stringData, str, length);
                stringData[length] = '\0';
            }
            else
            {
                stringData = (tchar *)malloc(sizeof(tchar));

                length     = 0;
                stringData[length] = '\0';
            }
        }

        tstring(int);

        tstring(char *start, uint alength)
        {
            length = alength;
            stringData = (tchar *)malloc((length + 1) * sizeof(tchar));
            memcpy(stringData, start, length);
            stringData[length] = '\0';
        }

        virtual ~tstring()
        {
            if(stringData)
				free(stringData);
        }
        const tchar* c_str() const
        {
            return stringData;
        }

        bool Empty() const { return ((stringData==NULL)||(length==0)); }
        // delete the last characters
        void BackDrop();
        // copy operators
        inline void operator = ( const tstring & str );
        inline void operator = ( const char * str );

        friend const tstring    operator +    (const tchar *, const tstring&);

        // file name operations
        tstring GetExtension() const
        {
            //char drive[_MAX_DRIVE];
	    //char dir[_MAX_DIR];
	    //char file[_MAX_PATH];
	    char ext[_MAX_PATH];

	    _splitpath_s(stringData, NULL, 0, NULL, 0, NULL, 0, ext, _MAX_PATH);
            return ext;
        }

        tstring GetBaseName() const
        {
            char filen[_MAX_PATH];
		    //char ext[_MAX_PATH];

    _splitpath_s(stringData, NULL, 0, NULL, 0, filen, _MAX_PATH, NULL, 0);
            //tstring basename = filen;
            //basename += ext;
            return filen;

        }

        tstring GetFileName() const
        {
            char filen[_MAX_PATH];
		    char ext[_MAX_PATH];

		    _splitpath_s(stringData, NULL, 0, NULL, 0, filen, _MAX_PATH, ext, _MAX_PATH);
            tstring basename = filen;
            basename += ext;
            return basename;
        }

        tstring GetPath() const
        {
            char path[_MAX_PATH];
		    //char ext[_MAX_PATH];

		    _splitpath_s(stringData, NULL, 0, path, _MAX_PATH, NULL, 0, NULL, 0);
            //tstring basename = filen;
            //basename += ext;
            return path;

        }
        /*


        // binary operators
        void    operator += (const tstring &);
        void    operator -= (const tstring &);
        tstring    operator -    (const tstring &) const;
        tstring    operator +    (const tstring &) const;
        tstring    operator +    (const tchar *str) const;



        void    operator +=    (const tchar    *);
        void    operator +=    (const tchar    );
        tstring operator -    (const tchar    *) const;

        // Comparaison operators


*/
        inline bool    operator == (const tstring &) const;
        inline bool    operator == (const tchar    *) const;
        inline bool    operator != (const tstring &) const;
        inline bool    operator <  (const tstring &) const;
        inline bool    operator >  (const tstring &) const;

        inline bool    operator != (const tchar    *) const;
        inline bool    operator <  (const tchar    *) const;
        inline bool    operator >  (const tchar    *) const;

        inline operator char*();
        inline operator const char*() const;

        inline tchar    &operator[](uint16 i){return stringData[i];}
      inline tchar    GetCar(uint16 i){ return stringData[i]; }
      inline tchar    GetCar(uint32 i){ return stringData[i]; }

	  static const char *GetScreenShotDirectory();

	  void ComputeRelativeWD();
		void SimplifyPath();
        tstring operator + ( const tstring & str ) const
        {
            tstring tmpS;

            tmpS.length = 0;

            if(stringData)
            {
                tmpS.length = length;
            }

            if(str.stringData)
            {
                tmpS.length += str.length;
            }

            tmpS.stringData = (tchar *)malloc((tmpS.length + 1) * sizeof(tchar));
            tmpS.stringData[0] = '\0';

            if(stringData)
            {
                strncpy(tmpS.stringData, stringData, (tmpS.length + 1) );
            }

            if(str.stringData)
            {
                strncat(tmpS.stringData, str.stringData, (tmpS.length + 1));
            }

            return tmpS;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // tstring + char *

        tstring operator + ( const char * str ) const
        {
            tstring tmpS;

            tmpS.length = 0;

            if(stringData)
            {
                tmpS.length = length;
            }

            if(str)
            {
                tmpS.length += (tulong)strlen(str);
            }

            tmpS.stringData = (tchar *)malloc((tmpS.length + 1) * sizeof(tchar));
            tmpS.stringData[0] = '\0';

            if(stringData)
            {
                strncpy(tmpS.stringData, stringData, (tmpS.length + 1));
            }

            if(str)
            {
                strncat(tmpS.stringData, str, (tmpS.length + 1));
            }

            return tmpS;

        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // char * + string
/*
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
                tmpS.length += strlen(chaine);
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
*/
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // this += car

        void operator += ( const tchar car )
        {
            if(stringData)
            {
                stringData = (tchar *)realloc(stringData, (length + 2) * sizeof(tchar));
            }
            else
            {
                stringData = (tchar *)malloc(2 * sizeof(tchar));
            }

            stringData[length] = car;
            length++;
            stringData[length]= '\0';
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // this += tchar *

        void operator += ( const tchar * str )
        {
            if(str)
            {
				int len1;
                if(stringData)
                {
					len1 = ( length + strlen(str) + 1);
                    stringData = (tchar *)realloc(stringData, len1 * sizeof(tchar));
                }
                else
                {
					len1 = ( (unsigned int)strlen(str) + 1);
                    stringData = (tchar *)malloc( len1 * (unsigned int)sizeof(tchar));
                }

                stringData[length]= '\0';
                length += (tulong)strlen(str);
                strncat(stringData, str, len1);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // this += tstring

        void operator += ( const tstring & str )
        {
			this->operator +=(str.c_str());
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////

        tstring operator - ( const tstring & str ) const
        {
            tstring tmpS;

            tmpS.length = 0;

            if(stringData)
            {
                tmpS.length = str.length;
            }

            tmpS.stringData = (tchar *) malloc((length + 1) * sizeof(tchar));
            stringData[0]= '\0';

            if(str.stringData && stringData)
            {
                tulong pos;
                strncpy(tmpS.stringData, stringData, (length + 1));

                while(tmpS.Find(str,&pos))
                {
                    for(tulong i= pos ; i< tmpS.length-str.length ; i++)
                    {
                        tmpS.stringData[i] = tmpS.stringData[i+str.length];
                    }
                    tmpS.length -= str.length;
                }

                tmpS.stringData[tmpS.length]='\0';
            }

            return tmpS;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////

        tstring operator - ( const char * str ) const
        {
            tstring tmpS;

            tmpS.length = 0;

            if(stringData)
            {
                tmpS.length = (tulong)strlen(str);
            }

            tmpS.stringData = (tchar *) malloc((length + 1) * sizeof(tchar));
            stringData[0]= '\0';

            if(str && stringData)
            {
                tulong pos;
                strncpy(tmpS.stringData, stringData, (length + 1));

                while(tmpS.Find(str,&pos))
                {
                    for(tulong i= pos ; i< tmpS.length-strlen(str) ; i++)
                    {
                        tmpS.stringData[i] = tmpS.stringData[i+strlen(str)];
                    }
                    tmpS.length -= (tulong)strlen(str);
                }

                tmpS.stringData[tmpS.length]='\0';
            }
            return tmpS;

        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////

        tulong Length() const
        {
            return length;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////

        tulong Find(const tstring & str, tulong * position = NULL) const
        {
            tulong j;
            tulong nbOcc=0;
            if(stringData && str.stringData)
            {
                if (position) *position = 0;
                if (str.length==0)    return 0;

                for (tlong i=0; i<((tlong) length) - (tlong)str.length + 1 ; i++)
                {
                    for (j=0; j<str.length && str.stringData[j] == stringData[j+i]; j++) {}

                    if (j == str.length)
                    {
                        if (position && !*position)    *position = i;
                        nbOcc++;
                    }

                }
            }
            return nbOcc;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        tulong Replace(char toReplace, char replaceBy)
        {
            tulong nbReplace =0;
            for (unsigned int i=0;i<Length();i++)
            {
                if (stringData[i] == toReplace)
                {
                    stringData[i] = replaceBy;
                    nbReplace++;
                }
            }
            return nbReplace;
        }

        tulong Replace(tstring toReplace, tstring replaceBy)
        {
            tulong nbReplace =0;
            tstring newStr = "";
            tstring curStr = *this;

            tulong position;
            tulong nbOcc;

            while( (nbOcc = curStr.Find(toReplace, &position)) > 0 )
            {
                tstring tmpStr = curStr;
                tmpStr.stringData[position] = 0;
                newStr += tmpStr;
                newStr += replaceBy;
                tstring tmpStr2;
                tmpStr2 = &(curStr.stringData[position+toReplace.Length()]);
                curStr = tmpStr2;
                nbReplace++;
            }

            newStr += curStr;

            *this = newStr;
            return nbReplace;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
/*
        tulong Find(const char * str, tulong * position) const
        {
            tulong nbOcc=0;
            tulong j;

            if(stringData && str)
            {
                if (position) *position = 0;
                if (strlen(str)==0)    return 0;


                for (tlong i=0; i<((tlong) length) - (tlong)strlen(str) + 1 ; i++)
                {
                    for (j=0; j<strlen(str) && str[j] == stringData[j+i]; j++);

                    if (j == strlen(str))
                    {
                        if (position && !*position) *position = i;
                        nbOcc++;
                    }

                }
            }
            return nbOcc;
        }
*/
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        //! This function convert the set of characters to LowerCase.
        //! Ex : "HelLo" -> "hello"
        //! Ex : "TEST 123" -> "test 123"

        void ToLower()
        {
            if(stringData)
            {
                for(tulong i=0; i<length ; i++)
                {
                    if((stringData[i] >= 65) && (stringData[i] <= 90))
                    {
                        stringData[i] += 32;
                    }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        //! This function convert the set of characters to UpperCase.
        //! Ex : "HelLo" -> "HELLO"

        void ToUpper()
        {
            if(stringData)
            {
                for(tulong i=0; i<length ; i++)
                {
                    if((stringData[i] >= 97) && (stringData[i] <= 122))
                    {
                        stringData[i] -= 32;
                    }
                    else
                    {
                        switch((unsigned char)stringData[i])
                        {
                            case 129:
                            case 150:
                            case 151:
                                stringData[i] = 'U';
                                break;

                            case 130:
                            case 136:
                            case 137:
                            case 138:
                                stringData[i] = 'E';
                                break;

                            case 131:
                            case 132:
                            case 133:
                            case 134:
                                stringData[i] = 'A';
                                break;

                            case 139:
                            case 140:
                            case 141:
                                stringData[i] = 'I';
                                break;

                            case 147:
                            case 148:
                            case 149:
                                stringData[i] = 'O';
                                break;

                            case 152:
                                stringData[i] = 'Y';
                                break;

                            default:
                                break;
                        }
                    }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        //! This function Inverse the order of the set of characters
        //! Ex : "abcde" -> "edcba"

        void Invert()
        {
            int i;
            tstring tmp;


            for(i=length-1;i>=0; i--)
            {
                tmp += GetCar((uint32)i);
            }

            *this = tmp;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        //! @param startCar    begining of the substring.
        //! @param endCar        end of the substring. Set this to auto set this to the length of the string
        //! @return                A new string.

        tstring GetSubString(tlong startCar, tlong endCar = -1)
        {
            tstring tmpS;

            tmpS.length = 0;

            if(endCar == -1)
            {
                endCar = length;
            }

            if(stringData)
            {
                tmpS.length = endCar - startCar;
            }

            tmpS.stringData = (tchar *)malloc((tmpS.length + 1) * sizeof(tchar));
            tmpS.stringData[0] = '\0';

            if(stringData)
            {
                memcpy(tmpS.stringData,&stringData[startCar], tmpS.length);
            }
            tmpS.stringData[tmpS.length] = '\0';

            return tmpS;

        }



        ///////////////////////////////////////////////////////////////////////////////////////////////////

        // Misc String Methods ////////////////////////////////////////

        //! Build a formatted String
        void Printf(const char *format, va_list *ptr_arg)
        {
			// DON'T use vsprintf_s here. CEGUI shits it.
            tchar strToWrite[4000];
            vsprintf(strToWrite, format, *ptr_arg);
            *this = strToWrite;
        }

        void Printf(const char *format,...)
        {
            tchar strToWrite[4000];

            va_list ptr_arg;
            va_start(ptr_arg, format);
            vsprintf(strToWrite, format, ptr_arg);

            length = (tulong)strlen(strToWrite);

            if(stringData)
            {
                stringData = (tchar *)realloc(stringData, (length + 1) * sizeof(tchar));
            }
            else
            {
                stringData = (tchar *)malloc((length + 1) * sizeof(tchar));
            }

            memcpy(stringData, strToWrite, length);
            stringData[length] = '\0';

            va_end(ptr_arg);
        }

        void LoadFile(ZFile & file);
		void LoadMem(char **pBuf);
        void LoadFileZeroed(ZFile & file);

        void SaveFile(ZFile & file) const;
};

// Inline /////////////////////////////////////////////////////////////////////////////////////////

inline tstring::operator char*()
{
    if(!stringData)
    {
        stringData = (tchar *)malloc(sizeof(tchar));
        stringData[0] = '\0';
    }

    return stringData;
}

inline tstring::operator const char*() const
{
    return stringData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Accesseurs

inline void tstring::operator = ( const tstring & str )
{
     if(str.stringData != NULL )
    {
        length = str.length ;

        if(stringData)
        {
            stringData = (tchar *)realloc(stringData, (length + 1) * sizeof(tchar));
        }
        else
        {
            stringData = (tchar *)malloc((length + 1) * sizeof(tchar));
        }

        strncpy(stringData, str.stringData, (length + 1));
    }
    else
    {
        if(stringData)
        {
            stringData[0] = 0;
        }
        else
        {
            stringData = (tchar *)malloc(sizeof(tchar));
            stringData[0] = 0;
        }
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline void tstring::operator = ( const char * str )
{
     if(str != NULL )
    {
        length = (tulong)strlen(str);

        if(stringData)
        {
            stringData = (tchar *)realloc(stringData, (length + 1) * sizeof(tchar));
        }
        else
        {
            stringData = (tchar *)malloc((length + 1) * sizeof(tchar));
        }

        strncpy(stringData, str, (length + 1));
    }
    else
    {
        if(stringData)
        {
            stringData[0] = 0;
        }
        else
        {
            stringData = (tchar *)malloc(sizeof(tchar));
            stringData[0] = 0;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator == ( const tstring & str ) const
{
     if((str.stringData == NULL ) || (stringData == NULL))
    {
        return false;
    }

   return ((length == str.length) && !memcmp(stringData, str.stringData, length * sizeof(tchar)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator != ( const tstring & str ) const
{
     if((str.stringData == NULL ) || (stringData == NULL))
    {
        return true;
    }

   return ((length != str.length) || memcmp(stringData, str.stringData, length * sizeof(tchar)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator < ( const tstring & str ) const
{
     if((str.stringData == NULL ) || (stringData == NULL))
    {
        return false;
    }

    return (strcmp(stringData, str.stringData)<0);//(0 > memcmp(stringData, str.stringData, length * sizeof(tchar)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator > ( const tstring & str ) const
{
     if((str.stringData == NULL ) || (stringData == NULL))
    {
        return false;
    }

    return (0 < memcmp(stringData, str.stringData, length * sizeof(tchar)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator == (const tchar * str) const
{
    if((!str) || (stringData == NULL))
    {
        return false;
    }

   return (!strcmp(stringData, str));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator != (const tchar * str) const
{
     if((!str) || (stringData == NULL))
    {
        return true;
    }
    return (0 != strcmp(stringData, str));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator < (const tchar * str) const
{
     if((!str) || (stringData == NULL))
    {
        return false;
    }
    return (0 > strcmp(stringData, str));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline bool tstring::operator > (const tchar * str) const
{
    if((!str) || (stringData == NULL))
    {
        return false;
    }
   return (0 < strcmp(stringData, str));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

inline void tstring::BackDrop()
{
    if(stringData)
    {
        if (length)
        {
            length--;
            stringData[length]= '\0';
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
