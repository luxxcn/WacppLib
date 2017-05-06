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
#include "SQLiteDatabase.h"

SQLiteDatabase::SQLiteDatabase()
{

}

SQLiteDatabase::~SQLiteDatabase()
{
    for(int32 i = 0; i < m_connectionCount; i++)
    {
        sqlite3_close(((SQLiteDatabaseConnection*)m_connections[i])->SQLite);
        delete m_connections[i];
    }
    delete [] m_connections;
}

bool SQLiteDatabase::Initialize(const char *hostname, uint32 port, const char *username, 
                                const char *password, const char *dbname, uint32 connectionCount, uint32 bufferSize)
{
    return Initialize(hostname);
}

bool SQLiteDatabase::Initialize(const char *connInfo)
{
    SQLiteDatabaseConnection ** conns;
    m_connectionCount = 1;  // only one coonection
    conns = new SQLiteDatabaseConnection*[m_connectionCount];
    m_connections = ((DatabaseConnection**)conns);

    sqlite3 *pTemp = NULL;
    for(int32 i = 0; i < m_connectionCount; i++)
    {
        if(sqlite3_open(connInfo, &pTemp) != SQLITE_OK || !pTemp)
            return false;

        conns[i] = new SQLiteDatabaseConnection;
        conns[i]->SQLite = pTemp;
    }

    return true;
}

void SQLiteDatabase::Shutdown()
{

}

string SQLiteDatabase::EscapeString(std::string str)
{
    return "";
}

void SQLiteDatabase::EscapeLongString(const char * str, uint32 len, stringstream& out)
{

}

string SQLiteDatabase::EscapeString(const char * esc, DatabaseConnection * con)
{
    return "";
}

SQLiteQueryResult::SQLiteQueryResult(char **res, uint32 FieldCount, uint32 RowCount)
: QueryResult(FieldCount, RowCount), m_result( res )
{
    m_currentRow = new Field[FieldCount];
    m_curRowId = 1; // SQLite中的第0行为表的标题行
}

SQLiteQueryResult::~SQLiteQueryResult()
{
    sqlite3_free_table(m_result);
    delete [] m_currentRow;
}

bool SQLiteQueryResult::NextRow()
{
    uint32 row = m_curRowId * m_fieldCount;
    if(m_curRowId > m_rowCount)
        return false;

    for(uint32 i = 0; i < m_fieldCount; ++i)
        m_currentRow[i].SetValue(m_result[row+i]);

    m_curRowId++;

    return true;
}

bool SQLiteDatabase::_SendQuery(DatabaseConnection *conn, const char* Sql, bool Self)// self 是否为迭代,未使用
{
    char ** ppData;
    char * pErrmsg;
    int rows, cols, result;

    if(!conn || !((SQLiteDatabaseConnection*)conn)->SQLite)
        return false;

    result = sqlite3_get_table(((SQLiteDatabaseConnection*)conn)->SQLite, Sql, &ppData, &rows, &cols, &pErrmsg);
    if(result != SQLITE_OK)
        return false; // TODO: handle err msg

    return true;
}

void SQLiteDatabase::_BeginTransaction(DatabaseConnection * conn)
{
}

void SQLiteDatabase::_EndTransaction(DatabaseConnection * conn)
{
}

QueryResult * SQLiteDatabase::_StoreQueryResult(DatabaseConnection * conn,  const char *sql)
{
    char ** ppData;
    char * pErrmsg;
    int rows, cols, result;

    if(!conn || !((SQLiteDatabaseConnection*)conn)->SQLite)
        return NULL;

    result = sqlite3_get_table(((SQLiteDatabaseConnection*)conn)->SQLite, sql, &ppData, &rows, &cols, &pErrmsg);
    if(result != SQLITE_OK)
        return NULL; // TODO: handle err msg

    SQLiteQueryResult *res = new SQLiteQueryResult(ppData, cols, rows);
    res->NextRow();

    return res;
}
