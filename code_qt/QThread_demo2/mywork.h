#ifndef MYWORK_H
#define MYWORK_H

#include <QObject>
#include <QVector>

class Create : public QObject
{
    Q_OBJECT
public:
    explicit Create(QObject *parent = nullptr);

signals:
    void finished(QVector<int> list);

public slots:
    void working(int num);

private:
    QVector<int> list;
};


class Bubble : public QObject
{
    Q_OBJECT
public:
    explicit Bubble(QObject *parent = nullptr);

signals:
    void finished(QVector<int> list);

public slots:
    void working(QVector<int> list);
};


class Quick : public QObject
{
    Q_OBJECT
public:
    explicit Quick(QObject *parent = nullptr);
    void quickSort(QVector<int> &list, int left, int right);

signals:
    void finished(QVector<int> list);

public slots:
    void working(QVector<int> list);
};

#endif // MYWORK_H
