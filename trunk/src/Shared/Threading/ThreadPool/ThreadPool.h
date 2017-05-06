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

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "Common.h"

class SERVER_DECL ThreadController
{
public:
    HANDLE hThread;
    uint32 thread_id;// �����߳�ʱ��ֵ

    void Steup(HANDLE h)
    {
        hThread = h;
    }

    void Suspend()
    {
        // �����߳̿��Կ�����ͣ
        ASSERT(GetCurrentThreadId() == thread_id);
        SuspendThread(hThread);
    }

    void Resume()
    {
        // ���Ǳ��߳̿��Իָ�
        ASSERT(GetCurrentThreadId() != thread_id);
        if(ResumeThread(hThread) == (DWORD)-1)
        {
            DWORD err = GetLastError();
            printf("�߳̿�������������,LastError:%u", err);
        }
    }

    uint32 GetId() { return thread_id; }
};

struct SERVER_DECL Thread
{
    ThreadBase * ExecutionTarget;
    ThreadController ControlInterface;
    Mutex SetupMutex;
    bool DeleteAfterExit;
};

typedef std::set<Thread*> ThreadSet;

class SERVER_DECL CThreadPool
{
    uint32 _threadsToExit;
    int32 _threadsToEaten;
    Mutex _mutex;

    ThreadSet m_activeThreads;
    ThreadSet m_freeThreads;

public:
    CThreadPool();

    /** �����̳߳� */
    void Startup();

    /** �ر��̳߳� */
    void Shutdown();

    /** �̳߳�״̬��� ���Լ2���Ӽ��һ�� */
    void IntegrityCheck();

    void KillFreeThreads(uint32 count);

    /** �߳��˳�
     * @return true  �߳�t������ͣ״̬,������ִ��������
     *         false �ر�,���߳�t������Ҫ
     */
    bool ThreadExit(Thread * t);

    /** ���û򴴽��߳�ִ������ */
    void ExecuteTask(ThreadBase * ExecutionTarget);

    inline uint32 GetFreeThreadCount()   { return (uint32)m_freeThreads.size();   }
    inline uint32 GetActiveThreadCount() { return (uint32)m_activeThreads.size(); }

private:

    /** ����һ���߳� */
    Thread * StartThread(ThreadBase * ExecutionTarget);
};

extern CThreadPool ThreadPool;

#endif // THREAD_POOL_H
