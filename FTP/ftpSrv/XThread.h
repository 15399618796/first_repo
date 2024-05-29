#pragma once
#include <event2/util.h> // 引入libevent的工具函数头文件
#include <list>
#include <mutex>
class XTask; // 声明XTask类，具体实现不在此文件中
struct event_base; // 声明event_base结构体，用于libevent事件基础

/**
 * XThread类用于实现线程的封装，管理任务列表，并提供线程启动、激活任务等接口。
 */
class XThread
{
public:
    // 启动线程。该方法会创建一个event_base，并初始化相关资源，然后调用Main方法进入线程主循环。
	void Start();

    // 线程主函数。该函数在一个无限循环中等待事件，处理任务。
	void Main();

    // 设置线程环境。初始化线程的通知发送socket和事件基础。
	bool Setup();

    /**
     * 通知线程有新任务需要处理。
     * 通过发送信号到notify_send_fd来通知线程。
     * @param fd 通知发送的socket文件描述符。
     * @param events 事件类型。
     */
	void Notify(evutil_socket_t fd, short events);

    // 激活一个任务。将任务加入到任务列表中，等待处理。
	void Activate();

    // 添加一个任务到线程的任务列表。
	void AddTack(XTask *task);

    // 构造函数和析构函数
	XThread();
	~XThread();

	int id = 0; // 线程ID，用于标识不同的线程

private:
	int notify_send_fd = 0; // 用于线程间通知的发送socket文件描述符
	event_base *base = 0; // libevent的事件基础，用于事件处理
	std::list<XTask*> tasks; // 任务列表，存储待处理的任务
	std::mutex tasks_mutex; // 任务列表的互斥锁，用于线程安全访问
};