#include "XFtpPORT.h"
#include "testUtil.h"

#include <iostream>
#include <vector>
using namespace std;


/**
 * @brief 解析PORT命令的响应消息。
 * 
 * 该函数用于解析客户端通过PORT命令发送的IP地址和端口号信息，
 * 并基于这些信息设置服务器的连接IP和端口。
 * 
 * @param type 命令类型，通常为"PORT"。
 * @param msg 包含IP地址和端口号的字符串，格式为"n1,n2,n3,n4,n5,n6"，
 *            其中n1-n4为IP地址的四段十进制表示，n5和n6组合成端口号。
 */
void XFtpPORT::Parse(string type, string msg) {
    testout("XFtpPORT::Parse");
    // 解析PORT命令中的IP地址和端口号

    vector<string>vals;
    string tmp = "";
    // 从消息中提取出IP地址和端口号的各个字段
    for (int i = 5; i < msg.size(); i++) {
        if (msg[i] == ',' || msg[i] == '\r') {
            vals.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += msg[i];
    }

    // 如果提取出的字段数量不等于6，则回复语法错误信息并返回
    if (vals.size() != 6) {
        ResCMD("501 Syntax error in parameters or arguments.");
        return;
    }

    // 根据提取出的字段组装IP地址，并计算端口号
    ip = vals[0] + "." + vals[1] + "." + vals[2] + "." + vals[3];
    port = atoi(vals[4].c_str()) * 256 + atoi(vals[5].c_str());
    cmdTask->ip = ip;
    cmdTask->port = port;
    testout("ip: " << ip);
    testout("port: " << port);
    // 发送PORT命令成功的响应
    ResCMD("200 PORT command success.");
}
