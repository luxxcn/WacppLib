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

#ifndef MYSQL_DATABASE_H
#define MYSQL_DATABASE_H

#include <mysql/mysql.h>

struct MySQLDatabaseConnection : public DatabaseConnection
{
    MYSQL * MySql;
};

class SERVER_DECL MySQLDatabase : public Database
{
    friend class QueryThread;
    friend class AsyncQuery;
public:
    MySQLDatabase();
    ~MySQLDatabase();

    bool Initialize(
        const char * hostname, uint32 port, 
        const char * username, const char * password, const char * dbname, 
        uint32 connectionCount, uint32 bufferSize);

    // e.g. hostname;port;username;password;dbname;connectionCount;buffersize
    bool Initialize(const char *connInfo);

    void Shutdown();

    string EscapeString(string Escape);
    void EscapeLongString(const char * str, uint32 len, stringstream& out);
    string EscapeString(const char * esc, DatabaseConnection * con);

    bool SupportsReplaceInto()  { return true; }
    bool SupportsTableLocking() { return true; }

protected:
    bool _HandleError(MySQLDatabaseConnection* conn, uint32 ErrorNumber);
    bool _SendQuery(DatabaseConnection *conn, const char* Sql, bool Self = false);

    void _BeginTransaction(DatabaseConnection * conn);
    void _EndTransaction(DatabaseConnection * conn);
    bool _Reconnect(MySQLDatabaseConnection * conn);

    QueryResult * _StoreQueryResult(DatabaseConnection * conn,  const char *sql);
};

class SERVER_DECL MySQLQueryResult : public QueryResult
{
public:
    MySQLQueryResult(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount);
    ~MySQLQueryResult();

    bool NextRow();

protected:
    MYSQL_RES* m_result;
};

#endif // MYSQL_DATABASE_H
