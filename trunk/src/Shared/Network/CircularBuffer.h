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

// �ο�: http://www.codeproject.com/KB/IP/bipbuffer.aspx

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

class CircularBuffer
{
public:
    CircularBuffer();
    ~CircularBuffer();

    /** ����洢�ռ� */
    void Allocate(size_t size);

    /** д�����ݵ�buffer��
     * @param data ��д�������
     * @param size ���ݳ���
     */
    bool Write(const void * data, size_t size);

    /** ��buffer�ж�ȡ����
     * @param dst  ���ݱ��浽
     * @param size ��ȡ����
     */
    bool Read(void * dst, size_t size);

    /** ��ȡʣ��ռ� */
    size_t GetSpace();

    /** ��ȡbuffer�д洢�����ݳ��� */
    size_t GetSize() { return (m_sizeA + m_sizeB); }
    /** ��ȡ�������ݳ��� */
    size_t GetContiguousBytes() { return (m_sizeA ? m_sizeA : m_sizeB); }

    /** �Ƴ��������� */
    void Remove(size_t len);

    /** ��ȡ������д��λ��("ĩβ") */
    void * GetBuffer();

    /** ������дָ�볤�� */
    void IncrementWritten(size_t len);

    /** ��ȡbuffer��ʼλ�� */
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
