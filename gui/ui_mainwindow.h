/********************************************************************************
** Form generated from reading UI file 'mainwindowHp6964.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWHP6964_H
#define MAINWINDOWHP6964_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
    QWidget *centralwidget;
    QLabel *monitor;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        monitor = new QLabel(centralwidget);
        monitor->setObjectName(QStringLiteral("monitor"));
        monitor->setGeometry(QRect(40, 30, 581, 471));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(660, 340, 101, 83));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        connect_button = new QPushButton(verticalLayoutWidget);
        connect_button->setObjectName(QStringLiteral("connect_button"));

        verticalLayout->addWidget(connect_button);

        capture_button = new QPushButton(verticalLayoutWidget);
        capture_button->setObjectName(QStringLiteral("capture_button"));

        verticalLayout->addWidget(capture_button);

        stop_button = new QPushButton(verticalLayoutWidget);
        stop_button->setObjectName(QStringLiteral("stop_button"));

        verticalLayout->addWidget(stop_button);

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

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionReconnect->setText(QApplication::translate("MainWindow", "Reconnect", 0));
        monitor->setText(QString());
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

#endif // MAINWINDOWHP6964_H
