#include <QPluginLoader>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include <QCoreApplication>

void loadMySqlDriver();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    /*
     *链接mysql数据库
     */
    QSqlDatabase db( QSqlDatabase::addDatabase( "QMYSQL" ) );
    db.setHostName("localhost");
    db.setDatabaseName("demo");
    db.setUserName("root");
    db.setPassword("123456");
    db.setPort(3360);
    if(!db.open())/*测试数据库是否链接成功*/
    {
        qDebug()<<db.lastError();
    }
    else
    {
        qDebug()<<"Ok";
    }
    return a.exec();
}
