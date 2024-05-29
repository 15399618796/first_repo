/********************************************************************************
** Form generated from reading UI file 'ftpconnectdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FTPCONNECTDLG_H
#define UI_FTPCONNECTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FtpConnectDlg
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *edit_host;
    QLabel *label_2;
    QLineEdit *edit_user;
    QLabel *label_3;
    QLineEdit *edit_pass;
    QPushButton *btn_connect;

    void setupUi(QDialog *FtpConnectDlg)
    {
        if (FtpConnectDlg->objectName().isEmpty())
            FtpConnectDlg->setObjectName(QString::fromUtf8("FtpConnectDlg"));
        FtpConnectDlg->resize(248, 282);
        gridLayout = new QGridLayout(FtpConnectDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(FtpConnectDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        edit_host = new QLineEdit(FtpConnectDlg);
        edit_host->setObjectName(QString::fromUtf8("edit_host"));
        edit_host->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(edit_host, 0, 1, 1, 1);

        label_2 = new QLabel(FtpConnectDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        edit_user = new QLineEdit(FtpConnectDlg);
        edit_user->setObjectName(QString::fromUtf8("edit_user"));
        edit_user->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(edit_user, 1, 1, 1, 1);

        label_3 = new QLabel(FtpConnectDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        edit_pass = new QLineEdit(FtpConnectDlg);
        edit_pass->setObjectName(QString::fromUtf8("edit_pass"));
        edit_pass->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(edit_pass, 2, 1, 1, 1);

        btn_connect = new QPushButton(FtpConnectDlg);
        btn_connect->setObjectName(QString::fromUtf8("btn_connect"));
        btn_connect->setMinimumSize(QSize(225, 40));

        gridLayout->addWidget(btn_connect, 3, 0, 1, 2);


        retranslateUi(FtpConnectDlg);

        QMetaObject::connectSlotsByName(FtpConnectDlg);
    } // setupUi

    void retranslateUi(QDialog *FtpConnectDlg)
    {
        FtpConnectDlg->setWindowTitle(QCoreApplication::translate("FtpConnectDlg", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("FtpConnectDlg", "Host", nullptr));
        label_2->setText(QCoreApplication::translate("FtpConnectDlg", "User", nullptr));
        label_3->setText(QCoreApplication::translate("FtpConnectDlg", "Pass", nullptr));
        btn_connect->setText(QCoreApplication::translate("FtpConnectDlg", "\350\277\236\346\216\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FtpConnectDlg: public Ui_FtpConnectDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FTPCONNECTDLG_H
