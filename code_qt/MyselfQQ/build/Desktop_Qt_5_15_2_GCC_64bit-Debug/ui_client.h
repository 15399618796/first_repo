/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QLabel *cStatusLbl;
    QProgressBar *progressBar;
    QPushButton *cCancleBtn;
    QPushButton *cCloseBtn;
    QLabel *label;

    void setupUi(QDialog *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QString::fromUtf8("Client"));
        Client->resize(400, 190);
        cStatusLbl = new QLabel(Client);
        cStatusLbl->setObjectName(QString::fromUtf8("cStatusLbl"));
        cStatusLbl->setGeometry(QRect(40, 70, 331, 71));
        QFont font;
        font.setPointSize(10);
        cStatusLbl->setFont(font);
        progressBar = new QProgressBar(Client);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(100, 40, 271, 23));
        progressBar->setValue(0);
        cCancleBtn = new QPushButton(Client);
        cCancleBtn->setObjectName(QString::fromUtf8("cCancleBtn"));
        cCancleBtn->setGeometry(QRect(80, 150, 75, 23));
        cCloseBtn = new QPushButton(Client);
        cCloseBtn->setObjectName(QString::fromUtf8("cCloseBtn"));
        cCloseBtn->setGeometry(QRect(240, 150, 75, 23));
        label = new QLabel(Client);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 61, 21));
        QFont font1;
        font1.setPointSize(12);
        label->setFont(font1);

        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QDialog *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "\346\216\245\346\224\266", nullptr));
        cStatusLbl->setText(QCoreApplication::translate("Client", "\347\255\211\345\276\205\346\216\245\346\224\266\346\226\207\344\273\266\342\200\246", nullptr));
        cCancleBtn->setText(QCoreApplication::translate("Client", "\345\217\226\346\266\210", nullptr));
        cCloseBtn->setText(QCoreApplication::translate("Client", "\345\205\263\351\227\255", nullptr));
        label->setText(QCoreApplication::translate("Client", "\345\267\262\345\256\214\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
