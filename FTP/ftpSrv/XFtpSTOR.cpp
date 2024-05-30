#include "XFtpSTOR.h"
#include "testUtil.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>
using namespace std;

// 该函数用于在FTP STOR命令执行时，从bufferevent中读取数据，然后将读取到的数据写入到指定的文件中
void XFtpSTOR::Read(bufferevent *bev) {
    testout("At XFtpSTOR::Read"); // 输出当前位置信息，用于调试。

    // 如果文件指针为空，则直接返回，不执行读取操作。
    if (!fp) return;

    // 不断读取数据直到读取失败或结束。
    while (1) {
        // 从bufferevent中读取数据到缓冲区。
        int len = bufferevent_read(bev, buf, sizeof(buf));
        // 如果读取的数据长度小于等于0，表示读取结束或出错，退出循环。
        if (len <= 0) {
            return;
        }
        // 将读取到的数据写入文件。
        fwrite(buf, 1, len, fp);
    }
}

// 处理与客户端连接相关的事件，如连接建立、读写事件、超时或错误
void XFtpSTOR::Event(bufferevent *bev, short events) {
    testout("At XFtpSTOR::Event"); // 输出当前函数调用信息，用于调试。

    // 检查是否触发了结束、错误或超时事件。
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        ClosePORT(); // 关闭PORT连接。
        ResCMD("226 Transfer complete"); // 发送传输完成的响应。
    }
    // 检查是否触发了连接建立事件。
    else if (events & BEV_EVENT_CONNECTED)
        cout << "XFtpSTOR BEV_EVENT_CONNECTED" << endl; // 输出连接建立事件的信息。
}

// 解析STOR命令，准备接收文件。
void XFtpSTOR::Parse(std::string type, std::string msg) {
    testout("At XFtpSTOR::Parse");
    
    // 从消息中提取文件名
    int pos = msg.rfind(" ") + 1;
    std::string filename = msg.substr(pos, msg.size() - pos - 2);
    // 拼接文件的完整路径
    std::string path = filename;
    testout("filepath:[" << path << "]");

    // 尝试打开文件以准备写入
    fp = fopen(path.c_str(), "wb");
    if (fp) {
        // 文件打开成功，设置连接并响应客户端
        ConnectoPORT();
        ResCMD("125 File OK");
        bufferevent_trigger(bev, EV_READ, 0);
    }
    else {
        // 文件打开失败，向客户端发送错误信息
        ResCMD("450 file open failed!");
    }

}

