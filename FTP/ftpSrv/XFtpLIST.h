#pragma once
#include "XFtpTask.h" // 引入XFtpTask基类的头文件
#include <string> // 引入string标准库

// XFtpLIST类继承自XFtpTask，用于处理FTP服务器的LIST命令
class XFtpLIST : public XFtpTask
{
public:
    // 解析LIST命令的请求和响应字符串
    void Parse(std::string request, std::string response) override;

    // 处理与LIST命令相关的事件
    void Event(bufferevent *buffev, short event) override;

    // 向客户端写入LIST命令的响应数据
    void Write(bufferevent *buffev) override;

private:
    // 根据给定的路径获取FTP服务器的文件列表数据
    string GetListData(std::string path);
};