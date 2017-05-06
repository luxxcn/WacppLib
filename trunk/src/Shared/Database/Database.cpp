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

#include "DatabaseEnv.h"

Database::Database()
{
    m_connections = NULL;
    m_connectionCount = -1; // not connected
    ThreadRunning = false;
}

Database::~Database()
{

}

void Database::_Initialize()
{
    // Spawn Database thread
    if(m_connectionCount > 1)
        ThreadPool.ExecuteTask(this);

    // launch the query thread
    if(m_connectionCount > 2)
    {
        qt = new QueryThread(this);
        ThreadPool.ExecuteTask(qt);
    }
}

bool Database::run()
{
    SetThreadName("Database Execute Thread");
    SetThreadState(THREADSTATE_BUSY);
    ThreadRunning = true;

    char * query = queries_queue.pop();
    DatabaseConnection * conn = GetFreeConnection();
    while(1)
    {
        if(query)
        {
            _SendQuery(conn, query, false);
            delete [] query;
        }

        if(GetThreadState() == THREADSTATE_TERMINATE)
            break;

        query = queries_queue.pop();

        if(!query)
            Sleep(10);
    }
    if(conn)
        conn->Busy.Release();

    // execute all the remaining queries
    query = queries_queue.pop();
    conn  = GetFreeConnection();
    while(query)
    {
        if(conn)
        {
            _SendQuery(conn, query, false);
            delete [] query;
        }

        query = queries_queue.pop();
    }
    if(conn)
        conn->Busy.Release();

    ThreadRunning = false;
    return false;
}

// Use this when we request data that can return a value (not async)
QueryResult * Database::Query(const char * sql, ...)
{
    char query[16384];
    va_list ap;
    va_start(ap, sql);
    vsnprintf(query, 16384, 16384, sql, ap);
    va_end(ap);

    // Send the query
    QueryResult * result = NULL;
    DatabaseConnection * conn = GetFreeConnection();
    if(_SendQuery(conn, sql, false))
        result = _StoreQueryResult(conn, query);

    conn->Busy.Release();

    return result;
}

QueryResult * Database::QueryNA(const char * sql)
{
    // Send the query
    QueryResult * result = NULL;
    DatabaseConnection * conn = GetFreeConnection();
    if(_SendQuery(conn, sql, false))
        result = _StoreQueryResult(conn, sql);

    conn->Busy.Release();

    return result;
}

QueryResult * Database::FQuery(const char * sql, DatabaseConnection * conn)
{
    // Send the query
    if(_SendQuery(conn, sql, false))
        return _StoreQueryResult(conn, sql);

    return NULL;
}

bool Database::Execute(const char * sql, ...)
{
    char query[16384];
    va_list ap;
    va_start(ap, sql);
    vsnprintf(query, 16384, 16384, sql, ap);
    va_end(ap);

    if(!ThreadRunning)
        return WaitExecuteNA(query);

    size_t len = strlen(query);
    char * buffer = new char[len + 1];
    memcpy(buffer, query, len + 1);

    queries_queue.push(buffer);
    return true;
}

bool Database::ExecuteNA(const char * sql)
{
    if(!ThreadRunning)
        return WaitExecuteNA(sql);

    size_t len = strlen(sql);
    char * buffer = new char[len + 1];
    memcpy(buffer, sql, len + 1);

    queries_queue.push(buffer);
    return true;
}

bool Database::WaitExecute(const char * sql, ...)
{
    char query[16384];
    va_list ap;
    va_start(ap, sql);
    vsnprintf(query, 16384, 16384, sql, ap);
    va_end(ap);

    DatabaseConnection * conn = GetFreeConnection();
    bool res = _SendQuery(conn, query, false);
    conn->Busy.Release();

    return res;
}

bool Database::WaitExecuteNA(const char * sql)
{
    DatabaseConnection * conn = GetFreeConnection();
    bool res = _SendQuery(conn, sql, false);
    conn->Busy.Release();

    return res;
}

bool Database::FWaitExecute(const char * sql, DatabaseConnection * conn)
{
    return _SendQuery(conn, sql, false);
}

void Database::QueueAsyncQuery(AsyncQuery * query)
{
    query->db = this;
    // todo: need sub thread ?
    query->Perform();
}

