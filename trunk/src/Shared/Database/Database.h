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

#ifndef _DATABASE_H
#define _DATABASE_H

#include "Callback.h"

using namespace std;
class QueryResult;
class QueryThread;
class Database;

// database type
enum DatabaseType
{
    DB_TYPE_MYSQL  = 0,
    DB_TYPE_SQLITE = 1,
};

struct DatabaseConnection
{
    Mutex Busy;
};

struct SERVER_DECL AsyncQueryResult
{
    QueryResult * result;
    char * query;
};

class SERVER_DECL AsyncQuery
{
    friend class Database;
    SQLCallbackBase * func;
    vector<AsyncQueryResult> queries;
    Database * db;
public:
    AsyncQuery(SQLCallbackBase * f) : func(f) {}
    ~AsyncQuery();

    void AddQuery(const char * sql, ...);
    void Perform();
    inline void SetDB(Database * _db) { db = _db; }
};

class SERVER_DECL QueryBuffer
{
    vector<char*> queries;
public:
    friend class Database;
    void AddQuery( const char * format, ... );
    void AddQueryNA( const char * str );
    void AddQueryStr(const string& str);
};

class SERVER_DECL Database : public CThread
{
    friend class QueryThread;
    friend class AsyncQuery;
public:
    Database();
    virtual ~Database();

    /** Thread */
    bool run();
    bool ThreadRunning;

    virtual bool Initialize(
        const char * hostname, uint32 port, 
        const char * username, const char * password, const char * dbname, 
        uint32 connectionCount, uint32 bufferSize) = 0;

    // connection info : hostname;port;username;password;dbname;connect count;buffer size
    // (sqlite) file name
    virtual bool Initialize(const char *connectInformation) = 0;

    virtual void Shutdown() = 0;

    virtual QueryResult * Query(const char * sql, ...);
    virtual QueryResult * QueryNA(const char * sql);
    virtual QueryResult * FQuery(const char * sql, DatabaseConnection * conn);

    virtual bool Execute(const char * sql, ...);
    virtual bool ExecuteNA(const char * sql);    
    virtual bool WaitExecute(const char * sql, ...);// wait for request completion
    virtual bool WaitExecuteNA(const char * sql);
    virtual bool FWaitExecute(const char * sql, DatabaseConnection * conn);

    DatabaseConnection * GetFreeConnection();

    inline const std::string & GetHostName()     { return m_hostname; }
    inline const std::string & GetDatabaseName() { return m_dbname;   }
    inline const uint32 GetQueueSize() { return queries_queue.GetSize(); }

    virtual std::string EscapeString(std::string str) = 0;
    virtual void EscapeLongString(const char * str, uint32 len, std::stringstream & out) = 0;
    virtual std::string EscapeString(const char * str, DatabaseConnection * conn) = 0;

    void QueueAsyncQuery(AsyncQuery * query);
    void EndThreads();

    void thread_proc_query();
    void FreeQueryResult(QueryResult * p);

    void PerformQueryBuffer(QueryBuffer * b, DatabaseConnection * ccon);
    void AddQueryBuffer(QueryBuffer * b);

    static Database * CreateDatabaseInterface(DatabaseType type);
    static void CleanupLibs();

    virtual bool SupportsReplaceInto() = 0;
    virtual bool SupportsTableLocking() = 0;

protected:

    // spawn threads and shizzle
    void _Initialize();

    virtual void _BeginTransaction(DatabaseConnection * conn) = 0;
    virtual void _EndTransaction(DatabaseConnection * conn) = 0;

    /** actual query function */
    virtual bool _SendQuery(DatabaseConnection * conn, const char * sql, bool self) = 0;
    virtual QueryResult * _StoreQueryResult(DatabaseConnection * conn,  const char *sql) = 0;

    FQueue<QueryBuffer*> query_buffer;

    // async sql queue
    FQueue<char*> queries_queue;

    std::string m_hostname;
    std::string m_username;
    std::string m_password;
    uint32 m_port;
    std::string m_dbname;

    DatabaseConnection ** m_connections;
    int32 m_connectionCount;

    QueryThread * qt;
};

class SERVER_DECL QueryResult
{
public:
    QueryResult(uint32 fields, uint32 rows) : m_fieldCount(fields), m_rowCount(rows), m_currentRow(NULL)
    {}
    virtual ~QueryResult() {}

    virtual bool NextRow() = 0;
    void Delete() { delete this; }

    inline Field * Fetch()        { return m_currentRow; }
    inline uint32 GetFiledCount() { return m_fieldCount; }
    inline uint32 GetRowCount()   { return m_rowCount;   }

protected:
    uint32 m_fieldCount;
    uint32 m_rowCount;
    Field * m_currentRow;
};

class SERVER_DECL QueryThread : public CThread
{
    friend class Database;
    Database * db;
public:
    QueryThread(Database * d) : CThread(), db(d) {}
    ~QueryThread();
    bool run();
};

#endif // _DATABASE_H
