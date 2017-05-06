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
#include <Windows.h>
#include "Log.h"

createFileSingleton(Log);

SERVER_DECL time_t UNIXTIME;
SERVER_DECL tm g_localTime;

#define TRED    FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE  TNORMAL | FOREGROUND_INTENSITY
#define TBLUE   FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY

//#define FILE_NAME "Log.txt"

bool Log::Init(const char* filename, uint8 level)
{
    m_logLevel = level > 1 ? 1 : level;

    errno_t err;
    err = fopen_s(&m_file, filename, "a");
    if(err != 0)
    {
        m_file = NULL;
        printf("创建日志文件`%s`失败,错误:%d", filename, err);
        return false;
    }

    m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    m_hStderr = GetStdHandle(STD_ERROR_HANDLE);

    return true;
}

void Log::Close()
{
    if(m_file)
        fclose(m_file);
}

void Log::outlog(const char * msg, const char * source)
{
    ASSERT(m_file);

    char timebuf[8];
    time_t now;
    struct tm timeinfo;
    time(&now);
    errno_t err;
    if((err = localtime_s(&timeinfo, &now)) != 0)
        timebuf[0] = '\0';
    else
        strftime(timebuf, 8, "[%H:%M]", &timeinfo);


    printf("%s[UuEmu]%s:%s\n", timebuf, source, msg);
    fprintf(m_file, "%s[UuEmu]%s:%s\n", timebuf, source, msg);
}

void Log::outString(const char * format, ...)
{
    if(!m_file)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, 327668, format, ap);
    va_end(ap);

    SetConsoleTextAttribute(m_hStdout, TWHITE);
    outlog(buf);
}

void Log::outError(const char *format, ...)
{
    if(m_file)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, 327668, format, ap);
    va_end(ap);

    SetConsoleTextAttribute(m_hStderr, TRED);
    outlog(buf);
}

void Log::outDebug(const char *format, ...)
{
    if(m_logLevel < 1)
        return;
    if(!m_file)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, 327668, format, ap);
    va_end(ap);

    SetConsoleTextAttribute(m_hStdout, TBLUE);
    outlog(buf);
}

void Log::Detail(const char *source, const char *format, ...)
{
    if(!m_file)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, 32768, format, ap);
    va_end(ap);

    SetConsoleTextAttribute(m_hStdout, TWHITE);
    outlog(buf, source);
}

void Log::Debug(const char *source, const char *format, ...)
{
    if(m_logLevel < 1)
        return;
    if(!m_file)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, 32768, format, ap);
    va_end(ap);

    SetConsoleTextAttribute(m_hStdout, TBLUE);
    outlog(buf, source);
}

void Log::Error(const char *source, const char *format, ...)
{
    if(m_logLevel < 1)
        return;
    if(!m_file)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, 32768, format, ap);
    va_end(ap);

    SetConsoleTextAttribute(m_hStdout, TRED);
    outlog(buf, source);
}
