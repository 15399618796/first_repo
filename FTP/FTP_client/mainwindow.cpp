#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QFile>
#include <QDataStream>
#include <QHostAddress>
#include <QFileSystemModel>
#include <QTreeView>
#include <QStandardPaths>
#include <QThread>
#include <QStringListModel>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ftpSocket = nullptr; // 初始化为空指针
    ui->serverAddressLineEdit->setText("127.0.0.1");
    ui->usernameLineEdit->setText("along");
    ui->passwordLineEdit->setText("654321");


    QFileSystemModel *model = new QFileSystemModel(this);
    // connect(model, &QFileSystemModel::directoryLoaded, this, &MainWindow::onDirectoryLoaded);
    
    //设置家目录为用户主目录
    model->setRootPath(QDir::homePath());

    // 设置文件过滤器，只显示目录和所有者可读的文件
    // model->setFilter(QDir::AllDirs | QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
    // 设置模型到视图中
    ui->treeView->setModel(model);
    
    // 设置视图样式，并调整列宽以适应内容
    // 禁用动画效果，以确保视图在操作时不会出现不必要的闪烁或过渡效果。
    ui->treeView->setAnimated(false);
    // 设置树视图的缩进大小为10个单位，以便更好地展示树结构的层次感。
    ui->treeView->setIndentation(10);
    // 启用视图的排序功能，使得用户可以通过点击列头来对视图中的项进行排序。
    // ui->treeView->setSortingEnabled(true);
    // 根据第一列中内容的宽度调整列的大小，确保所有内容都能完全显示，提高可读性。
    ui->treeView->resizeColumnToContents(50);
    // 设置第一列的宽度为50个像素，以便更好地展示文件名。
    ui->treeView->setColumnWidth(0, 300);

    // 当listView检测到鼠标双击
    connect(ui->listView, &QListView::doubleClicked, this, [=](){
        // 获取listView中选中行的文本
        QStringListModel *listModel = (QStringListModel *)ui->listView->model();
        QModelIndex index = ui->listView->currentIndex(); // 获取当前选中的索引。

        QString text = listModel->data(index, Qt::DisplayRole).toString(); // 获取第一行第一列的文本。

        text = "CWD " + text + "\r\n";
        ftpSocket->write(text.toLatin1().data()); // 发送CWD命令。
        wait(); // 等待服务器响应。

        sendListCommand("LIST"); // 发送LIST命令。
        qDebug()<<"listView : "<<text; // 调试输出文本。
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray MainWindow::wait(){
    // 等待服务器响应,不响应则退出
    if (!ftpSocket->waitForReadyRead(100)) {
        ui->statusLabel->setText("Failed to receive response from server.");
        exit(1); // 退出程序。
    }
     // 读取服务器响应,不响应则退出
    QByteArray response = ftpSocket->readAll();
    qDebug()<<"connect:"<<response; // 打印服务器响应。
    ui->statusLabel->setText(response); // 显示服务器响应。
    return response; // 返回服务器响应。
}

// 发送 PORT 命令
void MainWindow::sendPortCommand()
{
    // 获取客户端的 IP 地址和端口号
    QHostAddress clientAddress = ftpSocket->localAddress();
    quint16 clientPort = 6666; // 假设数据连接的端口号为 6666
    qDebug()<<clientAddress<<" "<<clientPort; // 打印客户端的 IP 地址和端口号

    // 将 IP 地址和端口号转换为字符串形式
    QString ipAddress = clientAddress.toString();
    QString portCommand = "PORT " + ipAddress.replace(".", ",") + "," + QString::number(clientPort / 256) + "," + QString::number(clientPort % 256) + "\r\n";

    // 发送 PORT 命令到服务器
    ftpSocket->write(portCommand.toUtf8());
    wait();
}

// 发送 LIST 命令
void MainWindow::sendListCommand(QString cmd)
{
    if(cmd == "PWD"){ // 如果命令是 PWD，则发送 PWD 命令到服务器，获取当前工作目录。
        ftpSocket->write("PWD\r\n"); // 发送 PWD 命令到服务器。
        QString response = wait();
        curDir = (QString)response.split(" ")[0]; // 获取当前工作目录。
        qDebug()<<curDir;

        return ;
    }
    if(cmd == "CDUP"){
        ftpSocket->write("CDUP\r\n"); // 发送 CDUP 命令到服务器。
        wait(); // 等待服务器响应。

        return ;
    }

    /* cmd == LIST 要进行数据传输*/
    // 设置数据连接端口监听
    dataServer = new QTcpServer(this);
    dataServer->listen(QHostAddress::Any, 6666);

    // 有新的连接
    connect(dataServer, &QTcpServer::newConnection, this, [=]{
        ui->statusbar->showMessage("Connected to data server."); //设置状态
        dataSocket = dataServer->nextPendingConnection(); // 获取连接的 socket 对象

        // 检测是否有数据
        connect(dataSocket, &QTcpSocket::readyRead, this, [=]{
            // 读取数据
            QString data = dataSocket->readAll(); // 读取服务器发送的数据
            /*将收到的数据添加到listView中*/
            QStringListModel *model = new QStringListModel(this); // 创建一个 QStringListModel 对象。
            data.replace(QRegExp("\n"), "\r\n"); // 使用正则表达式匹配并替换
            QStringList list = data.split("\r\n");
            for(auto& line : list){
                if(line.isEmpty()) continue; // 跳过空行。
                QStringList parts = line.split(" "); // 按空格分割。
                QString fileName = parts[parts.length() - 1]; // 获取文件名。
                line = fileName; // 只显示文件名。
            }
            list.insert(0, curDir); // 添加当前目录到列表的开头。
            
            model->setStringList(list); // 设置数据。    
        
            ui->listView->setModel(model); // 设置模型到视图中。   

            // 关闭连接，释放资源。
            dataSocket->close(); // 关闭 socket 连接
            dataServer->close(); // 关闭服务器监听。
        });
       
        // 连接断开时
        connect(dataSocket, &QTcpSocket::disconnected, this, [=]{
            ui->statusbar->showMessage("Disconnected from data server."); //设置状态
        });
    });

    // 首先获取当前目录
    ftpSocket->write("PWD\r\n"); // 发送 PWD 命令到服务器。
    QString response = wait();
    curDir = (QString)response.split(" ")[0]; // 更新当前工作目录。
    qDebug()<<curDir; // 调试输出当前工作目录。

    sendPortCommand(); // 发送 PORT 命令到服务器。
    ftpSocket->write("LIST\r\n");
    wait();
}

// 连接FTP服务器, USER, PASS, PORT 命令
void MainWindow::on_pushButton_clicked()
{
    // 获取用户输入的服务器地址、用户名和密码
    QString serverAddress = ui->serverAddressLineEdit->text();
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // 创建一个 QTcpSocket 对象
    ftpSocket = new QTcpSocket(this);

    // 设置连接状态栏
    connect(ftpSocket, &QTcpSocket::connected, this, [=](){
        ui->statusbar->showMessage("Connected to FTP server.");
    });
    connect(ftpSocket, &QTcpSocket::disconnected, this, [=](){
        ui->statusbar->showMessage("Disconnected from FTP server.");
    });

    // 连接到 FTP 服务器的默认端口（21）
    ftpSocket->connectToHost(serverAddress, 21);

    // 等待连接完成
    if (!ftpSocket->waitForConnected()) {
        ui->statusLabel->setText("Failed to connect to FTP server.");
        return;
    }
    ui->statusLabel->setText("Success to connect to FTP server.");

    // 连接成功后发送登录信息
    QString userCommand = "USER " + username + "\r\n";
    ftpSocket->write(userCommand.toUtf8());
    wait(); // 等待服务器响应。

    // 发送密码信息
    QString passCommand = "PASS " + password + "\r\n";
    ftpSocket->write(passCommand.toUtf8());
    wait(); // 等待服务器响应

    // 发送 LIST 命令,显示当前目录下的文件列表。
    sendListCommand("LIST");
}

// 上传文件, STOR 命令
void MainWindow::on_pushButton_2_clicked()
{
    // 登录服务器
    // sendListCommand("LIST");
    ftpSocket->write("CWD home\r\n");
    wait();
    sendListCommand("PWD"); // 发送LIST命令。
    sendListCommand("LIST");

    // QString localFilePath = "/home/along/hello.txt";
    // QString remoteFilePath = "/home/along/test/hello.txt";
    // // 打开本地文件
    // QFile file(localFilePath);
    // if (!file.open(QIODevice::ReadOnly)) {
    //     ui->statusLabel->setText("Failed to open local file.");
    //     return;
    // };

    // // 创建数据连接
    // QDataStream in(&file);

    // // 发送上传文件命令
    // QString uploadCommand = "STOR " + localFilePath + "\r\n";
    // ftpSocket->write(uploadCommand.toUtf8());

    // // 等待服务器响应
    // // if (!ftpSocket->waitForReadyRead()) {
    // //     ui->statusLabel->setText("Failed to receive response from server.");
    // //     return;
    // // }else qDebug()<<"ftp response success";

    // // 读取服务器响应
    // QByteArray response = ftpSocket->readAll();
    // qDebug()<<response;
    // // ui->statusLabel->setText(response);


    // // 检查服务器响应是否为文件准备就绪（150）
    // if (response.startsWith("150"))
    // {
    //     ui->statusLabel->setText("Uploading file...");
    //     // 上传文件内容
    //     while (!in.atEnd())
    //     {
    //         QByteArray buffer = file.read(1024); // 读取文件内容
    //         ftpSocket->write(buffer);            // 将文件内容写入 socket
    //     }
    //     // 关闭文件
    //     file.close();

    //     // 等待上传完成
    //     if (!ftpSocket->waitForBytesWritten())
    //     {
    //         ui->statusLabel->setText("Failed to upload file.");
    //         return;
    //     }

    //     // 读取上传完成的响应
    //     QString uploadCompleteResponse = ftpSocket->readAll();
    //     ui->statusLabel->setText(uploadCompleteResponse);
    // }
    // else
    // {
    //     ui->statusLabel->setText("Failed to upload file: " + response);
    // }
}

// 下载文件
void MainWindow::on_pushButton_3_clicked()
{
    QString localFilePath = "/home/along/hello.txt";
    QString remoteFilePath = "/home/along/";

    // 打开本地文件
    QFile file(localFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        ui->statusLabel->setText("Failed to open local file for writing.");
        return;
    }

    // 创建数据连接
    QDataStream out(&file);

    // 发送下载文件命令
    QString downloadCommand = "RETR " + remoteFilePath + "\r\n";
    ftpSocket->write(downloadCommand.toUtf8());

    // 等待服务器响应
    if (!ftpSocket->waitForReadyRead()) {
        ui->statusLabel->setText("Failed to receive response from server.");
        return;
    }

    // 读取服务器响应
    QString response = ftpSocket->readAll();

    // 检查服务器响应是否为文件准备就绪（150）
    if (response.startsWith("150")) {
        ui->statusLabel->setText("Downloading file...");

        // 接收文件内容并写入本地文件
        while (ftpSocket->bytesAvailable()) {
            QByteArray buffer = ftpSocket->readAll(); // 读取服务器发送的数据
            //out.write(buffer); // 将数据写入本地文件
            out.writeRawData(buffer, buffer.length());
        }

        // 关闭文件
        file.close();

        // 等待文件写入完成
        if (!file.flush()) {
            ui->statusLabel->setText("Failed to write file to disk.");
            return;
        }

        // 读取下载完成的响应
        QString downloadCompleteResponse = ftpSocket->readAll();
        ui->statusLabel->setText(downloadCompleteResponse);
    } else {
        ui->statusLabel->setText("Failed to download file: " + response);
    }
}

// 返回上一级目录
void MainWindow::on_pushButton_4_clicked()
{
    // 发送返回上一级目录命令（CDUP）并获取当前目录列表（LIST）
    sendListCommand("CDUP");
    sendListCommand("LIST");
}
