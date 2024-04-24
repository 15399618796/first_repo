#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");
    ui->spinBox->setRange(8888, 9999);

    tcpServer = new QTcpServer();

    //检测是否有新的连接
    connect(tcpServer, &QTcpServer::newConnection, this, [=](){
        tcpSocket = tcpServer->nextPendingConnection();
        ui->textBrowser->append("成功建立连接");
        //进行通信
        connect(tcpSocket, &QTcpSocket::readyRead, this, [=](){
            QByteArray msg = tcpSocket->readAll();
            ui->textBrowser->append("客户端：" + msg);
        });
        //断开连接
        connect(tcpSocket, &QTcpSocket::disconnected, this, [=](){
            ui->textBrowser->append("连接已断开");
        });
    });

}

Widget::~Widget()
{
    delete ui;
}

//启动
void Widget::on_pushButton_clicked()
{
    tcpServer->listen(QHostAddress("192.168.195.128"), 8888);
    ui->textBrowser->append("服务器启动");
}

//发送数据
void Widget::on_pushButton_2_clicked()
{
    tcpSocket->write(ui->lineEdit->text().toUtf8().data());
    ui->textBrowser->append("服务器：" + ui->lineEdit->text());
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}

