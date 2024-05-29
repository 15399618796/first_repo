#include "XFtpUSER.h"
#include "testUtil.h"

// 解析XFtpUSER命令, 响应登录成功信息
void XFtpUSER::Parse(std::string, std::string)
{
	testout("AT XFtpUSER::Parse");

	ResCMD("230 Login successsful.\r\n");
}

