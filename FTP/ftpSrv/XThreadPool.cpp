#include "XThreadPool.h"
#include "XThread.h"
#include "XTask.h"
#include <thread>
#include <iostream>
using namespace std;

#include "testUtil.h"

void XThreadPool::Dispatch(XTask *task) { // 分发任务到线程池中的其中一个线程上,并激活该线程
	testout("main thread At XThreadPoll::dispathch()");
	
	if (!task) return;
	int tid = (lastThread + 1) % threadCount;
	lastThread = tid;
	XThread *t = threads[tid];
	
	t->AddTack(task);
	t->Activate();
}

void XThreadPool::Init(int threadCount) { // 初始化线程池
	testout("main thread At XThreadPoll::Init()");

	this->threadCount = threadCount;
	this->lastThread = -1;
	for (int i = 0; i < threadCount; i++) {
		cout << "Create thread" << i << endl;
		XThread *t = new XThread();
		t->id = i;
		t->Start();
		threads.push_back(t);
		// 使当前线程暂停一段时间10ms,相当于sleep()
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}
