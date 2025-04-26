#pragma once
#include<mysql.h>
#include<string.h>
#include<iostream>
#include"public.h"
using namespace std;

class mysql_connection
{
public:
    // ��ʼ�����ݿ�����
    mysql_connection();

    mysql_connection(const mysql_connection&) = delete;
    mysql_connection& operator=(const mysql_connection&) = delete;
    mysql_connection(mysql_connection&&) noexcept = default;
    mysql_connection& operator=(mysql_connection&&) noexcept = default;
    // �ͷ����ݿ�������Դ
    ~mysql_connection();
    // �������ݿ�
    bool connect(const std::string& ip, 
        unsigned short port,
        const std::string& user, 
        const std::string& password,
        const std::string& dbname);
    // ���²��� insert��delete��update
    bool update(const std::string& sql);
    // ��ѯ���� select   
    std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> query(const std::string& sql);

    void refreshAliveTime();
    clock_t getAliveTime()const;
private:    
    MYSQL* conn_; // ��ʾ��MySQL Server��һ������
    clock_t alivetime_;
};          

