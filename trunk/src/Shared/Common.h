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

#ifndef UUEMU_COMMON_H
#define UUEMU_COMMON_H

#pragma warning(disable:4251) // dll-interface ?

#include <cstdlib>
#include <cstdio>

#include <cstdarg>
#include <ctime>
#include <cmath>
#include <cerrno>

#include <ASSERT.h>

#  define WIN32_LEAN_AND_MEAN // 若不定义,则可能  winsock.h与winsock2.h冲突
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdlib>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <climits>
#include <cstdlib>

#include <unordered_map>
#include <unordered_set>

typedef signed __int8    int8;
typedef signed __int16   int16;
typedef signed __int32   int32;
typedef signed __int64   int64;

typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#define I64FMTD "%I64u"

#define snprintf _snprintf
#define vsnprintf _vsnprintf_s

#define ATTR_PRINTF(F,V)

#ifndef SCRIPTLIB
    #define SERVER_DECL __declspec(dllexport)
    #define SCRIPT_DECL __declspec(dllimport)
#else
    #define SERVER_DECL __declspec(dllimport)
    #define SCRIPT_DECL __declspec(dllexport)
#endif

#include "Singleton.h"
#include "Util.h"

#include "Threading.h"
#include "AtomicULong.h"
#include "AtomicBoolean.h"
#include "AtomicCounter.h"

#endif // UUEMU_COMMON_H
