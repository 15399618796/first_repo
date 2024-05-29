#pragma once
#include <event2/bufferevent.h>
#include <string>

using namespace std;

#include "XTask.h"


struct bufferevent;

// 实现FTP任务的类，继承自XTask
class XFtpTask : public XTask
{
public:
	string curDir = "/";  // 当前目录，默认为根目录
	string rootDir = "/"; // 前置目录，默认为根目录
	string ip = "";
	int port = 0;
	XFtpTask *cmdTask = 0; // 指向一个命令任务的指针，用于处理特定的FTP命令

	// 虚函数，解析传入的字符串命令和参数
	virtual void Parse(std::string cmd, std::string arg) {}

	// 处理接收到的命令消息
	void ResCMD(string msg);

	// 建立连接并设置端口
    void ConnectoPORT();

    // 关闭与服务器的端口连接
    void ClosePORT();

    // 发送字符串数据
    void Send(const string &data);
    // 发送字符数组数据
    void Send(const char *data, size_t datasize);

    // 当事件发生时的处理函数，可被重写以提供具体实现
    virtual void Event(bufferevent *, short) {}

    // 当可以读取数据时的处理函数，可被重写以提供具体实现
    virtual void Read(bufferevent *) {}

    // 当可以写入数据时的处理函数，可被重写以提供具体实现
    virtual void Write(bufferevent *) {}

    // 设置回调函数，开启读写缓冲区
    void Setcb(struct bufferevent *);

    // 初始化方法，返回true表示成功
    bool Init() { return true; }

    // XFtpTask 析构函数
    ~XFtpTask();

protected:
    // 事件回调函数（静态）
    static void EventCB(bufferevent *, short, void *);
    // 读取回调函数（静态）
    static void ReadCB(bufferevent *, void *);
    // 写入回调函数（静态）
    static void WriteCB(bufferevent *, void *);

    // bufferevent 对象指针
    bufferevent *bev = 0;
    // 文件指针
    FILE *fp = 0;
};