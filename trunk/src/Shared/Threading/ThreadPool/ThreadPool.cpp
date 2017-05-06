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

#include "ThreadPool.h"
#include "Log.h"

#define THREAD_RESERVE 10
CThreadPool ThreadPool;

CThreadPool::CThreadPool()
{
    //_threadsToExit = 0;
    _threadsToEaten = 0;
}

bool CThreadPool::ThreadExit(Thread *t)
{
    _mutex.Acquire();

    m_activeThreads.erase(t);

    // �Ƿ���Ҫ�ر�һЩ�߳�
    if(_threadsToExit > 0)
    {
        --_threadsToExit;
        if(t->DeleteAfterExit)
            m_freeThreads.erase(t);

        _mutex.Release();
        delete t;
        return false;
    }

    // ��ͣ
    ++_threadsToEaten;
    ThreadSet::iterator itr = m_freeThreads.find(t);
    if(itr != m_freeThreads.end())
    {
        sLog.Error("�̳߳�", "���г�,�߳�%u���߳�%u�ظ�",
            (*itr)->ControlInterface.GetId(), t->ControlInterface.GetId());
    }
    m_freeThreads.insert(t);

    sLog.Debug("�̳߳�", "�߳�%u������г�", t->ControlInterface.GetId());

    _mutex.Release();
    return true;
}

static unsigned long WINAPI thread_proc(void * param)
{
    Thread * t = (Thread*)param;

    t->SetupMutex.Acquire();

    uint32 thread_id = t->ControlInterface.GetId();
    bool ht = (t->ExecutionTarget != NULL);

    t->SetupMutex.Release();

    sLog.Debug("�̳߳�", "�߳�%u����", thread_id);
    while(1)
    {
        if(t->ExecutionTarget)
        {
            if(t->ExecutionTarget->run())
                delete t->ExecutionTarget;

            t->ExecutionTarget = NULL;
        }

        if(!ThreadPool.ThreadExit(t))
        {
            sLog.Debug("�̳߳�", "�߳�%u�˳�", thread_id);
            break;
        }
        else
        {
            if(ht)
                sLog.Debug("�̳߳�", "�߳�%u�ȴ���������", thread_id);
            t->ControlInterface.Suspend();
        }
    }

    ExitThread(0);
}

Thread * CThreadPool::StartThread(ThreadBase *ExecutionTarget)
{
    HANDLE h;
    Thread * t = new Thread;
    t->DeleteAfterExit = false;
    t->ExecutionTarget = ExecutionTarget;

    t->SetupMutex.Acquire();
    h = CreateThread(NULL, 0, &thread_proc, (LPVOID)t, 0, (LPDWORD)&t->ControlInterface.thread_id);
    t->ControlInterface.Steup(h);
    t->SetupMutex.Release();

    return t;
}

void CThreadPool::ExecuteTask(ThreadBase *ExecutionTarget)
{
    Thread * t;
    _mutex.Acquire();

    --_threadsToEaten;

    if(m_freeThreads.size())
    {
        ThreadSet::iterator itr = m_freeThreads.begin();
        t = *itr;
        m_freeThreads.erase(itr);

        t->ExecutionTarget = ExecutionTarget;

        t->ControlInterface.Resume();
        sLog.Debug("�̳߳�", "�߳�%u�뿪�̳߳�", t->ControlInterface.GetId());
    }
    else
        t = StartThread(ExecutionTarget);

    m_activeThreads.insert(t);

    _mutex.Release();
}

void CThreadPool::Startup()
{
    int count = THREAD_RESERVE;
    for(int i = 0; i < count; i++)
        StartThread(NULL);

    sLog.Debug("�̳߳�", "����%u���߳�", count);
}

void CThreadPool::Shutdown()
{
    _mutex.Acquire();

    size_t tcount = m_activeThreads.size() + m_freeThreads.size();
    sLog.Debug("�̳߳�", "��ʼ�ر�%u���߳�", tcount);
    KillFreeThreads((uint32)m_freeThreads.size());
    _threadsToExit += (uint32)m_activeThreads.size();

    for(ThreadSet::iterator itr = m_activeThreads.begin(); itr != m_activeThreads.end(); itr++)
    {
        Thread * t = *itr;
        if(t->ExecutionTarget)
            t->ExecutionTarget->OnShutdown();
        else
            t->ControlInterface.Resume();
    }
    _mutex.Release();

    for(int i = 0; ; i++)
    {
        _mutex.Acquire();
        if(m_activeThreads.size() || m_freeThreads.size())
        {
            if(i != 0 && m_freeThreads.size())
            {
                Thread *t;
                ThreadSet::const_iterator itr;
                for(itr = m_freeThreads.begin(); itr != m_freeThreads.end(); itr++)
                {
                    t = *itr;
                    t->ControlInterface.Resume();
                }
            }
            sLog.Debug("�̳߳�", "ʣ��%u�������߳�,%u�������߳�", m_activeThreads.size(), m_freeThreads.size());
            _mutex.Release();
            // �������б�Sleep,���ܻ�������߳��޷�����
            Sleep(1000);
            continue;
        }

        _mutex.Release();
        break;
    }
}

void CThreadPool::IntegrityCheck()
{
    _mutex.Acquire();

    int32 gobbled = _threadsToEaten;

    if(gobbled < 0)
    {
        //�̲߳�����
        uint32 new_threads = abs(gobbled) + THREAD_RESERVE;
        _threadsToEaten = 0;

        for(uint32 i = 0; i < new_threads; i++)
            StartThread(NULL);

        sLog.Debug("�̳߳�", "�������,(gobbled < 0)����%u���߳�", gobbled);
    }
    else if(gobbled < THREAD_RESERVE)
    {
        uint32 new_threads = THREAD_RESERVE - gobbled;
        _threadsToEaten = 0;

        for(uint32 i = 0; i < new_threads; i++)
            StartThread(NULL);

        sLog.Debug("�̳߳�", "�������,(gobbled < %u)����%u���߳�", THREAD_RESERVE, gobbled);
    }
    else if(gobbled > THREAD_RESERVE)
    {
        uint32 kill_count = (gobbled - THREAD_RESERVE);
        KillFreeThreads(kill_count);
        _threadsToEaten -= kill_count;

        sLog.Debug("�̳߳�", "�������,(gobbled > %u)�ر�%u���߳�", THREAD_RESERVE, kill_count);
    }
    else
        sLog.Debug("�̳߳�", "�������,����!");

    _mutex.Release();
}

void CThreadPool::KillFreeThreads(uint32 count)
{
    sLog.Debug("�̳߳�", "�ر�%u���߳�", count);

    _mutex.Acquire();

    ThreadSet::iterator itr = m_freeThreads.begin();
    for(uint32 i = 0; i < count, itr != m_freeThreads.end(); i++, itr++)
    {
        Thread * t = *itr;
        t->ExecutionTarget = NULL;
        t->DeleteAfterExit = true;
        ++_threadsToExit;
        t->ControlInterface.Resume();
    }

    _mutex.Release();
}
