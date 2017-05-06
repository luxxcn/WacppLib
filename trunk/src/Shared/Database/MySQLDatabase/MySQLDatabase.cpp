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

#include "MySQLDatabase.h"

MySQLDatabase::MySQLDatabase() : Database()
{

}

MySQLDatabase::~MySQLDatabase()
{
    for(int32 i = 0; i < m_connectionCount; i++)
    {
        mysql_close(((MySQLDatabaseConnection*)m_connections[i])->MySql);
        delete m_connections[i];
    }
    delete [] m_connections;
}

bool MySQLDatabase::Initialize(
        const char * hostname, uint32 port, 
        const char * username, const char * password, const char * dbname, 
        uint32 connectionCount, uint32 bufferSize)
{
    // 用于连接失效时进行重连
    m_hostname = hostname;
    m_port     = port;
    m_dbname   = dbname;
    m_username = username;
    m_password = password;

    m_connectionCount = connectionCount;

    MYSQL * temp = NULL;
    MYSQL * temp2 = NULL;
    MySQLDatabaseConnection ** conns;
    my_bool my_true = true;// 设置mysql属性可重连

    sLog.Detail("MySQL", "连接到`%s:%u`,数据库名`%s`", hostname, port, dbname);

    conns = new MySQLDatabaseConnection*[connectionCount];
    m_connections = ((DatabaseConnection**)conns);
    for(uint32 i = 0; i < connectionCount; i++)
    {
        temp = mysql_init(NULL);
        if(!temp)
            continue;

        if(mysql_options(temp, MYSQL_SET_CHARSET_NAME, "utf8"))
            sLog.Error("MySQL", "设置`utf8`编码失败");

        if(mysql_options(temp, MYSQL_OPT_RECONNECT, &my_true))
            sLog.Error("MySQL", "设置允许重连失败");

        temp2 = mysql_real_connect(temp, hostname, username, password, dbname, port, NULL, 0);
        if(!temp2)
        {
            sLog.outError("MySQL", "连接失败,原因:`%s`", mysql_error(temp));
            mysql_close(temp);
            return false;
        }

        conns[i] = new MySQLDatabaseConnection;
        conns[i]->MySql = temp2;
    }

    _Initialize();
    return true;
}

bool MySQLDatabase::Initialize(const char *connInfo)
{
    vector<string> cInfo = StrSplit(connInfo, ";");
    vector<string>::const_iterator itr = cInfo.begin();

    string hostname, port, username, password, dbname, connectionCount, buffersize;

    if(itr != cInfo.end())
        hostname = *itr++;
    if(itr != cInfo.end())
        port = *itr++;
    if(itr != cInfo.end())
        username = *itr++;
    if(itr != cInfo.end())
        password = *itr++;
    if(itr != cInfo.end())
        dbname = *itr++;
    if(itr != cInfo.end())
        connectionCount = *itr++;
    if(itr != cInfo.end())
        buffersize = *itr;

    return Initialize(hostname.c_str(), (uint32)atoi(port.c_str()), 
        username.c_str(), password.c_str(), dbname.c_str(), 
        (uint32)atoi(connectionCount.c_str()), (uint32)atoi(buffersize.c_str()));

}

void MySQLDatabase::Shutdown()
{
    mysql_library_end();
}

string MySQLDatabase::EscapeString(string Escape)
{
    char buf[16384];
    string ret;

    DatabaseConnection * conn = GetFreeConnection();
    if(!mysql_real_escape_string(((MySQLDatabaseConnection*)conn)->MySql, buf, Escape.c_str(), (unsigned long)Escape.length()))
        ret = Escape.c_str();
    else
        ret = buf;

    conn->Busy.Release();
    return ret;
}

void MySQLDatabase::EscapeLongString(const char * str, uint32 len, stringstream& out)
{
    char buf[65536*3] = { 0 };

    DatabaseConnection * conn = GetFreeConnection();
    const char * ret;
    if(!mysql_real_escape_string(((MySQLDatabaseConnection*)conn)->MySql, buf, str, (unsigned long)len))
        ret = str;
    else
        ret = buf;

    out.write(ret, (std::streamsize)strlen(ret));
    conn->Busy.Release();
}

