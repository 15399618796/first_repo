#include "createthread.h"

#include <QDebug>
#include <QVector>

CreateThread::CreateThread(QObject *parent)
    : QThread{parent}
{}

void CreateThread::run()
{
    // 线程执行的代码
    qDebug() << "CreateThread thread is" << QThread::currentThread();
    QVector<int> list;
    for(int i = 0; i < m_num; i ++){
        list.append(rand() % 1000);
    }
    
    //写个冒泡排序
   
    emit sendList(list); 
}