#include "ConnectionPool.h"

ConnectionPool::~ConnectionPool()
{
    isRunning = false;
    while (!connectionQue_.empty())
    {
        auto p = connectionQue_.front();
        connectionQue_.pop();
        delete p;
    }
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
        auto p = new mysql_connection();
        if (p->connect(ip_, port_, userName_, passWord_, dbName_)) {
            p->refreshAliveTime();
            connectionQue_.push(p);
            ++connectionCount_;
        }
        else {
            std::cerr << "连接池创建连接失败\n";
        }  
    }
    
    std::thread produce([this] {produceTask(); });
    produce.detach();
    std::thread scanner([this] {scannerTask(); });
    scanner.detach();
}

shared_ptr<mysql_connection> ConnectionPool::getConnection()
{

    unique_lock<mutex> lock(queueMutex_);
    if (connectionQue_.empty())
    {
        cv.wait_for(lock, chrono::milliseconds(connectionTimeOut_));
        if (connectionQue_.empty())
        {
            LOG("等待超时，获取连接失败")
                return nullptr;
        }
    }
    // 获取队列中的shared_ptr
    shared_ptr<mysql_connection> connPtr(connectionQue_.front(),
        [&](mysql_connection *ptr){
            unique_lock<mutex> lock(queueMutex_);
            ptr->refreshAliveTime();
            connectionQue_.push(ptr);
    });
    connectionQue_.pop();
    cv.notify_all();
    return connPtr;
}

void ConnectionPool::produceTask()
{
    while(isRunning) {
        unique_lock<mutex> lock(queueMutex_);
        cv.wait(lock, [this]
        {
	        return connectionQue_.empty() && connectionCount_< maxSize_;
        });

    	auto p = new mysql_connection();
        if (p->connect(ip_, port_, userName_, passWord_, dbName_)) {
            p->refreshAliveTime();
            connectionQue_.push(p);
            ++connectionCount_;
            cv.notify_all();
        }else {
            std::cerr << "创建连接失败！\n";
            
        }

    }
}

void ConnectionPool::scannerTask()
{
    while (isRunning)
    {
        this_thread::sleep_for(chrono::seconds(maxIdleTime_));
        unique_lock<mutex> lock(queueMutex_);
        while(connectionCount_>initSize_)
        {
            auto p = connectionQue_.front();
            if (p->getAliveTime()>=maxIdleTime_*1000)
            {
                connectionQue_.pop();
                delete p;
                --connectionCount_;
            }else
            {
	            break;
            }
        }
    }
}

