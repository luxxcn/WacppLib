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

#ifndef __MD5_H
#define __MD5_H

#include <openssl/md5.h>
#include "Common.h"

class MD5Hash
{
public:
    MD5Hash();
    ~MD5Hash();

    void UpdateData(const uint8 * data, int len);
    void UpdateData(const std::string &str);

    void Initialize();
    void Finalize();

    uint8 * GetDigest(void) { return m_digest; }
    int GetLength(void) { return MD5_DIGEST_LENGTH; }

private:
    MD5_CTX m_c;
    uint8 m_digest[MD5_DIGEST_LENGTH];
};


#endif // __MD5_H

