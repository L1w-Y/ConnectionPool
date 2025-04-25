#include "ConnectionPool.h"

ConnectionPool::~ConnectionPool()
{
    isRunning = false;
}

ConnectionPool* ConnectionPool::getConnectionPool() {
	static ConnectionPool pool;
	return &pool;
}

void ConnectionPool::init(const ConfigMgr& config)
{
    if (initialized_) return;
    ip_ = config.ip;
    port_ = config.port;
    dbName_ = config.dbname;
    userName_ = config.username;
    passWord_ = config.password;
    initSize_ = config.initSize;
    maxSize_ = config.maxSize;
    maxIdleTime_ = config.maxIdleTime;
    connectionTimeOut_ = config.maxConnectionTimeOut;
    initialized_ = true;

    for (int i = 0; i < initSize_; ++i) {
        auto p = make_shared<mysql_connection>();
        if (p->connect(ip_, port_, userName_, passWord_, dbName_)) {
            connectionQue_.push(std::move(p));
            ++connectionCount_;
        }
        else {
            std::cerr << "连接池创建连接失败\n";
        }  
    }

    std::thread produce([this] {produceTask(); });
}
void ConnectionPool::produceTask()
{
    while(isRunning) {
        unique_lock<mutex> lock(queueMutex_);
        cv.wait(lock, [this]
        {
	        return connectionQue_.empty() && connectionCount_< maxSize_;
        });

        auto p = make_shared<mysql_connection>();
        if (p->connect(ip_, port_, userName_, passWord_, dbName_)) { 
            connectionQue_.push(std::move(p));
            ++connectionCount_;
            cv.notify_all();
        }else {
            std::cerr << "创建连接失败！\n";
            
        }

    }
}

