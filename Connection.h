#pragma once
#include<mysql.h>
#include<string.h>
#include<iostream>
#include"public.h"
using namespace std;

class mysql_connection
{
public:
    // 初始化数据库连接
    mysql_connection();

    mysql_connection(const mysql_connection&) = delete;
    mysql_connection& operator=(const mysql_connection&) = delete;
    mysql_connection(mysql_connection&&) noexcept = default;
    mysql_connection& operator=(mysql_connection&&) noexcept = default;
    // 释放数据库连接资源
    ~mysql_connection();
    // 连接数据库
    bool connect(const std::string& ip, 
        unsigned short port,
        const std::string& user, 
        const std::string& password,
        const std::string& dbname);
    // 更新操作 insert、delete、update
    bool update(const std::string& sql);
    // 查询操作 select   
    std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> query(const std::string& sql);

    void refreshAliveTime();
    clock_t getAliveTime()const;
private:    
    MYSQL* conn_; // 表示和MySQL Server的一条连接
    clock_t alivetime_;
};          

