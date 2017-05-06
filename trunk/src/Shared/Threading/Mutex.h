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

#ifndef MUTEX_H
#define MUTEX_H

class SERVER_DECL Mutex
{
public:
    Mutex();
    ~Mutex();

    /** ��û�����,��ȡʧ��ʱ�ȴ� */
    inline void Acquire()
    {
        EnterCriticalSection(&cs);
    }

    /** ���Ի�ȡ������
     * @return false ��ȡʧ��,�����߳�ռ��(δ�ͷ�)
     */
    inline bool AttemptAcquire()
    {
        return (TryEnterCriticalSection(&cs) == TRUE ? true : false);
    }

    /** �ͷŻ����� */
    inline void Release()
    {
        LeaveCriticalSection(&cs);
    }

private:
    CRITICAL_SECTION cs;
};

class SERVER_DECL FastMutex
{
#pragma pack(push, 8)
    volatile long m_lock;
#pragma pack(pop)
    DWORD m_recursiveCount;

public:
    FastMutex() : m_lock(0), m_recursiveCount(0) {}
    ~FastMutex() {}

    inline void Acquire()
    {
        DWORD thread_id = GetCurrentThreadId(), owner;
        if(thread_id == (DWORD)m_lock)
        {
            ++m_recursiveCount;
            return;
        }

        while(1)
        {
            owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
            if(owner == 0)
                break;

            Sleep(0);
        }
        ++m_recursiveCount;
    }

    inline bool AttempdAcquire()
    {
        DWORD thread_id = GetCurrentThreadId();

        if(thread_id == (DWORD)m_lock)
        {
            ++m_recursiveCount;
            return true;
        }

        /** m_lock��0���,������m_lock = thread_id, ����m_lockԭʼֵ */
        DWORD owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
        if(owner == 0)
        {
            ++m_recursiveCount;
            return true;
        }

        return false;
    }

    inline void Release()
    {
        if((--m_recursiveCount) == 0)
            InterlockedExchange(&m_lock, 0);
    }
};

#endif // MUTEX_H
