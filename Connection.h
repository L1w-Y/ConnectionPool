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
private:    
    MYSQL* conn_; // ��ʾ��MySQL Server��һ������
};          

