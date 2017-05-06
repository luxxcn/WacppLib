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
#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
    : m_buffer(NULL)
    , m_bufferEnd(NULL)
    , m_regionA(NULL)
    , m_regionB(NULL)
    , m_sizeA(0)
    , m_sizeB(0)
{
}

CircularBuffer::~CircularBuffer()
{
    if(m_buffer)
        free(m_buffer);
}

void CircularBuffer::Allocate(size_t size)
{
    m_buffer    = (uint8*)malloc(size);
    m_bufferEnd = m_buffer + size;
    m_regionA   = m_buffer;
}

bool CircularBuffer::Write(const void * data, size_t size)
{
    if(!m_buffer)
        return false;

    if(m_regionB)
    {
        if(GetBFreeSpace() < size)
            return false;

        memcpy(&m_regionB[m_sizeB], data, size);
        m_sizeB += size;
    }
    else
    {
        if(GetSpaceBeforeA() > GetAFreeSpace())
        {
            AllocateB();
            if(GetBFreeSpace() < size)
                return false;

            memcpy(&m_regionB[m_sizeB], data, size);
            m_sizeB += size;
        }
        else
        {
            if(GetAFreeSpace() < size)
                return false;

            memcpy(&m_regionA[m_sizeA], data, size);
            m_sizeA += size;
        }
    }
    return true;
}

bool CircularBuffer::Read(void * dst, size_t size)
{
    if(!m_buffer)
        return false;

    if(size > (m_sizeA + m_sizeB))
        return false;

    size_t cnt = size;
    size_t aRead = 0, bRead = 0;

    // 先从A段中取数据
    if(m_sizeA)
    {
        aRead = m_sizeA > cnt ? cnt : m_sizeA;
        memcpy(dst, m_regionA, cnt);

        m_regionA += aRead;
        m_sizeA   -= aRead;
        cnt       -= aRead;
    }

    // 从B段中读取数据
    if(cnt && m_sizeB)
    {
        bRead = m_sizeB > cnt ? cnt : m_sizeB;
        memcpy((char*)dst + aRead, m_regionB, cnt);

        m_regionB += bRead;
        m_sizeB   -= bRead;
        cnt       -= bRead;
    }

    if(!m_sizeA)
    {
        if(m_sizeB)
            memcpy(m_buffer, m_regionB, m_sizeB);

        m_regionA = m_buffer;
        m_sizeA   = m_sizeB;
        m_regionB = NULL;
        m_sizeB   = 0;
    }

    return true;
}

void CircularBuffer::Remove(size_t len)
{
    size_t cnt = len;
    size_t aRem, bRem;

    if(m_sizeA)
    {
        aRem = m_sizeA > cnt ? cnt : m_sizeA;

        m_regionA += aRem;
        m_sizeA   -= aRem;
        cnt       -= aRem;
    }

    if(cnt && m_sizeB)
    {
        bRem = m_sizeB > cnt ? cnt : m_sizeB;
        
        m_regionB += bRem;
        m_sizeB   -= bRem;
        cnt       -= aRem;
    }

    if(!m_sizeA)
    {
        if(m_sizeB)
            memcpy(m_buffer, m_regionB, m_sizeB);

        m_regionA = m_buffer;
        m_sizeA   = m_sizeB;
        m_regionB = NULL;
        m_sizeB   = 0;
    }
}

void * CircularBuffer::GetBuffer()
{
    if(m_regionB)
        return m_regionB + m_sizeB;
    else
        return m_regionA + m_sizeA;
}

void CircularBuffer::IncrementWritten(size_t len)
{
    if(m_regionB)
        m_sizeB += len;
    else
        m_sizeA += len;
}

size_t CircularBuffer::GetSpace()
{
    if(m_regionB)
        return GetBFreeSpace();
    else
    {
        if(GetSpaceBeforeA() > GetAFreeSpace())
        {
            AllocateB();
            return GetBFreeSpace();
        }

        return GetAFreeSpace();
    }
}
