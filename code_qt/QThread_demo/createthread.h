#ifndef CREATETHREAD_H
#define CREATETHREAD_H

#include <QThread>

class CreateThread : public QThread
{
    Q_OBJECT
public:
    explicit CreateThread(QObject *parent = nullptr);

    protected:
    void run() override;

    signals:
    void sendList(QVector<int> list); //发送生成的随机数

    public slots:
    //接受来自主窗口的数据，随机数的个数num
    void recvNum(int num){
        m_num = num;
    }

    private:
    int m_num;
};

#endif // CREATETHREAD_H
