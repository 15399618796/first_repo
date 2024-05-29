#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::flush;
#include <string.h>
#include <string>
using std::string;

#include <stdlib.h>
#include <signal.h>
#include <fstream>

#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/buffer.h>
#include <event2/util.h>

#include "testUtil.h"
#include "XThreadPool.h"
#include "XThread.h"
#include "XTask.h"
#include "XFtpFactory.h"

#define SPORT 21
#define BUFS 1024
#define errmsg(msg) do{cout<< msg << endl; exit(1);}while(0)
#define XThreadPoolGet XThreadPool::Get()


// 连接成功，开始通信
void listen_cb(struct evconnlistener *ev, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg) {
	cout << endl;
	testout("main thread At listen_cb");

	sockaddr_in *sin = (sockaddr_in*)addr;
	// 创建任务
	XTask *task = XFtpFactory::Get()->CreateTask();
	task->sock = fd; //通信套接字

    // 分配任务
	XThreadPool::Get()->Dispatch(task);
}

int main() {
	// 设置SIGPIPE信号的处理方式为忽略方式。
	// 防止程序在发送数据到已经关闭的管道时接收到 SIGPIPE 信号，从而导致程序异常退出
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		errmsg("signal(SIGPIPE, SIG_IGN) error!");

	// 初始化线程池
	XThreadPoolGet->Init(10);

	event_base *base = event_base_new();
	// 检查是否成功创建了事件基础结构
	if (!base)
		errmsg("main thread event_base_new error");

    //创建libevent上下文
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);
	sin.sin_addr.s_addr = INADDR_ANY;

	//创建监听事件
	evconnlistener *ev = evconnlistener_new_bind(
		base,									   // libevent的上下文
		listen_cb,								   // 接收到连接的回调函数
		base,									   // 回调函数获取的参数 arg
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, // 地址重用，evconnlistener关闭同时关闭socket
		10,										   // 连接队列大小，对应listen函数
		(sockaddr *)&sin,						   // 绑定的地址和端口
		sizeof(sin)
	);

	// 输出事件和错误信息
	cout << endl;
	cout << "ev:" << ev << endl;
	cout << "err:" << errno << endl;
	perror("err");

	// 如果base非空，开始监听
	if (base) {
		cout << "begin to listen..." << endl;
		cout << "current thread : " << getpid() << endl; // 输出当前进程ID
		cout << event_base_dispatch(base) << endl; // 启动事件循环，分发事件
	}

	cout << "event_base_dispatch end..." << endl;
	// 释放事件监听器和事件基础资源
	if (ev)
		evconnlistener_free(ev);
	if (base)
		event_base_free(base);

	// 输出测试结束信息
	testout("server end");
	return 0;
}