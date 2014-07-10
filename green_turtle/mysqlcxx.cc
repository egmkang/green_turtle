#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mysqlcxx.h"

using namespace green_turtle;

SqlCommand& MySqlCommand::SetParam(int index, void *data, int len)
{
    assert(index >= 0 && data && len > 0);
    SetParam(index, MYSQL_TYPE_BLOB, data, len);
    return *this;
}

SqlCommand& MySqlCommand::SetParam(int index, const char *data, int len)
{
    assert(index >= 0 && data && len > 0);
    SetParam(index, MYSQL_TYPE_VAR_STRING, data, len);
    return *this;
}

void MySqlCommand::SetParam(int index, int type, const void *data, int len)
{
    if(this->params.size() <= (size_t)index)
        this->params.resize(index + 1);
    SqlParamType &param = params[index];
    param.param_type = type;
    param.data.clear();
    param.data.append((const char*)data, ((const char*)data) + len);
}

MySqlConnection::MySqlConnection()
{
    memset(&conn_, 0, sizeof(conn_));
    mysql_init(&conn_);
    mysql_options(&conn_, MYSQL_OPT_RECONNECT, "1");
}

MySqlConnection::~MySqlConnection()
{
    mysql_close(&conn_);
    memset(&conn_, 0, sizeof(conn_));
    mysql_init(&conn_);
    mysql_options(&conn_, MYSQL_OPT_RECONNECT, "1");
}

int MySqlConnection::Connect(const char* host, unsigned short port, const char* user, const char* passwd, const char* db_name)
{
    assert(host && user && passwd && db_name);
    MYSQL *mysql = mysql_real_connect(&conn_, host, user, passwd, db_name, (unsigned int)(port), NULL, 0);
    if(!mysql) return -1;
    mysql_select_db(&conn_, db_name);
    return 0;
}

int MySqlConnection::ExecSql(const char *sql)
{
    assert(sql);
    int ret = mysql_real_query(&conn_, sql, (unsigned long)strlen(sql));
    if(ret)
    {
        return -mysql_errno(&conn_);
    }
    return mysql_affected_rows(&conn_);;
}

int MySqlConnection::ExecSql(const SqlCommand &command)
{
    MYSQL_STMT *stmt = mysql_stmt_init(&conn_);
    if(stmt)
    {
        const std::string& sql = command.GetSql();
        mysql_stmt_prepare(stmt, sql.c_str(), sql.size());

        const std::vector<SqlParamType>& params = command.GetParams();
        std::vector<MYSQL_BIND> binds;
        binds.resize(params.size());
        for(unsigned idx = 0; idx < params.size(); ++idx)
        {
            memset(&binds[idx], 0, sizeof(binds[idx]));
            binds[idx].buffer_type  = (enum_field_types)params[idx].param_type;
            binds[idx].buffer       = params[idx].ptr();
            binds[idx].buffer_length= params[idx].size();
        }
        mysql_stmt_bind_param(stmt, &binds[0]);
        int ret = mysql_stmt_execute(stmt);
        if(ret)
        {
            mysql_stmt_errno(stmt);
            return -mysql_errno(&conn_);
        }
        int rows = mysql_stmt_affected_rows(stmt);
        mysql_stmt_close(stmt);
        return rows;
    }
    return -1;
}

unsigned long long MySqlConnection::GetNewID()
{
    return mysql_insert_id(&conn_);
}

ResultSet* MySqlConnection::ExecSelect(const char *sql)
{
    int ret = mysql_real_query(&conn_, sql, (unsigned long)strlen(sql));
    if(ret) return NULL;
    MYSQL_RES *res = mysql_store_result(&conn_);
    if(res)
        return new MySqlResultSet(res);
    return NULL;
}

int MySqlConnection::EscapeString(char *to,const char *from, unsigned long length)
{
    assert(to && from && length && to != from);
    return mysql_real_escape_string(&conn_, to, from, length);
}

MySqlResultSet::MySqlResultSet(MYSQL_RES *result) :
    result(result, [](MYSQL_RES *res){mysql_free_result(res);}),
    field_len(NULL),
    total_fields(0),
    total_rows(0),
    current_row(-1)
{
    if(result)
    {
        this->total_fields = mysql_num_fields(result);
        this->total_rows = mysql_num_rows(result);
        if(total_rows)
        {
            current_row = 0;
            row = mysql_fetch_row(result);
            field_len = mysql_fetch_lengths(result);
        }
    }
}

MySqlResultSet::~MySqlResultSet()
{
    //mysql_free_result(result);
}

int  MySqlResultSet::FieldCount() const
{
    return this->total_fields;
}

int  MySqlResultSet::RowCount() const
{
    return this->total_rows;
}

void MySqlResultSet::Next() const
{
    if(this->current_row < this->total_rows)
    {
        ++this->current_row;
        row = mysql_fetch_row(result.get());
        field_len = mysql_fetch_lengths(result.get());
    }
}

bool MySqlResultSet::IsValid() const
{
    if(this->total_fields > 0 &&
            this->field_len &&
            this->total_rows > 0 &&
            this->current_row >= 0 &&
            this->current_row < this->total_rows)
        return true;
    return false;
}

SqlResultVarType MySqlResultSet::operator[] (int field) const
{
    if(!IsValid() || field >= this->total_fields)
        return SqlResultVarType(NULL, 0);
    const char *data = this->row[field];
    int len = (unsigned long)this->field_len[field];
    if(len <= 0) data = NULL;
    return SqlResultVarType(data, len);
}

SqlResultVarType MySqlResultSet::at(int field) const
{
  return (*this)[field];
}
