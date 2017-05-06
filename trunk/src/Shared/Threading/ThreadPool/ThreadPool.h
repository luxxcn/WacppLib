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
    uint32 thread_id;// 创建线程时赋值

    void Steup(HANDLE h)
    {
        hThread = h;
    }

    void Suspend()
    {
        // 仅本线程可以控制悬停
        ASSERT(GetCurrentThreadId() == thread_id);
        SuspendThread(hThread);
    }

    void Resume()
    {
        // 仅非本线程可以恢复
        ASSERT(GetCurrentThreadId() != thread_id);
        if(ResumeThread(hThread) == (DWORD)-1)
        {
            DWORD err = GetLastError();
            printf("线程控制器发生错误,LastError:%u", err);
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

    /** 启动线程池 */
    void Startup();

    /** 关闭线程池 */
    void Shutdown();

    /** 线程池状态检查 最好约2分钟检查一次 */
    void IntegrityCheck();

    void KillFreeThreads(uint32 count);

    /** 线程退出
     * @return true  线程t进入悬停状态,可用于执行新任务
     *         false 关闭,或线程t不再需要
     */
    bool ThreadExit(Thread * t);

    /** 调用或创建线程执行任务 */
    void ExecuteTask(ThreadBase * ExecutionTarget);

    inline uint32 GetFreeThreadCount()   { return (uint32)m_freeThreads.size();   }
    inline uint32 GetActiveThreadCount() { return (uint32)m_activeThreads.size(); }

private:

    /** 创建一个线程 */
    Thread * StartThread(ThreadBase * ExecutionTarget);
};

extern CThreadPool ThreadPool;

#endif // THREAD_POOL_H
