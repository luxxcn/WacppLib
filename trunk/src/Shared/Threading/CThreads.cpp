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
#include "CThreads.h"

CThread::CThread() : ThreadBase()
{
    m_state.SetVal(THREADSTATE_AWAITING);
    m_starttime = 0;
}

CThread::~CThread()
{}

bool CThread::run()
{
    return false;
}

void CThread::OnShutdown()
{
    m_state.SetVal(THREADSTATE_TERMINATE);
}
