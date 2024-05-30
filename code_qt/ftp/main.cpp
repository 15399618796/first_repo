#include <QCoreApplication>
#include <QFile>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    

    QNetworkAccessManager manager;
    QUrl ftpUrl; // 设置FTP连接参数，参考前面的代码示例

    ftpUrl.setScheme("ftp");             // 设置协议为FTP
    ftpUrl.setHost("127.0.0.1");   // 设置FTP服务器地址
    ftpUrl.setPort(21);                  // 设置FTP服务器端口，默认为21
    ftpUrl.setUserName("along");      // 设置用户名
    ftpUrl.setPassword("654321");      // 设置密码
    ftpUrl.setPath("/home/along/");     // 设置远程路径

    QFile file("/home/along/hello.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file";
        return -1;
    }

    QUrl fileUrl = ftpUrl;
    fileUrl.setPath(fileUrl.path() + "test/path.txt");
    QNetworkRequest request(fileUrl);

    QNetworkReply *reply = manager.put(request, &file);
    QObject::connect(reply, &QNetworkReply::uploadProgress, [](qint64 bytesSent, qint64 bytesTotal) {
        qDebug() << "Progress:" << bytesSent << "/" << bytesTotal;
    });

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "File uploaded successfully";
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        file.close();
        reply->deleteLater();
        a.quit();
    });

    return a.exec();
}
