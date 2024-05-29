#include "XFtpLIST.h"
#include "testUtil.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <string>
#include <sys/stat.h>
using namespace std;

// 主要完成FTP LIST命令的响应，回复客户端
void XFtpLIST::Write(bufferevent *bev) {
	testout("AT XFtpLIST::Write");

	ResCMD("226 Transfer comlete");
	ClosePORT();
}

void XFtpLIST::Event(bufferevent *bev, short events) {
	testout("At XFtpLIST::Event");

	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
		ClosePORT();
	}
	else if (events & BEV_EVENT_CONNECTED)
		cout << "XFtpLIST BEV_EVENT_CONNECTED" << endl;
}

/**
 * @brief 解析FTP命令并做出相应处理
 * 
 * 该函数根据传入的类型（type）和消息（msg）解析FTP命令，执行相应的操作，
 * 包括切换当前目录、获取目录列表、改变工作目录等。
 * 
 * @param type 指令类型，用于区分不同的FTP命令，如"PWD"、"LIST"、"CWD"、"CDUP"
 * @param msg FTP命令的具体内容，格式根据不同的命令类型而变化
 */
void XFtpLIST::Parse(std::string type, std::string msg) {
    testout("At XFtpLIST::Parse");

    string resmsg = "";
    if (type == "PWD") {
        // 处理PWD命令，返回当前目录
        //resmsg = "257 \"";
        //resmsg += cmdTask->curDir;
        //resmsg += " is current dir.";
        resmsg = cmdTask->curDir + " is current dir\n";
        ResCMD(resmsg);
    }
    else if (type == "LIST") {
        // 处理LIST命令，返回目录列表
        string path = cmdTask->rootDir + cmdTask->curDir;
        testout("listpath: " << path);
        string listdata = GetListData(path); // 获取目录列表数据
        ConnectoPORT(); // 建立数据连接
        ResCMD("150 Here coms the directory listing."); // 发送即将发送目录列表的响应
        Send(listdata); // 发送目录列表数据
    }
    else if (type == "CWD") {
        // 处理CWD命令，改变当前工作目录
        
        int pos = msg.rfind(" ") + 1; // 查找最后一个空格的位置，以确定路径的开始
        string path = msg.substr(pos, msg.size() - pos - 2); // 从空格后提取路径，直到字符串倒数第二个字符
        string curDir = cmdTask->curDir; // 当前工作目录
        
        // 判断路径是否为绝对路径，是则直接用作新目录，否则相对于当前目录
        if (path[0] == '/') 
        {
            curDir = path; // 使用绝对路径作为新目录
        }
        else
        {
            // 如果当前目录不是以斜杠结尾，则在其后添加斜杠，然后加上新路径
            if (curDir[curDir.size() - 1] != '/')
                curDir += "/";
            curDir += path + "/";
        }
        
        // 确保目录路径以斜杠结尾
        if (curDir[curDir.size() - 1] != '/')
            curDir += "/";
        
        struct stat s_buf; // 用于存储文件状态的结构体
        stat(curDir.c_str(),&s_buf); // 获取目录信息
        
        // 检查路径是否为目录，是则更新当前目录，否则发送错误消息
        if(S_ISDIR(s_buf.st_mode)) // 检查文件类型是否为目录
        {
            cmdTask->curDir = curDir; // 更新当前目录
            ResCMD("250 Directory succes chanaged.\r\n"); // 发送成功更改目录的响应
            ResCMD("curDir:" + cmdTask->curDir +"\r\n");
        }
        else
        {
            ResCMD("501 Directory not chanaged.\r\n"); // 发送无法更改目录的错误响应
        }
    }
    else if (type == "CDUP") {
        // 处理CDUP命令，回到上级目录
        cout << "msg:" << msg << endl;
        cout << "cmdTask->curDir:" << cmdTask->curDir << endl;
        string path = cmdTask->curDir;
        if (path[path.size() - 1] == '/')
        {
            path = path.substr(0, path.size() - 1);
        }
        int pos = path.rfind("/");
        path = path.substr(0, pos);
        cmdTask->curDir = path;
        if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
            cmdTask->curDir += "/";
        cout << "cmdTask->curDir:" << cmdTask->curDir << endl;
        ResCMD("250 Directory succes chanaged.\r\n");
    }
}



// 获取ls命令输出的文件列表
string XFtpLIST::GetListData(string path) {
	// -rwxrwxrwx 1 root root 418 Mar 21 16:10 XFtpFactory.cpp

	string data = "";
	string cmd = "ls -h ";
	cmd += path;
	cout << cmd << endl;

	FILE *f = popen(cmd.c_str(), "r");
	if (!f) return data;
	char buf[1024] = {0};
	while (1) {
		int len = fread(buf, 1, sizeof(buf) - 1, f);
		if (len <= 0)break;
		buf[len] = '\0';
		data += buf;
	}
	pclose(f);
 
	return data;
}
