#include "widget.h"
#include "ui_widget.h"
#include "createthread.h"
#include "bubblethread.h"
#include "quickthread.h"

#include <QVector>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("多线程");

    CreateThread* createThread = new CreateThread;
    BubbleThread* bubbleThread = new BubbleThread;
    QuickThread* quickThread = new QuickThread; 

    //启动子线程CreateThread
    connect(ui->pushButton, &QPushButton::clicked, createThread, [=](){
        emit sendNum(100); // 发送信号，指定随机数的数量
        createThread->start(); // 启动createThread线程
    });
    connect(this, &Widget::sendNum, createThread, &CreateThread::recvNum);

    //接受来自子线程CreateThread的信号
    connect(createThread, &CreateThread::sendList, this, [=](QVector<int> list){
        for(int i : list){
            ui->listWidget_rand->addItem(QString::number(i)); 
        }
        //bubbleThread->start(); // 启动bubbleThread线程
        //quickThread->start(); // 启动quickThread线程
        emit sendList(list); // 发送信号，指定随机数列表
    });

    //发送信号，指定随机数列表
    connect(this, &Widget::sendList, bubbleThread, &BubbleThread::recvList);
    //接受来自子线程BubbleThread的信号
    connect(bubbleThread, &BubbleThread::finished, this, [=](QVector<int> list){
        for(int i : list){
            ui->listWidget_bubble->addItem(QString::number(i)); 
        }
    });


    //发送信号，指定随机数的列表
    connect(this, &Widget::sendList, quickThread, &QuickThread::recvList);
    //接受来自子线程QuickThread的信号
    connect(quickThread, &QuickThread::finished, this, [=](QVector<int> list){
        for(int i : list){
            ui->listWidget_quick->addItem(QString::number(i)); 
        }
    });
}

Widget::~Widget()
{
    delete ui;
}
