#pragma once
#include<string.h>
#include<iostream>
#include"ConfigMgr.h"
#include"Connection.h"
#include<mutex>
#include<queue>
#include<thread>
class ConnectionPool
{
public:
	static ConnectionPool* getConnectionPool();
	void init(const ConfigMgr& config);
	ConnectionPool(const ConnectionPool&) = delete;
	ConnectionPool operator=(const ConnectionPool&) = delete;
	ConnectionPool(ConnectionPool&&) = delete;
	ConnectionPool& operator=(ConnectionPool&&) = delete;
private:
	ConnectionPool()=default;
	unsigned short port_;
	std::string ip_;
	std::string userName_;
	std::string password_;
	int initSize_;
	int maxSize_;
	std::string dbname_;
	int maxIdleTime_; //������ʱ��
	int connectionTimeOut_;//��ȡ���ӵĳ�ʱʱ��
	bool initialized_{};
	atomic_int connectionCount_;
	std::queue<std::shared_ptr<mysql_connection>> connectionQue_;//���Ӷ���
	mutex queueMutex_;
};

