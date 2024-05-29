#include "XFtpTask.h"
#include "testUtil.h"

#include <event2/bufferevent.h>
#include <event2/event.h>

#include <string.h>
#include <iostream>
using namespace std;

// 建立连接并设置端口
void XFtpTask::ConnectoPORT() {
	testout("At XFtpTask::ConnectoPORT");

	if (cmdTask->ip.empty() || cmdTask->port <= 0 || !cmdTask->base) {
		cout << "ConnectPORT failed" << endl;
		return;
	}
	// 如果已经存在一个活动的连接，则先释放它
	if (bev) {
		bufferevent_free(bev);
		bev = 0;
	}
	bev = bufferevent_socket_new(cmdTask->base, -1, BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(cmdTask->port);
    evutil_inet_pton(AF_INET, cmdTask->ip.c_str(), &sin.sin_addr.s_addr);

	Setcb(bev);

	timeval t = {60, 0};
	bufferevent_set_timeouts(bev, &t, 0);

	bufferevent_socket_connect(bev, (sockaddr*)&sin, sizeof(sin));
}

void XFtpTask::ClosePORT() {
	if (bev) {
		bufferevent_free(bev);
		bev = 0;
	}
	if (fp) {
		fclose(fp);
		fp = 0;
	}
}

// 设置回调函数，开启读写缓冲区
void XFtpTask::Setcb(bufferevent *bev) {
	bufferevent_setcb(bev, ReadCB, WriteCB, EventCB, this);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void XFtpTask::Send(const string &data) {
	testout("At XFtpTask::Send");

	Send(data.c_str(), data.size());
}
// 使用bufferevent将数据发送给连接的对端
void XFtpTask::Send(const char *data, size_t datasize) {
	testout("At XFtpTask::Send");

	cout << data;
	// cout << datasize << endl;
	if (datasize == 0) return;
	if (bev) {
		bufferevent_write(bev, data, datasize);
	}
}
// 回复命令的处理函数
void XFtpTask::ResCMD(string msg) {
	testout("At XFtpTask::ResCMD");

	if (!cmdTask || !cmdTask->bev) return;
	cout << "ResCMD: " << msg << endl << flush;
	if (msg[msg.size() - 1] != '\n') {
		msg += "\r\n";
	}
	bufferevent_write(cmdTask->bev, msg.c_str(), msg.size());
}

void XFtpTask::EventCB(bufferevent *bev, short events, void *arg) {
	XFtpTask *t = (XFtpTask*)arg;
	t->Event(bev, events);
}
void XFtpTask::ReadCB(bufferevent *bev, void *arg) {
	XFtpTask *t = (XFtpTask*)arg;
	t->Read(bev);
}
void XFtpTask::WriteCB(bufferevent *bev, void *arg) {
	XFtpTask *t = (XFtpTask*)arg;
	t->Write(bev);
}

XFtpTask::~XFtpTask() {
	ClosePORT();
}
