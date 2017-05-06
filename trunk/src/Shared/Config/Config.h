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

#ifndef CONFIG_H
#define CONFIG_H

#include "Singleton.h"

class DOTCONFDocument;

class SERVER_DECL Config : public Singleton<Config>
{
    public:

        Config();
        ~Config();

        bool SetSource(const char *file, bool ignorecase = true);
        bool Reload();

		bool GetString(const char* name, std::string *value);
        bool GetString(const char* name, char const **value);
        std::string GetStringDefault(const char* name, const char* def);

		bool GetBool(const char* name, bool *value);
        bool GetBoolDefault(const char* name, const bool def = false);

		bool GetInt(const char* name, int *value);
        int32 GetIntDefault(const char* name, const int32 def);

		bool GetFloat(const char* name, float *value);
        float GetFloatDefault(const char* name, const float def);

        std::string GetFilename() const { return mFilename; }

    private:

        std::string mFilename;
        bool mIgnoreCase;
        DOTCONFDocument *mConf;
};

#define sConfig Config::getSingleton()

#endif