#include <thread>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <event2/event.h>

#include "testUtil.h"
#include "XThread.h"
#include "XTask.h"

// 定义回调函数，当监听套接字有事件时调用
static void Notify_cb(evutil_socket_t fd, short which, void *arg) {
    XThread *t = static_cast<XThread*>(arg);
    t->Notify(fd, which);
}

// 通知线程有新任务
void XThread::Notify(evutil_socket_t fd, short which)
{
	testout(id << " thread At Notify()");

	char buf[2] = {0};
    //这里buf会读到数据"C",是因为在下面的Activate()函数中会发送数据"C"
	int ret = read(fd, buf, sizeof(buf)); 
	if (ret < 0){
        testout(id << " thread At Notify() read error");
        return ;
    }
	cout << id << " thread recv : " << buf << endl;
}

// 启动线程并开始运行
void XThread::Start() {
    testout(id << " thread At Start()");

    Setup();
    // 创建线程并初始化，准备调用XThread的Main方法
    std::thread th(&XThread::Main, this);
    // 使线程与主线程分离
    th.detach();
}

// 主循环处理事件，th线程的工作函数
void XThread::Main() {
    cout << id << " thread::Main() begin" << endl;
    
    // 开始事件循环,程序阻塞在这里
    event_base_dispatch(base);

    // 阻塞结束，销毁事件处理框架
    event_base_free(base);
    cout << id << " thread::Main() end" << endl;
}

// 初始化线程并设置通知机制。创建管道并配置事件基础结构，用于线程间的通信和事件处理。
bool XThread::Setup() {
    testout(id << " thread At Setup()");

    // 创建管道
    int fds[2];
    if (pipe(fds)) {
        cerr << "pipe failed" << endl;
        return false; 
    }
    
    // 设置通知发送文件描述符
    notify_send_fd = fds[1];

    // 创建事件框架配置，禁用锁
    event_config *ev_conf = event_config_new();
    event_config_set_flag(ev_conf, EVENT_BASE_FLAG_NOLOCK);

    // 创建事件基础结构
    base = event_base_new_with_config(ev_conf);
    event_config_free(ev_conf);
    if (!base) {
        cout << "event_base_new_with_config error!" << endl;
        return false; // 创建事件基础结构失败
    }

    // 创建并添加读事件，用于接收通知
    event *ev = event_new(base, fds[0], EV_READ | EV_PERSIST, Notify_cb, this);
    event_add(ev, 0);

    return true;
}

// 激活线程，发送激活信号"c",并开始工作处理任务
void XThread::Activate() {
    testout(id << " thread At Activate()");

    // 尝试发送通知，触发相关事件
    int ret = write(notify_send_fd, "c", 1);
    if (ret <= 0) {
        cerr << "XThread::Activate() fail" << endl;
    }

    // 锁定任务队列的互斥锁，确保线程安全
    std::lock_guard<std::mutex> lock(tasks_mutex);

    // 取出并初始化任务队列中的第一个任务
    if (tasks.empty()) return;
    XTask *t = tasks.front();
    tasks.pop_front();
    t->Init();
}

//添加任务到线程
void XThread::AddTack(XTask *t) {
    if (t == NULL) return;

    t->base = this->base;

    std::lock_guard<std::mutex> lock(tasks_mutex);
    tasks.push_back(t);
}

// XThread构造函数
XThread::XThread() {}

// XThread析构函数
XThread::~XThread() {}