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

#ifndef FILED_H_
#define FILED_H_

#define atol(a) strtoul(a, NULL, 10)

class Field
{
public:
    inline void SetValue(char * value) { mValue = value; }

    inline const char * GetString() { return mValue; }
    // signed
    inline float GetFlot()    { return mValue ? static_cast<float>(atof(mValue)) : 0.0f; }
    inline bool GetBool()     { return mValue ? atoi(mValue) > 0 : false;                }
    inline int8 GetInt8()     { return mValue ? static_cast<int8>(atol(mValue)) : 0;     }
    inline int16 GetInt16()   { return mValue ? static_cast<int16>(atol(mValue)) : 0;    }
    inline int32 GetInt32()   { return mValue ? static_cast<int32>(atol(mValue)) : 0;    }
    // unsigned 
    inline uint8 GetUInt8()   { return mValue ? static_cast<uint8>(atol(mValue)) : 0;    }
    inline uint16 GetUInt16() { return mValue ? static_cast<uint16>(atol(mValue)) : 0;   }
    inline uint32 GetUInt32() { return mValue ? static_cast<uint32>(atol(mValue)) : 0;   }
    inline uint64 GetUInt64()
    {
        if(mValue)
        {
            uint64 value;
            sscanf_s(mValue, I64FMTD, &value);
            return value;
        }
        else
            return 0;
    }
    
private:
    char * mValue;
};

#endif // FILED_H_
