#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <stdexcept>
class ConfigMgr
{
public:
    std::string ip;
    int port;
    std::string dbname;
    std::string username;
    std::string password;
    int initSize;
    int maxSize;
    int maxIdleTime;
    int maxConnectionTimeOut;
    // 加载配置文件
    void loadConfig(const std::string& filePath) {
        std::ifstream configFile(filePath);
        if (!configFile.is_open()) {
            throw std::runtime_error("无法打开配置文件: " + filePath);
        }

        std::unordered_map<std::string, std::string> configMap;
        std::string line;

        while (std::getline(configFile, line)) {
            // 跳过空行和注释
            if (line.empty() || line[0] == '#') {
                continue;
            }

            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                // 去除键和值两端的空白字符
                key = trim(key);
                value = trim(value);
                configMap[key] = value;
            }
        }

        // 解析配置项
        try {
            ip = configMap.at("ip");
            port = std::stoi(configMap.at("port"));
            dbname = configMap.at("dbname");
            username = configMap.at("username");
            password = configMap.at("password");
            initSize = std::stoi(configMap.at("initSize"));
            maxSize = std::stoi(configMap.at("maxSize"));
            maxIdleTime = std::stoi(configMap.at("maxIdleTime"));
            maxConnectionTimeOut = std::stoi(configMap.at("MaxConnectionTimeOut"));
        }
        catch (const std::out_of_range& e) {
            throw std::runtime_error("配置文件中缺少必要的配置项: " + std::string(e.what()));
        }
        catch (const std::invalid_argument& e) {
            throw std::runtime_error("配置项格式错误: " + std::string(e.what()));
        }
    }

    // 打印配置信息
    void printConfig() const {
        std::cout << "数据库连接池配置:\n"
            << "IP: " << ip << "\n"
            << "端口: " << port << "\n"
            << "用户名: " << username << "\n"
            << "密码: " << password << "\n"
            << "初始大小: " << initSize << "\n"
            << "最大大小: " << maxSize << "\n"
            << "最大空闲时间: " << maxIdleTime << "\n"
            << "连接超时时间: " << maxConnectionTimeOut << std::endl;
    }
    
private:
    // 辅助函数：去除字符串两端的空白字符
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (std::string::npos == first) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }
};

