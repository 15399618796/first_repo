#pragma once

// 这是一个抽象基类，设计用于定义一个任务的基本属性和行为
class XTask
{
public:
    // 指向一个事件基础结构的指针，用于事件驱动编程
    struct event_base *base = 0;

    // 用于网络通信的套接字
    int sock = 0;

    // 线程的唯一标识符
    int thread_id = 0;

    /**
     * 初始化函数。
     * 这是一个纯虚函数，需要在派生类中实现，用于执行任务的初始化工作。
     * @return 返回初始化是否成功。成功返回true，失败返回false。
     */
    virtual bool Init() = 0;
};
