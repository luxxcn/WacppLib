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

#include <Windows.h>
#include <stdio.h>
#include "ConfigIni.h"

ConfigIni::ConfigIni()
{
    memset(m_path, 0, MAX_PATH);
}

ConfigIni::~ConfigIni()
{

}

bool ConfigIni::SetSource(const char *szFileName)
{
    char szDir[MAX_PATH];
    if(GetCurrentDirectory(MAX_PATH, szDir))// must get curr dir
    {
        sprintf_s(m_path, MAX_PATH, "%s\\%s", szDir, szFileName);

        // check file exists
        FILE *pf = fopen(m_path, "r");
        if(pf)
        {
            fclose(pf);
            return true;
        }
    }

    return false;
}

bool ConfigIni::GetBoolDefault(const char *section, const char *name, bool def)
{
    return ((unsigned int)GetIntDefault(section, name, def ? 1 : 0) > 0 ? true : false);
}

int ConfigIni::GetIntDefault(const char *section, const char *name, int def)
{
    if(!section || !strlen(section) || !name || strlen(name))
        return def;

    return GetPrivateProfileInt(section, name, def, m_path);
}

float ConfigIni::GetFloatDefault(const char *section, const char *name, float def)
{
    char szDef[MAX_PATH];
    sprintf_s(szDef, MAX_PATH, "%f", def);
    return static_cast<float>(atof(GetStringDefault(section, name, szDef).c_str()));
}

std::string ConfigIni::GetStringDefault(const char *section, const char *name, const char *def)
{
    if(!section || !strlen(section) || !name || strlen(name))
        return def;
    
    char res[MAX_PATH];
    std::string szResult;
    DWORD len = GetPrivateProfileString(section, name, def, res, MAX_PATH, m_path);

    // check length
    if(len >= MAX_PATH)
    {
        char *result = new char[len + 1];
        std::string szResult = result;
        delete [] result;
    }

    return szResult;
}

/** setting */
bool ConfigIni::SetBool(const char *section, const char *name, bool value)
{
    char szValue[2];
    sprintf_s(szValue, 2, "%d", value ? 1 : 0);
    return SetString(section, name, szValue);
}

bool ConfigIni::SetInt(const char *section, const char *name, int value)
{
    char szValue[MAX_PATH];
    sprintf_s(szValue, MAX_PATH, "%d", value);
    return SetString(section, name, szValue);
}

bool ConfigIni::SetFloat(const char *section, const char *name, float value)
{
    char szValue[MAX_PATH];
    sprintf_s(szValue, MAX_PATH, "%f", value);
    return SetString(section, name, szValue);
}

bool ConfigIni::SetString(const char *section, const char *name, const char *value)
{
    return WritePrivateProfileString(section, name, value, m_path) ? true : false;
}
