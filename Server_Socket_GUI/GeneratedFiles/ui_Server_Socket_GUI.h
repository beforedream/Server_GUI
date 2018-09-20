/********************************************************************************
** Form generated from reading UI file 'Server_Socket_GUI.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_SOCKET_GUI_H
#define UI_SERVER_SOCKET_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Server_Socket_GUIClass
{
public:
    QTableWidget *tableWidget;
    QPushButton *StartButton;
    QPushButton *StopButton;
    QTextBrowser *textBrowser;
    QPushButton *CloseButton;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Server_Socket_GUIClass)
    {
        if (Server_Socket_GUIClass->objectName().isEmpty())
            Server_Socket_GUIClass->setObjectName(QStringLiteral("Server_Socket_GUIClass"));
        Server_Socket_GUIClass->resize(642, 400);
        tableWidget = new QTableWidget(Server_Socket_GUIClass);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 30, 341, 291));
        tableWidget->setAutoScrollMargin(16);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setTabKeyNavigation(false);
        tableWidget->setProperty("showDropIndicator", QVariant(false));
        tableWidget->setDragEnabled(true);
        tableWidget->setDragDropOverwriteMode(true);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(4);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setDefaultSectionSize(70);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setDefaultSectionSize(20);
        tableWidget->verticalHeader()->setHighlightSections(false);
        tableWidget->verticalHeader()->setMinimumSectionSize(10);
        tableWidget->verticalHeader()->setStretchLastSection(false);
        StartButton = new QPushButton(Server_Socket_GUIClass);
        StartButton->setObjectName(QStringLiteral("StartButton"));
        StartButton->setGeometry(QRect(60, 340, 91, 31));
        StopButton = new QPushButton(Server_Socket_GUIClass);
        StopButton->setObjectName(QStringLiteral("StopButton"));
        StopButton->setGeometry(QRect(270, 340, 101, 31));
        textBrowser = new QTextBrowser(Server_Socket_GUIClass);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(370, 30, 256, 291));
        CloseButton = new QPushButton(Server_Socket_GUIClass);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(460, 340, 101, 31));
        label = new QLabel(Server_Socket_GUIClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 54, 12));
        label_2 = new QLabel(Server_Socket_GUIClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(340, 10, 54, 12));

        retranslateUi(Server_Socket_GUIClass);
        QObject::connect(StartButton, SIGNAL(clicked()), Server_Socket_GUIClass, SLOT(Start()));
        QObject::connect(StopButton, SIGNAL(clicked()), Server_Socket_GUIClass, SLOT(Stop()));
        QObject::connect(CloseButton, SIGNAL(clicked()), Server_Socket_GUIClass, SLOT(Close()));

        QMetaObject::connectSlotsByName(Server_Socket_GUIClass);
    } // setupUi

    void retranslateUi(QWidget *Server_Socket_GUIClass)
    {
        Server_Socket_GUIClass->setWindowTitle(QApplication::translate("Server_Socket_GUIClass", "Server_Socket_GUI", Q_NULLPTR));
        StartButton->setText(QApplication::translate("Server_Socket_GUIClass", "\345\220\257\345\212\250", Q_NULLPTR));
        StopButton->setText(QApplication::translate("Server_Socket_GUIClass", "\345\201\234\346\255\242", Q_NULLPTR));
        CloseButton->setText(QApplication::translate("Server_Socket_GUIClass", "\351\200\200\345\207\272", Q_NULLPTR));
        label->setText(QApplication::translate("Server_Socket_GUIClass", "\347\224\250\346\210\267\345\210\227\350\241\250\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("Server_Socket_GUIClass", "\344\272\213\344\273\266\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Server_Socket_GUIClass: public Ui_Server_Socket_GUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_SOCKET_GUI_H
