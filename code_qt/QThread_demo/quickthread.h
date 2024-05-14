#ifndef QUICKTHREAD_H
#define QUICKTHREAD_H


#include <QThread>
#include <QVector>

class QuickThread : public QThread
{
    Q_OBJECT
public:
    explicit QuickThread(QObject *parent = nullptr);
    void quick_sort(QVector<int> &list, int l, int r);

protected:
    virtual void run() override;

signals:
    void finished(QVector<int> list);

public slots:
    void recvList(QVector<int> list){
        //接收传入的list
        this->list = list;
        //启动线程
        QThread::start();
    }

private:
    QVector<int> list;
};

#endif // QUICKTHREAD_H
