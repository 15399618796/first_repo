#ifndef BUBBLETHREAD_H
#define BUBBLETHREAD_H

#include <QThread>
#include <QVector>

class BubbleThread : public QThread
{
    Q_OBJECT
public:
    explicit BubbleThread(QObject *parent = nullptr);

protected:
    //重写run函数
    void run() override;

signals:
    //发送排序后的列表
    void finished(QVector<int> list);

public slots:
    //收到来自主线程的随机数列表
    void recvList(QVector<int> list){
        m_list = list;
        //启动线程
        QThread::start();
    }

private:
    QVector<int> m_list;
};

#endif // BUBBLETHREAD_H
