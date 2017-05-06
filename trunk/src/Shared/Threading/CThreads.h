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

#ifndef CTHREADS_H
#define CTHREADS_H

enum CThreadState
{
	THREADSTATE_TERMINATE = 0,
	THREADSTATE_PAUSED	  = 1,
	THREADSTATE_SLEEPING  = 2,
	THREADSTATE_BUSY	  = 3,
	THREADSTATE_AWAITING  = 4,
};

struct SERVER_DECL CThread : public ThreadBase
{
public:
    CThread();
    ~CThread();

    inline void SetThreadState(CThreadState state) { m_state.SetVal(state); }
    inline CThreadState GetThreadState()
    {
        unsigned long val = m_state.GetVal();
        return static_cast<CThreadState>(val);
    }
    time_t GetStartTime() { return m_starttime; }
    int GetThreadId()     { return m_threadid;  }

    virtual bool run();
    virtual void OnShutdown();

protected:
    CThread & operator=(CThread & other)
    {
        this->m_starttime = other.m_starttime;
        this->m_threadid = other.m_threadid;
        this->m_state.SetVal(other.m_state.GetVal());
        return *this;
    }

    Threading::AtomicCounter m_state;
    time_t m_starttime;
    int    m_threadid;
};

#endif // CTHREADS_H
