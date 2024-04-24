#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("Qt 5 示例");

    // 设置窗口图标
    setWindowIcon(QIcon(":/images/icon.png"));
}

MainWindow::~MainWindow()w
{
    delete ui;
}
