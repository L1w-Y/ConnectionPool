#include <iostream>
#include"ConnectionPool.h"
int main()
{
	const std::string configPath = R"(D:\PROJECTS\C++project\ConnectionPool\config.ini)";
	ConfigMgr config;
	config.loadConfig(configPath);
	config.printConfig();
	return 0;
}


