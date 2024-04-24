#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Client");
    ui->spinBox->setRange(8888,9999);
    ui->lineEdit->setText("192.168.195.128");
    tcpSocket = new QTcpSocket(this);
    //连接服务器
    connect(ui->pushButton_open, &QPushButton::clicked, tcpSocket, [=](){
        //连接服务器
        tcpSocket->connectToHost(ui->lineEdit->text(), ui->spinBox->value());
        //连接成功
        connect(tcpSocket, &QTcpSocket::connected, [=](){
            ui->textBrowser->append("连接服务器成功");
        });
        //连接断开
        connect(tcpSocket, &QTcpSocket::disconnected, [=](){
            ui->textBrowser->append("连接断开");
        });
   });

    // 断开连接
    connect(ui->pushButton_close, &QPushButton::clicked, tcpSocket, &QTcpSocket::disconnectFromHost);

    // 发送消息
    connect(ui->pushButton_send, &QPushButton::clicked, this, [=](){
        QString msg = ui->textEdit->toPlainText();
        QByteArray array = msg.toUtf8();
        tcpSocket->write(array);
        ui->textBrowser->append("客户端:" + msg);
        ui->textEdit->clear();
        ui->textEdit->setFocus();
    });

    // 接收消息
    connect(tcpSocket, &QTcpSocket::readyRead, [=](){
        QByteArray array = tcpSocket->readAll();
        ui->textBrowser->append("服务器:" + array);
    });
}


Widget::~Widget()
{
    delete ui;
}
