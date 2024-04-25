#include "bubblethread.h"

BubbleThread::BubbleThread(QObject *parent)
    : QThread{parent}
{}

void BubbleThread::run()
{
    // 执行冒泡排序算法
    int size = m_list.size();
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (m_list[j] > m_list[j + 1]) {
                int temp = m_list[j];
                m_list[j] = m_list[j + 1];
                m_list[j + 1] = temp;
            }
        }
    }
    emit finished(m_list); // 发送完成信号
}