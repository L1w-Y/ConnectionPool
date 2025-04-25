#pragma once
#include<string.h>
#include<iostream>
#include"ConfigMgr.h"
#include"Connection.h"
#include<mutex>
#include<queue>
#include<thread>
#include<memory>
#include<functional>
#include<condition_variable>
class ConnectionPool
{
public:
	//获取连接池实列
	static ConnectionPool* getConnectionPool();
	//初始化连接池
	void init(const ConfigMgr& config);
	//外部接口，获取空闲连接
	//生产者线程
	void produceTask();
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
	int maxIdleTime_; //最大空闲时间
	int connectionTimeOut_;//获取连接的超时时间
	bool initialized_{};
	atomic_int connectionCount_;
	std::queue<std::shared_ptr<mysql_connection>> connectionQue_;//连接队列
	mutex queueMutex_;
	condition_variable cv; //条件变量
};

