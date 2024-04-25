#include "quickthread.h"


QuickThread::QuickThread(QObject *parent)
    : QThread{parent}
{
}

void QuickThread::quick_sort(QVector<int> &list, int l, int r)
{
    if (l < r)
    {
        int i = l, j = r, x = list[l];
        while (i < j)
        {
            while(i < j && list[j] >= x) // 从右向左找第一个小于x的数
                j--;
            if(i < j)
                list[i++] = list[j];
            while(i < j && list[i] < x) // 从左向右找第一个大于等于x的数
                i++;
            if(i < j)
                list[j--] = list[i];
        }
        list[i] = x;
        quick_sort(list, l, i - 1); // 递归排序左子数组
        quick_sort(list, i + 1, r); // 递归排序右子数组
    }
}

void QuickThread::run()
{
    //快速排序
    quick_sort(list, 0, list.size() - 1);
    emit finished(list); // 发送信号
}
