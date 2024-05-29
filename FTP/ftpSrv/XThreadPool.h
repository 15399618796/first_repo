#pragma once
#include <vector>

// 定义XThread类，任务线程基类
class XThread;
// 定义XTask类，任务基类
class XTask;

// 定义线程池类
class XThreadPool
{
public:
    // 获取线程池单例
    static XThreadPool *Get() {
        static XThreadPool p; // 静态局部变量，保证只初始化一次并返回同一实例
        return &p;
    }

    // 初始化线程池
    void Init(int threadCount); // 参数：threadCount，线程池中线程的数量

    // 分发任务到线程池
    void Dispatch(XTask*); // 参数：task，待执行的任务

private:
    int threadCount; // 线程池中线程的数量
    int lastThread = -1; // 记录上一次分配给任务的线程索引，优化任务分配
    std::vector<XThread *> threads; // 存储线程池中的线程们
    XThreadPool() {}; // 私有构造函数，防止外部实例化
};