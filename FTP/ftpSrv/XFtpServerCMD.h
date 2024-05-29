#pragma once
#include "XFtpTask.h"

#include <map>

// XFtpServerCMD 类继承自 XFtpTask，用于处理 FTP 服务器的命令。
class XFtpServerCMD : public XFtpTask
{
public:
    // 初始化函数，返回初始化是否成功。
    virtual bool Init();

    // 事件处理函数，接收 bufferevent 和事件类型参数。
    virtual void Event(bufferevent *be, short events);

    // 从 bufferevent 中读取数据的函数。
    virtual void Read(bufferevent *be);

    // 注册命令及其对应的 XFtpTask 处理函数。
    void Reg(std::string cmd, XFtpTask *call);

    // 构造函数
    XFtpServerCMD();
    
    // 析构函数
    ~XFtpServerCMD();
    
private:
    // 存储命令字符串与 XFtpTask 对象映射的容器。
    std::map<std::string, XFtpTask*> calls;
    
    // 存储待删除的 XFtpTask 对象映射的容器。
    std::map<XFtpTask*, int> calls_del;
};