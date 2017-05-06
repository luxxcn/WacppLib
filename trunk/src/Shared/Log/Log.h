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

#ifndef LOG_H
#define LOG_H

#include "Common.h"

extern SERVER_DECL time_t UNIXTIME;		/* update this every loop to avoid the time() syscall! */
extern SERVER_DECL tm g_localTime;

class SERVER_DECL Log : public Singleton<Log>
{
public:

    bool Init(const char* filename, uint8 level);

    /** log level 0 */
    void outString(const char * format, ...);
    void outError(const char * format, ...);

    /** log level 1 */
    void outDebug(const char * format, ...);

    /** 输出描述类日志 level 0 */
    void Detail(const char * source, const char * format, ...);
    /** 错误信息 */
    void Error(const char * source, const char * foramt, ...);
    /** 输出调试日志 level 1 */
    void Debug(const char * source, const char * format, ...);

    void Close();

private:
    void outlog(const char * msg, const char * source = "");

    HANDLE m_hStdout;
    HANDLE m_hStderr;

    uint8 m_logLevel;
    FILE * m_file;
};

#define sLog Log::getSingleton()

#endif // LOG_H
