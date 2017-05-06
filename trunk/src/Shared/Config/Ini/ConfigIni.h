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

/**
 * 对windows下 .ini 文件相关的操作的API函数的封装
 * ini文件不能少了section
 */

#ifndef CONFIG_INI_H
#define CONFIG_INI_H

#include <string>

class ConfigIni
{
public:
    ConfigIni();
    ~ConfigIni();

    bool SetSource(const char *szFileName);

    bool GetBoolDefault(const char *section, const char *name, bool def);
    int GetIntDefault(const char *section, const char *name, int def);
    float GetFloatDefault(const char *section, const char *name, float def);
    std::string GetStringDefault(const char *section, const char *name, const char *def);

    // specified section
    bool SetBool(const char *section, const char *name, bool value);
    bool SetInt(const char *section, const char *name, int value);
    bool SetFloat(const char *section, const char *name, float value);
    bool SetString(const char *section, const char *name, const char *value);



private:
    char m_path[MAX_PATH];
};

#endif // CONFIG_INI_H
