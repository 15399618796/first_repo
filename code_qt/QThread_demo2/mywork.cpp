#include "mywork.h"

Create::Create(QObject * parent)
{
}

//生成随机数
void Create::working(int num)
{
    for(int i = 0; i < num; i ++){
        list.append(qrand() % 1000);
    }
    emit finished(list);
}

Bubble::Bubble(QObject *parent)
{
}

//冒泡排序
void Bubble::working(QVector<int> list)
{
    for(int i = 0; i < list.size(); i ++){
        for(int j = 0; j < list.size() - i - 1; j ++){
            if(list[j] > list[j + 1]){
                int temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
    emit finished(list);
}

Quick::Quick(QObject *parent)
{
}

//快速排序
void Quick::working(QVector<int> list)
{
    quickSort(list, 0, list.size() - 1);
    emit finished(list);
}

void Quick::quickSort(QVector<int> &list, int left, int right)
{
    if(left >= right)
        return;
    int i = left, j = right, x = list[left];
    while(i < j){
        while(i < j && list[j] >= x)
            j --;
        if(i < j)
            list[i ++] = list[j];
        while(i < j && list[i] < x)
            i ++;
        if(i < j)
            list[j --] = list[i];
    }
    list[i] = x;
    quickSort(list, left, i - 1);
    quickSort(list, i + 1, right);
}