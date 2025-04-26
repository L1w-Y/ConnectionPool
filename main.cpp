#include <iostream>
#include"ConnectionPool.h"
int main()
{
	const std::string configPath = R"(D:\PROJECTS\C++project\ConnectionPool\config.ini)";
	ConfigMgr config;
	config.loadConfig(configPath);

	clock_t begin = clock();
	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	for (int i =0;i<1000;++i)
	{
		cp->init(config);
		auto sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf_s(sql, sizeof(sql),
			"INSERT INTO user(name, age, sex) VALUES('%s', %d, '%s')",
			"zhangshan", 20, "male");
		sp->update(sql);
	}
	clock_t end = clock();
	cout << end - begin << "ms\n";

	return 0;
}


