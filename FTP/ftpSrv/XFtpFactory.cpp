#include "XFtpFactory.h"
#include "XFtpServerCMD.h"
#include "XFtpUSER.h"
#include "XFtpLIST.h"
#include "XFtpPORT.h"
#include "XFtpRETR.h"
#include "XFtpSTOR.h"
#include "testUtil.h"

/**
 * @brief 创建一个FTP任务。
 * 
 * 该函数负责创建一个XFtpServerCMD对象，并注册各种FTP命令的处理程序，最后返回这个对象。
 * 
 * @return XTask* 指向创建的XFtpServerCMD对象的指针。
 */
XTask *XFtpFactory::CreateTask() {
    testout("At XFtpFactory::CreateTask()"); 
	
    XFtpServerCMD *x = new XFtpServerCMD(); // 创建一个XFtpServerCMD实例

    // 注册USER命令、PORT命令的处理程序
    x->Reg("USER", new XFtpUSER());
    x->Reg("PORT", new XFtpPORT());

    // 为PWD、LIST、CWD和CDUP命令注册相同的处理程序列表
    XFtpTask *list = new XFtpLIST();
    x->Reg("PWD", list);
    x->Reg("LIST", list);
    x->Reg("CWD", list);
    x->Reg("CDUP", list);

    x->Reg("RETR", new XFtpRETR());
    x->Reg("STOR", new XFtpSTOR());
    
    return x; // 返回创建的XFtpServerCMD对象
}

XFtpFactory::XFtpFactory() {

}