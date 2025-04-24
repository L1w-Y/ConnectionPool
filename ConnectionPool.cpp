#include "ConnectionPool.h"
ConnectionPool* ConnectionPool::getConnectionPool() {
	static ConnectionPool pool;
	return &pool;
}

void ConnectionPool::init(const ConfigMgr& config)
{
    if (initialized_) return;
    ip_ = config.ip;
    port_ = config.port;
    dbname_ = config.dbname;
    userName_ = config.username;
    password_ = config.password;
    initSize_ = config.initSize;
    maxSize_ = config.maxSize;
    maxIdleTime_ = config.maxIdleTime;
    connectionTimeOut_ = config.maxConnectionTimeOut;
    initialized_ = true;

    for (int i = 0; i < initSize_; ++i) {
        auto p = make_shared<mysql_connection>();
        if (p->connect(ip_, port_, userName_, password_, dbname_)) {
            connectionQue_.push(std::move(p));
            connectionCount_++;
        }
        else {
            std::cerr << "连接池创建连接失败 " << std::endl;
        }  
    }
} 
