#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QString>
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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void sendPortCommand();

    void sendListCommand(QString cmd);

    QByteArray wait();

    void urlInit();

private:
    Ui::MainWindow *ui;
    QTcpSocket *ftpSocket;
    QTcpServer *dataServer;
    QTcpSocket *dataSocket = nullptr;
    QString curDir = "/";

    QFileSystemModel *fileModel;
    QStringListModel *listModel; // 用于显示文件列表的模型。
    
    QNetworkAccessManager manager;
    QUrl ftpUrl; // 设置FTP连接参数，参考前面的代码示例
    QUrl fileUrl; // 用于上传和下载文件时使用。

    QNetworkRequest request; // 用于上传和下载文件时使用。
    QNetworkReply *reply; 

    QString localFilePath; // 本地文件路径，用于上传和下载文件时使用。
    QString remoteFilePath; // 远程文件路径，用于上传和下载文件时使用。
    QString localFileName; // 传输文件名，用于上传和下载文件时使用。
    QString remoteFileName; // 远程文件名，用于上传和下载文件时使用。
};
#endif // MAINWINDOW_H
