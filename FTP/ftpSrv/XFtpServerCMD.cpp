#include <string.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>

#include <string>
using namespace std;

#include "XFtpServerCMD.h"
#include "testUtil.h"

#define BUFS 4096

// 注册FTP命令及其处理函数
void XFtpServerCMD::Reg(std::string cmd, XFtpTask *call) { 
    testout("At XFtpServerCMD::Reg");

    // 检查传入的处理函数是否为nullptr
    if (!call) {
        cout << "XFtpServerCMD::Reg call is null " << endl;
        return;
    }
    // 检查传入的命令字符串是否为空
    if (cmd.empty()) {
        cout << "XFtpServerCMD::Reg cmd is null " << endl;
        return;
    }

    // 检查命令是否已经注册
    if (calls.find(cmd) != calls.end()) {
        cout << cmd << " is alredy register" << endl;
        return;
    }

    // 注册命令成功，设置处理函数的相关属性并记录
    testout(cmd << " Reg success");
    call->base = base;
    call->cmdTask = this;
    calls[cmd] = call; //命令的名字和处理函数的映射关系
    calls_del[call] = 0;
}

// 处理bufferevent触发的特定事件，如EOF、错误或超时，并在处理后删除当前对象。
void XFtpServerCMD::Event(bufferevent *bev, short events) {
	testout("At XFtpServerCMD::Event");

	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
		delete this;
	}
}

// 读取客户端发送的FTP命令，并进行解析处理
void XFtpServerCMD::Read(bufferevent *bev)
{
	cout << endl;
	testout("At XFtpServerCMD::Read"); // 进入函数时的日志输出

	char buf[BUFS] = {0};			   // 用于存储从bufferevent中读取的数据
	while (1)
	{
		int len = bufferevent_read(bev, buf, BUFS); // 从bufferevent读取数据到buf
		if (len <= 0)
			break;
		cout << "Recv CMD(" << len << "):" << buf; 

		string type = ""; // 用于存储命令类型
		for (int i = 0; i < len; i++)
		{
			if (buf[i] == ' ' || buf[i] == '\r') // 当遇到空格或回车时，停止提取命令类型
				break;
			type += buf[i];
		}

		cout << "type is [" << type << "]" << endl; // 打印命令类型
		if (calls.find(type) != calls.end())
		{										   // 检查是否存在该命令类型的处理对象
			testout("begin to parse");			   // 开始解析命令的日志输出
			XFtpTask *t = calls[type];			   // 获取对应命令类型的处理对象
			t->Parse(type, buf);				   // 调用处理对象的解析方法
			testout("curDir: [" << curDir << "]"); // 解析后的日志输出，包括当前目录
		}
		else
		{
			cout << "parse object not found" << endl; // 如果未找到处理对象，输出错误信息
			ResCMD("200 OK\r\n");					  // 发送默认的OK响应
		}
	}
}

// 该函数创建一个bufferevent对象用于处理FTP服务器的网络事件，设置超时时间，并向客户端发送欢迎消息。
bool XFtpServerCMD::Init() {
    testout("At XFtpServerCMD::Init");

    // 创建一个新的bufferevent对象，用于处理基于套接字的事件，配置为在释放时关闭套接字
    bufferevent *bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        // 如果创建失败，删除当前对象并返回false
        delete this;
        return false;
    }

    // 设置读写超时时间
    timeval t = {300, 0};
    bufferevent_set_timeouts(bev, &t, 0);

    // 构造并发送欢迎消息给客户端
    string msg = "220 Welcome to XFtpServer\r\n";
    bufferevent_write(bev, msg.c_str(), msg.size());

    // 设置命令处理任务和bufferevent对象
    this->cmdTask = this;
    this->bev = bev;

    // 设置bufferevent的回调函数
    Setcb(bev);

    return true;
}

XFtpServerCMD::XFtpServerCMD() {
}

XFtpServerCMD::~XFtpServerCMD() {
	ClosePORT();
	for (auto i : calls_del) {
		delete i.first;
	}
}
