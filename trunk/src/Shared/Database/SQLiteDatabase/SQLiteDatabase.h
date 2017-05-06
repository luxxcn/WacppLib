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

#ifndef SQLITE_DATABASE_H
#define SQLITE_DATABASE_H

#include "sqlite3/sqlite3.h"

struct SQLiteDatabaseConnection : public DatabaseConnection
{
    sqlite3 *SQLite;
};

class SERVER_DECL SQLiteDatabase : public Database
{
public:
    SQLiteDatabase();
    ~SQLiteDatabase();

    /**
     * hostname = sqlite file name
     * ignore other parameters
     * just use another function
     * TODO: need best idear
     */
    bool Initialize(
        const char * hostname, uint32 port, 
        const char * username, const char * password, const char * dbname, 
        uint32 connectionCount, uint32 bufferSize);

    /** connInfo = file name */
    bool Initialize(const char *connInfo);

    void Shutdown();

    /** 转义特殊字符,防止数据库攻击 sqlite封装中未实现 */
    string EscapeString(std::string str);
    void EscapeLongString(const char * str, uint32 len, stringstream& out);
    string EscapeString(const char * esc, DatabaseConnection * con);

    bool SupportsReplaceInto()  { return false; }
    bool SupportsTableLocking() { return false; }

protected:
    bool _SendQuery(DatabaseConnection *conn, const char* Sql, bool Self = false);

    void _BeginTransaction(DatabaseConnection * conn);
    void _EndTransaction(DatabaseConnection * conn);

    QueryResult * _StoreQueryResult(DatabaseConnection * conn, const char *sql);
};

class SERVER_DECL SQLiteQueryResult : public QueryResult
{
public:
    SQLiteQueryResult(char **res, uint32 FieldCount, uint32 RowCount);
    ~SQLiteQueryResult();

    bool NextRow();

protected:
    char **m_result;
    uint32 m_curRowId;
};

#endif // SQLITE_DATABASE_H