void Database::EndThreads()
{
    //these 2 loops spin until theres nothing left
    while (1)
    {
        QueryBuffer* buf = query_buffer.pop();
        if (buf == NULL)
            break;
        query_buffer.push(buf);
    }
    while (1)
    {
        char* buf = queries_queue.pop();
        if (buf == NULL)
            break;
        queries_queue.push(buf);
    }

    SetThreadState(THREADSTATE_TERMINATE);

    while(ThreadRunning || qt)
    {
        Sleep(100);
        if(!ThreadRunning)
            break;
    }
}

void Database::FreeQueryResult(QueryResult * p)
{
    delete p;
}

DatabaseConnection * Database::GetFreeConnection()
{
    uint32 i = 0;
    while(1)
    {
        DatabaseConnection * conn = m_connections[(++i % m_connectionCount)];
            if(conn->Busy.AttemptAcquire())
                return conn;
    }

    // shouldn't reached
    return NULL;
}

void Database::PerformQueryBuffer(QueryBuffer * b, DatabaseConnection * ccon)
{
    if(!b->queries.size())
        return;

    DatabaseConnection * conn = ccon;
    if(!ccon)
        conn = GetFreeConnection();

    _BeginTransaction(conn);

    for(vector<char*>::iterator itr = b->queries.begin(); itr != b->queries.end(); ++itr)
    {
        _SendQuery(conn, *itr, false);
        delete [] (*itr);
    }

    _EndTransaction(conn);

    if(!ccon)// get free connection
        conn->Busy.Release();
}

void Database::thread_proc_query()
{
    QueryBuffer* q;
    DatabaseConnection* conn = GetFreeConnection();

    q = query_buffer.pop();
    while(1)
    {
        if(q)
        {
            PerformQueryBuffer(q, conn);
            delete q;
        }

        if(GetThreadState() == THREADSTATE_TERMINATE)
            break;

        q = query_buffer.pop();
        if(!q)
            Sleep(10);
    }

    conn->Busy.Release();

    // kill any queries
    q = query_buffer.pop( );
    while(q)
    {
        PerformQueryBuffer( q, NULL );
        delete q;

        q = query_buffer.pop();
    }
}

void Database::AddQueryBuffer(QueryBuffer * b)
{
    if(qt)
        query_buffer.push(b);
    else
    {
        PerformQueryBuffer( b, NULL );
        delete b;
    }
}

AsyncQuery::~AsyncQuery()
{
    delete func;
    for(vector<AsyncQueryResult>::iterator itr = queries.begin(); itr != queries.end(); ++itr)
    {
        if(itr->result)
            delete itr->result;

        delete[] itr->query;
    }
}

void AsyncQuery::AddQuery(const char * format, ...)
{
    AsyncQueryResult res;
    va_list ap;
    char buffer[10000];
    size_t len;
    va_start(ap, format);
    vsnprintf(buffer, 10000, format, ap);
    va_end(ap);
    len = strlen(buffer);

    ASSERT(len);// todo: my ASSERT

    res.query = new char[len+1];
    res.query[len] = 0;
    memcpy(res.query, buffer, len);
    res.result = NULL;
    queries.push_back(res);
}

void AsyncQuery::Perform()
{
    DatabaseConnection * conn = db->GetFreeConnection();
    for(vector<AsyncQueryResult>::iterator itr = queries.begin(); itr != queries.end(); ++itr)
        itr->result = db->FQuery(itr->query, conn);

    conn->Busy.Release();
    func->run(queries);

    delete this;
}

void QueryBuffer::AddQuery(const char * format, ...)
{
    char query[16384];
    va_list vlist;
    va_start(vlist, format);
    vsnprintf(query, 16384, format, vlist);
    va_end(vlist);

    size_t len = strlen(query);
    char * pBuffer = new char[len+1];
    memcpy(pBuffer, query, len + 1);

    queries.push_back(pBuffer);
}

void QueryBuffer::AddQueryNA( const char * str )
{
    size_t len = strlen(str);
    char * pBuffer = new char[len+1];
    memcpy(pBuffer, str, len + 1);

    queries.push_back(pBuffer);
}

void QueryBuffer::AddQueryStr(const string& str)
{
    size_t len = str.size();
    char * pBuffer = new char[len+1];
    memcpy(pBuffer, str.c_str(), len + 1);

    queries.push_back(pBuffer);
}

QueryThread::~QueryThread()
{
    db->qt = NULL;
}

bool QueryThread::run()
{
    db->thread_proc_query();
    return true;
}
