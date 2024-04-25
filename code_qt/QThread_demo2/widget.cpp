#include "widget.h"
#include "ui_widget.h"
#include "mywork.h"

#include <QThread>
#include <QVector>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QThread* t1 = new QThread;
    QThread* t2 = new QThread;
    QThread* t3 = new QThread;

    Create* create = new Create;
    Bubble* bubble = new Bubble;
    Quick* quick = new Quick;

    create->moveToThread(t1);
    bubble->moveToThread(t2);
    quick->moveToThread(t3);

    //启动随机数子线程
    connect(ui->pushButton, &QPushButton::clicked, this, [=](){
        t1->start();
        emit starting(100);
    });
    //子线程t1收到信号后，开始生成随机数
    connect(this, &Widget::starting, create, &Create::working);
    //接受t1的信号，并显示在listWidget_rand中
    connect(create, &Create::finished, this, [=](QVector<int> list){
        t2->start();
        t3->start();
        for(auto i : list)
            ui->listWidget_rand->addItem(QString::number(i)); 
    });

    //子线程t2开始工作
    connect(create, &Create::finished, bubble, &Bubble::working);
    connect(bubble, &Bubble::finished, this, [=](QVector<int> list){
        for(auto i : list)
            ui->listWidget_bubble->addItem(QString::number(i));
    });

    //子线程t3开始工作
    connect(create, &Create::finished, quick, &Quick::working);
    connect(quick, &Quick::finished, this, [=](QVector<int> list){
        for(auto i : list)
            ui->listWidget_quick->addItem(QString::number(i));
    });
}

Widget::~Widget()
{
    delete ui;
}
