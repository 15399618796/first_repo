#include "XFtpRETR.h"
#include "testUtil.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>
using namespace std;

/**
 * @brief 解析RETR命令响应消息，并尝试打开文件进行传输。
 * 
 * @param type 消息类型（未使用）
 * @param msg FTP服务器返回的RETR命令响应消息，包含要下载的文件名。
 */
void XFtpRETR::Parse(string type, string msg) {
    testout("At XFtpRETR::Parse"); // 输出调试信息，表示进入Parse函数

    // 从响应消息中提取文件名
    int pos = msg.rfind(" ") + 1;
    string filename = msg.substr(pos, msg.size() - pos - 2);
    
    // 拼接文件的完整路径
    string path = cmdTask->rootDir + cmdTask->curDir + filename;
    testout("filepath:[" << path << "]"); // 输出文件路径的调试信息

    // 尝试以二进制只读模式打开文件
    fp = fopen(path.c_str(), "rb");
    if (fp) {
        // 文件打开成功，建立数据连接端口
        ConnectoPORT();
        // 发送文件准备好的响应命令
        ResCMD("150 File OK");
        // 手动触发写事件，开始文件传输
        bufferevent_trigger(bev, EV_WRITE, 0);
    }
    else {
        // 文件打开失败，发送错误响应命令
        ResCMD("450 file open failed!");
    }
}

// 该函数用于在FTP RETR命令响应中写入数据。
void XFtpRETR::Write(bufferevent *bev) {
    testout("At XFtpRETR::Write");

    // 如果文件指针为空，则直接返回，不执行后续操作。
    if (!fp) return;

    // 从文件中读取数据到缓冲区。
    int len = fread(buf, 1, sizeof(buf), fp);
    // 如果读取的数据长度小于等于0，表示文件读取完成，发送完成信息并关闭PORT。
    if (len <= 0) {
        ResCMD("226 Transfer complete"); // 发送传输完成的响应命令。
        ClosePORT(); // 关闭PORT连接。
        return;
    }

    // 打印读取到的数据，通常用于调试。
    cout << buf;
    // 发送缓冲区中的数据到bufferevent的读缓冲区中，通过触发写事件来触发数据发送。
    Send(buf, sizeof(buf));
}

void XFtpRETR::Event(bufferevent *bev, short events) {
	testout("At XFtpRETR::Event");
	
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
		ClosePORT();
	}
	else if (events & BEV_EVENT_CONNECTED) {
		cout << "TXFtpRETR BEV_EVENT_CONNECTED" << endl;
	}
}