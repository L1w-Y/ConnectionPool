#include "Connection.h"

mysql_connection::mysql_connection()
{
    conn_ = mysql_init(nullptr);
    if (!conn_) {
        throw std::runtime_error("mysql_init failed");
    }
}

mysql_connection::~mysql_connection()
{
    if (conn_) {
        mysql_close(conn_);
    }
}

bool mysql_connection::connect(const std::string& ip, 
    unsigned short port, 
    const std::string& user, 
    const std::string& password, 
    const std::string& dbname)
{
    if (!mysql_real_connect(conn_, ip.c_str(), user.c_str(),
        password.c_str(), dbname.c_str(), port, nullptr, 0)) {
        std::cerr << "连接失败: " << mysql_error(conn_) << std::endl;
        return false;
    }
    return true;
}

bool mysql_connection::update(const std::string& sql)
{       
    if (!conn_) return false;
    if (mysql_query(conn_, sql.c_str())) {
        std::cerr << "更新失败: " << mysql_error(conn_) << std::endl;
        return false;
    }
    return true;
}

std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> mysql_connection::query(const std::string& sql)
{
    if (!conn_) return { nullptr, mysql_free_result };
    if (mysql_query(conn_, sql.c_str())) {
        std::cerr << "查询失败: " << mysql_error(conn_) << std::endl;
        return { nullptr, mysql_free_result };
    }
    return { mysql_use_result(conn_), mysql_free_result };
}
