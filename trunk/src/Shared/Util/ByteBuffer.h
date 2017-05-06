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

#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include "Common.h"

class SERVER_DECL ByteBuffer
{
public:
    const static size_t DEFAULT_SIZE = 0x1000;

    ByteBuffer() : _rpos(0), _wpos(0)
    {
        _storage.reserve(DEFAULT_SIZE);
    }
    ByteBuffer(size_t res) : _rpos(0), _wpos(0)
    {
        _storage.reserve(res);
    }
    ByteBuffer(const ByteBuffer &buf) : _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage)
    {}
    virtual ~ByteBuffer() {}

    void clear()
    {
        _storage.clear();
        _rpos = _wpos = 0;
    }

    template <typename T>
    void append(T value)
    {
        append((uint8 *)&value, sizeof(value));
    }

    template <typename T>
    void put(size_t pos, T value)
    {
        put(pos, (uint8 *)&value, sizeof(value));
    }

    // 操作符重写,类似流操作
    // "写"操作
    ByteBuffer &operator<<(bool value)
    {
        append<char>((char)value);
        return *this;
    }
    ByteBuffer &operator<<(uint8 value)
    {
        append<uint8>(value);
        return *this;
    }
    ByteBuffer &operator<<(uint16 value)
    {
        append<uint16>(value);
        return *this;
    }
    ByteBuffer &operator<<(uint32 value)
    {
        append<uint32>(value);
        return *this;
    }
    ByteBuffer &operator<<(uint64 value)
    {
        append<uint64>(value);
        return *this;
    }

    // signed
    ByteBuffer &operator<<(int8 value)
    {
        append<int8>(value);
        return *this;
    }
    ByteBuffer &operator<<(int16 value)
    {
        append<int16>(value);
        return *this;
    }
    ByteBuffer &operator<<(int32 value)
    {
        append<int32>(value);
        return *this;
    }
    ByteBuffer &operator<<(int64 value)
    {
        append<int64>(value);
        return *this;
    }
    ByteBuffer &operator<<(float value)
    {
        append<float>(value);
        return *this;
    }
    ByteBuffer &operator<<(double value)
    {
        append<double>(value);
        return *this;
    }
    ByteBuffer &operator<<(const std::string &value)
    {
        append((uint8*)value.c_str(), value.length());
        append((uint8)0);// '\0'
        return *this;
    }
    ByteBuffer &operator<<(const char *str)
    {
        append((uint8*)str, strlen(str));
        append((uint8)0);
        return *this;
    }
    // "读"操作
    ByteBuffer &operator>>(bool &value)
    {
        value = read<char>() > 0 ? true : false;
        return *this;
    }
    // unsigned
    ByteBuffer &operator>>(uint8 &value)
    {
        value = read<uint8>();
        return *this;
    }
    ByteBuffer &operator>>(uint16 &value)
    {
        value = read<uint16>();
        return *this;
    }
    ByteBuffer &operator>>(uint32 &value)
    {
        value = read<uint32>();
        return *this;
    }
    ByteBuffer &operator>>(uint64 &value)
    {
        value = read<uint64>();
        return *this;
    }

    // signed
    ByteBuffer &operator>>(int8 &value)
    {
        value = read<int8>();
        return *this;
    }
    ByteBuffer &operator>>(int16 &value)
    {
        value = read<int16>();
        return *this;
    }
    ByteBuffer &operator>>(int32 &value)
    {
        value = read<int32>();
        return *this;
    }
    ByteBuffer &operator>>(int64 &value)
    {
        value = read<int64>();
        return *this;
    }
    ByteBuffer &operator>>(float &value)
    {
        value = read<float>();
        return *this;
    }
    ByteBuffer &operator>>(double &value)
    {
        value = read<double>();
        return *this;
    }
    ByteBuffer &operator>>(std::string &value)
    {
        value.clear();
        while(1)
        {
            char c = read<char>();
            if(!c) // '\0'
                break;
            value+=c;
        }
        return *this;
    }

    uint8 operator[](size_t pos) { return read<uint8>(pos); }

    size_t rpos() { return _rpos; }
    size_t rpos(size_t rpos) { _rpos = rpos; return _rpos; }

    size_t wpos() { return _wpos; }
    size_t wpos(size_t wpos) { _wpos = wpos; return _wpos; }

    const uint8 * contents() const { return &_storage[0];    }
    inline size_t size() const     { return _storage.size(); }

    template <typename T>
    T read(size_t pos) const
    {
        if(pos + sizeof(T) > size())
            return (T)0;
        else
            return *((T*)&_storage[pos]);
    }

    template <typename T>
    T read()
    {
        T r = read<T>(_rpos);
        _rpos += sizeof(T);
        return r;
    }

    void read(uint8 *dest, size_t len)
    {
        if(_rpos + len <= size())
            memcpy(dest, &_storage[_rpos], len);
        else
            memset(dest, 0, len); // throw error();
        _rpos += len;
    }

    void append(const uint8 * src, size_t cnt)
    {
        if(!cnt)
            return;

        // 所使用的长度最大不会超过10mb,如果超过,需要考虑算法是否有问题
        ASSERT(size() < 10000000);

        if(_storage.size() < _wpos + cnt)
            _storage.resize(_wpos + cnt);
        memcpy(&_storage[_wpos], src, cnt);
        _wpos += cnt;
    }

    void put(size_t pos, const uint8 * src, size_t cnt)
    {
        ASSERT(pos + cnt <= size());
        memcpy(&_storage[pos], src, cnt);
    }

    void reserve(size_t ressize)
    {
        if(ressize > size())
            _storage.reserve(ressize);
    }

    // 重置大小,原内容可能被截断,可能永远不会用到该函数
    void resize(size_t newsize)
    {
        _storage.resize(newsize);
        _rpos = 0;
        _wpos = size();
    }

protected:
    size_t _rpos, _wpos; // 读写位置
    std::vector<uint8> _storage;
};

#endif // BYTE_BUFFER_H

