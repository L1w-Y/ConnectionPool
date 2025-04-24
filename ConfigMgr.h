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
    // ���������ļ�
    void loadConfig(const std::string& filePath) {
        std::ifstream configFile(filePath);
        if (!configFile.is_open()) {
            throw std::runtime_error("�޷��������ļ�: " + filePath);
        }

        std::unordered_map<std::string, std::string> configMap;
        std::string line;

        while (std::getline(configFile, line)) {
            // �������к�ע��
            if (line.empty() || line[0] == '#') {
                continue;
            }

            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                // ȥ������ֵ���˵Ŀհ��ַ�
                key = trim(key);
                value = trim(value);
                configMap[key] = value;
            }
        }

        // ����������
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
            throw std::runtime_error("�����ļ���ȱ�ٱ�Ҫ��������: " + std::string(e.what()));
        }
        catch (const std::invalid_argument& e) {
            throw std::runtime_error("�������ʽ����: " + std::string(e.what()));
        }
    }

    // ��ӡ������Ϣ
    void printConfig() const {
        std::cout << "���ݿ����ӳ�����:\n"
            << "IP: " << ip << "\n"
            << "�˿�: " << port << "\n"
            << "�û���: " << username << "\n"
            << "����: " << password << "\n"
            << "��ʼ��С: " << initSize << "\n"
            << "����С: " << maxSize << "\n"
            << "������ʱ��: " << maxIdleTime << "\n"
            << "���ӳ�ʱʱ��: " << maxConnectionTimeOut << std::endl;
    }
    
private:
    // ����������ȥ���ַ������˵Ŀհ��ַ�
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (std::string::npos == first) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }
};

