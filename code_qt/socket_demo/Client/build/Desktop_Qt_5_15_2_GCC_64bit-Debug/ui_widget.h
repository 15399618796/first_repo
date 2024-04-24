/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *widget_3;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox;
    QPushButton *pushButton_open;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton_close;
    QLabel *label_3;
    QTextBrowser *textBrowser;
    QLabel *label_4;
    QTextEdit *textEdit;
    QPushButton *pushButton_send;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(594, 919);
        widget_3 = new QWidget(Widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(0, 0, 591, 921));
        verticalLayout = new QVBoxLayout(widget_3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(widget_3);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        spinBox = new QSpinBox(widget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout->addWidget(spinBox);

        pushButton_open = new QPushButton(widget);
        pushButton_open->setObjectName(QString::fromUtf8("pushButton_open"));

        horizontalLayout->addWidget(pushButton_open);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(widget_3);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit = new QLineEdit(widget_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        pushButton_close = new QPushButton(widget_2);
        pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));

        horizontalLayout_2->addWidget(pushButton_close);


        verticalLayout->addWidget(widget_2);

        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        textBrowser = new QTextBrowser(widget_3);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        label_4 = new QLabel(widget_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        textEdit = new QTextEdit(widget_3);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        pushButton_send = new QPushButton(widget_3);
        pushButton_send->setObjectName(QString::fromUtf8("pushButton_send"));

        verticalLayout->addWidget(pushButton_send);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\357\274\232", nullptr));
        pushButton_open->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200\357\274\232", nullptr));
        pushButton_close->setText(QCoreApplication::translate("Widget", "\346\226\255\345\274\200\350\277\236\346\216\245", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\351\200\232\344\277\241\350\256\260\345\275\225", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201\346\225\260\346\215\256", nullptr));
        pushButton_send->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
