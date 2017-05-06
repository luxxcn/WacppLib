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

// 参考: http://www.codeproject.com/KB/IP/bipbuffer.aspx

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

class CircularBuffer
{
public:
    CircularBuffer();
    ~CircularBuffer();

    /** 分配存储空间 */
    void Allocate(size_t size);

    /** 写入数据到buffer中
     * @param data 待写入的数据
     * @param size 数据长度
     */
    bool Write(const void * data, size_t size);

    /** 从buffer中读取数据
     * @param dst  数据保存到
     * @param size 读取长度
     */
    bool Read(void * dst, size_t size);

    /** 获取剩余空间 */
    size_t GetSpace();

    /** 获取buffer中存储的数据长度 */
    size_t GetSize() { return (m_sizeA + m_sizeB); }
    /** 获取连续数据长度 */
    size_t GetContiguousBytes() { return (m_sizeA ? m_sizeA : m_sizeB); }

    /** 移除部分数据 */
    void Remove(size_t len);

    /** 获取新数据写入位置("末尾") */
    void * GetBuffer();

    /** 增加已写指针长度 */
    void IncrementWritten(size_t len);

    /** 获取buffer开始位置 */
    void * GetBufferStart() { return m_sizeA ? m_regionA : m_regionB; }

private:
    uint8 * m_buffer;
    uint8 * m_bufferEnd;
    uint8 * m_regionA;
    uint8 * m_regionB;
    size_t m_sizeA;
    size_t m_sizeB;

    inline void AllocateB()         { m_regionB = m_buffer;                                                }
    inline size_t GetAFreeSpace()   { return (m_bufferEnd - m_regionA - m_sizeA);                          }
    inline size_t GetSpaceBeforeA() { return (m_regionA - m_buffer);                                       }
    inline size_t GetBFreeSpace()   { if(!m_regionB) return 0; return (m_regionA - m_regionB - m_sizeB);   }
};

#endif // CIRCULAR_BUFFER_H
