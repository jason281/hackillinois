/********************************************************************************
** Form generated from reading UI file 'mainwindowHp6496.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWHP6496_H
#define MAINWINDOWHP6496_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionReconnect;
    QAction *actionDisconnect;
    QWidget *centralwidget;
    QLabel *monitor;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *user_1;
    QLabel *user_2;
    QLabel *user_3;
    QLabel *user_4;
    QLabel *user_5;
    QVBoxLayout *verticalLayout_4;
    QLabel *user_6;
    QLabel *user_7;
    QLabel *user_8;
    QLabel *user_9;
    QLabel *user_10;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *connect_button;
    QPushButton *capture_button;
    QPushButton *stop_button;
    QMenuBar *menubar;
    QMenu *menuSetting;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        actionReconnect = new QAction(MainWindow);
        actionReconnect->setObjectName(QStringLiteral("actionReconnect"));
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        monitor = new QLabel(centralwidget);
        monitor->setObjectName(QStringLiteral("monitor"));
        monitor->setGeometry(QRect(230, 40, 551, 371));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(30, 30, 191, 471));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        user_1 = new QLabel(horizontalLayoutWidget);
        user_1->setObjectName(QStringLiteral("user_1"));

        verticalLayout_3->addWidget(user_1);

        user_2 = new QLabel(horizontalLayoutWidget);
        user_2->setObjectName(QStringLiteral("user_2"));

        verticalLayout_3->addWidget(user_2);

        user_3 = new QLabel(horizontalLayoutWidget);
        user_3->setObjectName(QStringLiteral("user_3"));

        verticalLayout_3->addWidget(user_3);

        user_4 = new QLabel(horizontalLayoutWidget);
        user_4->setObjectName(QStringLiteral("user_4"));

        verticalLayout_3->addWidget(user_4);

        user_5 = new QLabel(horizontalLayoutWidget);
        user_5->setObjectName(QStringLiteral("user_5"));

        verticalLayout_3->addWidget(user_5);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        user_6 = new QLabel(horizontalLayoutWidget);
        user_6->setObjectName(QStringLiteral("user_6"));

        verticalLayout_4->addWidget(user_6);

        user_7 = new QLabel(horizontalLayoutWidget);
        user_7->setObjectName(QStringLiteral("user_7"));

        verticalLayout_4->addWidget(user_7);

        user_8 = new QLabel(horizontalLayoutWidget);
        user_8->setObjectName(QStringLiteral("user_8"));

        verticalLayout_4->addWidget(user_8);

        user_9 = new QLabel(horizontalLayoutWidget);
        user_9->setObjectName(QStringLiteral("user_9"));

        verticalLayout_4->addWidget(user_9);

        user_10 = new QLabel(horizontalLayoutWidget);
        user_10->setObjectName(QStringLiteral("user_10"));

        verticalLayout_4->addWidget(user_10);


        horizontalLayout_2->addLayout(verticalLayout_4);

        horizontalLayoutWidget_2 = new QWidget(centralwidget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(390, 460, 281, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        connect_button = new QPushButton(horizontalLayoutWidget_2);
        connect_button->setObjectName(QStringLiteral("connect_button"));

        horizontalLayout_3->addWidget(connect_button);

        capture_button = new QPushButton(horizontalLayoutWidget_2);
        capture_button->setObjectName(QStringLiteral("capture_button"));

        horizontalLayout_3->addWidget(capture_button);

        stop_button = new QPushButton(horizontalLayoutWidget_2);
        stop_button->setObjectName(QStringLiteral("stop_button"));

        horizontalLayout_3->addWidget(stop_button);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuSetting = new QMenu(menubar);
        menuSetting->setObjectName(QStringLiteral("menuSetting"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSetting->menuAction());
        menuSetting->addAction(actionReconnect);
        menuSetting->addAction(actionDisconnect);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionReconnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        monitor->setText(QString());
        user_1->setText(QString());
        user_2->setText(QString());
        user_3->setText(QString());
        user_4->setText(QString());
        user_5->setText(QString());
        user_6->setText(QString());
        user_7->setText(QString());
        user_8->setText(QString());
        user_9->setText(QString());
        user_10->setText(QString());
        connect_button->setText(QApplication::translate("MainWindow", "Connect", 0));
        capture_button->setText(QApplication::translate("MainWindow", "Capture", 0));
        stop_button->setText(QApplication::translate("MainWindow", "Stop", 0));
        menuSetting->setTitle(QApplication::translate("MainWindow", "Setting", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWHP6496_H
