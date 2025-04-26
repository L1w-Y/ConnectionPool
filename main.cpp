#include <iostream>
#include"ConnectionPool.h"
int main()
{
	const std::string configPath = R"(D:\PROJECTS\C++project\ConnectionPool\config.ini)";
	ConfigMgr config;
	config.loadConfig(configPath);

	clock_t begin = clock();
	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	cp->init(config);
	
	thread t1([&]
		{
			auto sp = cp->getConnection();
			if (!sp) {
				std::cerr << "连接池没有拿到连接，线程退出\n";
				return;
			}
			for (int i = 0; i < 200; ++i)
			{
				char sql[1024] = { 0 };
				sprintf_s(sql, sizeof(sql),
					"INSERT INTO user(name, age, sex) VALUES('%s', %d, '%s')",
					"zhangshan", 20, "male");

				sp->update(sql);
			}
		});

	thread t2([&]
		{
			auto sp = cp->getConnection();
			if (!sp) {
				std::cerr << "连接池没有拿到连接，线程退出\n";
				return;
			}
			for (int i = 0; i <200; ++i)
			{
				char sql[1024] = { 0 };
				sprintf_s(sql, sizeof(sql),
					"INSERT INTO user(name, age, sex) VALUES('%s', %d, '%s')",
					"zhangshan", 20, "male");
				sp->update(sql);
			}
		});
	thread t3([&]
		{
		auto sp = cp->getConnection();
		if (!sp) {
			std::cerr << "连接池没有拿到连接，线程退出\n";
			return;
		}
			for (int i = 0; i < 200; ++i)
			{
				char sql[1024] = { 0 };
				sprintf_s(sql, sizeof(sql),
					"INSERT INTO user(name, age, sex) VALUES('%s', %d, '%s')",
					"zhangshan", 20, "male");
				
				sp->update(sql);
			}
		});

	thread t4([&]
		{
			auto sp = cp->getConnection();
			if (!sp) {
				std::cerr << "连接池没有拿到连接，线程退出\n";
				return;
			}
			for (int i = 0; i < 200; ++i)
			{
				char sql[1024] = { 0 };
				sprintf_s(sql, sizeof(sql),
					"INSERT INTO user(name, age, sex) VALUES('%s', %d, '%s')",
					"zhangshan", 20, "male");
				sp->update(sql);
			}
		});

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	clock_t end = clock();
	cout << end - begin << "ms\n";

	return 0;
}