string MySQLDatabase::EscapeString(const char * esc, DatabaseConnection * con)
{
    char buf[16384] = { 0 };
    const char * ret;
    if(mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(con)->MySql, buf, (char*)esc, (unsigned long)strlen(esc)) == 0)
        ret = esc;
    else
        ret = buf;

    return string(ret);
}

bool MySQLDatabase::_Reconnect(MySQLDatabaseConnection * conn)
{
    MYSQL * temp, * temp2;

    temp = mysql_init(NULL);
    temp2 = mysql_real_connect(temp, m_hostname.c_str(), m_username.c_str(), m_password.c_str(), m_dbname.c_str(), m_port, NULL, 0);
    if(!temp2)
    {
        sLog.Error("MySQL", "重连接失败:%s", mysql_error(temp));
        mysql_close(temp);
        return false;
    }

    if(conn->MySql)
        mysql_close(conn->MySql);

    conn->MySql = temp;
    return true;
}

bool MySQLDatabase::_HandleError(MySQLDatabaseConnection * conn, uint32 ErrorNumber)
{
    switch(ErrorNumber)
    {
    case 2006: // 服务端离线
    case 2008: // 客户端内存耗尽
    case 2013: // 查询时失去连接
    case 2055: // 失去连接 - 系统错误
        {
            return _Reconnect(conn);
        }
        break;
    }

    return false;
}
MySQLQueryResult::MySQLQueryResult(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount)
: QueryResult(FieldCount, RowCount), m_result( res )
{
    m_currentRow = new Field[FieldCount]; 
}

MySQLQueryResult::~MySQLQueryResult()
{
    mysql_free_result(m_result);
    delete [] m_currentRow;
}

bool MySQLQueryResult::NextRow()
{
    MYSQL_ROW row = mysql_fetch_row(m_result);
    if(!row)
        return false;

    for(uint32 i = 0; i < m_fieldCount; i++)
        m_currentRow[i].SetValue(row[i]);

    return true;
}

bool MySQLDatabase::_SendQuery(DatabaseConnection *conn, const char* Sql, bool Self)
{
    int result = mysql_query(static_cast<MySQLDatabaseConnection*>(conn)->MySql, Sql);
    if(result > 0)
    {
        if(Self == false && _HandleError(static_cast<MySQLDatabaseConnection*>(conn), mysql_errno(static_cast<MySQLDatabaseConnection*>(conn)->MySql)))
        {
            // 重连成功后再次查询
            result = _SendQuery(conn, Sql, true);
        }
        else
            sLog.Error("MySQL", "执行查询失败:%s,语句:%s", mysql_error(static_cast<MySQLDatabaseConnection*>(conn)->MySql), Sql);
    }

    return (result == 0 ? true : false);
}

QueryResult * MySQLDatabase::_StoreQueryResult(DatabaseConnection * con,  const char *sql)
{
    MySQLQueryResult * res;

    MySQLDatabaseConnection * conn = static_cast<MySQLDatabaseConnection*>(con);

    MYSQL_RES * myres = mysql_store_result(conn->MySql);
    uint32 rowCount   = (uint32)mysql_affected_rows(conn->MySql);
    uint32 fieldCount = (uint32)mysql_field_count(conn->MySql);
    
    if(!myres || !rowCount || !fieldCount)
    {
        if(!myres)
            mysql_free_result(myres);
        return NULL;
    }

    res = new MySQLQueryResult(myres, fieldCount, rowCount);
    res->NextRow();

    return res;
}

void MySQLDatabase::_BeginTransaction(DatabaseConnection * conn)
{
    _SendQuery(conn, "START TRANSACTION", false);
}

void MySQLDatabase::_EndTransaction(DatabaseConnection * conn)
{
    _SendQuery(conn, "COMMIT", false);
}
