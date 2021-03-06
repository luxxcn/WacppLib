/*
 * Copyright (C) 2011 <http://www.wacpp.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Common.h"

using namespace std;
vector<string> StrSplit(const string &src, const string &sep)
{
    vector<string> r;
    string s;
    for(string::const_iterator itr = src.begin(); itr != src.end(); itr++)
    {
        if(sep.find(*itr) != string::npos)
        {
            if(s.length())
                r.push_back(s);
            s = "";
        }
        else
            s += *itr;
    }
    if(s.length())
        r.push_back(s);

    return r;
}

void SetThreadName(const char * format, ...)
{
    va_list ap;
    char name[256];

    va_start(ap, format);
    vsnprintf(name, 256, 256, format, ap);
    va_end(ap);

    THREADNAME_INFO info;
    info.dwType     = 0x1000;
    info.szName     = name;
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags    = 0;

    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
    }
    __except(EXCEPTION_CONTINUE_EXECUTION)
    {}
}

WCHAR* utf8ToUnicode(char* szU8Buf)
{
    int nUTF8 = strlen(szU8Buf);
    int nWCHAR = MultiByteToWideChar(CP_UTF8, 0, szU8Buf, nUTF8, NULL, 0);
    WCHAR* szUnicode = new WCHAR[nWCHAR + 1];
    MultiByteToWideChar(CP_UTF8, 0, szU8Buf, nUTF8, szUnicode, nWCHAR);
    szUnicode[nWCHAR] = '\0';

    return szUnicode;
}

char *unicodeToUtf8(WCHAR *zWideBuff)
{

    int nByte = WideCharToMultiByte(CP_UTF8, 0, zWideBuff, -1, 0, 0, 0, 0);
    char *szUtf8 = new char[nByte + 1];
    szUtf8[nByte] = '\0';
    WideCharToMultiByte(CP_UTF8, 0, zWideBuff, -1, szUtf8, nByte, 0, 0);
    return szUtf8;
}
