/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionClientParams;
    QAction *actionSaveConfig;
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutMain;
    QHBoxLayout *controlsLayout;
    QPushButton *btnStartServer;
    QPushButton *btnStopServer;
    QPushButton *btnStartClients;
    QPushButton *btnStopClients;
    QPushButton *btnSettings;
    QPushButton *btnClearLog;
    QSpacerItem *horizontalSpacer;
    QLabel *lblServerStatus;
    QSplitter *splitter;
    QGroupBox *groupClients;
    QVBoxLayout *layoutClients;
    QTableWidget *tableClients;
    QGroupBox *groupData;
    QVBoxLayout *layoutData;
    QTableWidget *tableData;
    QGroupBox *groupLog;
    QVBoxLayout *layoutLog;
    QTextEdit *textLog;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSettings;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1100, 750);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionClientParams = new QAction(MainWindow);
        actionClientParams->setObjectName("actionClientParams");
        actionSaveConfig = new QAction(MainWindow);
        actionSaveConfig->setObjectName("actionSaveConfig");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutMain = new QVBoxLayout(centralwidget);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        controlsLayout = new QHBoxLayout();
        controlsLayout->setObjectName("controlsLayout");
        btnStartServer = new QPushButton(centralwidget);
        btnStartServer->setObjectName("btnStartServer");

        controlsLayout->addWidget(btnStartServer);

        btnStopServer = new QPushButton(centralwidget);
        btnStopServer->setObjectName("btnStopServer");
        btnStopServer->setEnabled(false);

        controlsLayout->addWidget(btnStopServer);

        btnStartClients = new QPushButton(centralwidget);
        btnStartClients->setObjectName("btnStartClients");

        controlsLayout->addWidget(btnStartClients);

        btnStopClients = new QPushButton(centralwidget);
        btnStopClients->setObjectName("btnStopClients");
        btnStopClients->setEnabled(false);

        controlsLayout->addWidget(btnStopClients);

        btnSettings = new QPushButton(centralwidget);
        btnSettings->setObjectName("btnSettings");

        controlsLayout->addWidget(btnSettings);

        btnClearLog = new QPushButton(centralwidget);
        btnClearLog->setObjectName("btnClearLog");

        controlsLayout->addWidget(btnClearLog);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        controlsLayout->addItem(horizontalSpacer);

        lblServerStatus = new QLabel(centralwidget);
        lblServerStatus->setObjectName("lblServerStatus");
        lblServerStatus->setStyleSheet(QString::fromUtf8("color: #b00; font-weight: bold;"));

        controlsLayout->addWidget(lblServerStatus);


        verticalLayoutMain->addLayout(controlsLayout);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        groupClients = new QGroupBox(splitter);
        groupClients->setObjectName("groupClients");
        layoutClients = new QVBoxLayout(groupClients);
        layoutClients->setObjectName("layoutClients");
        tableClients = new QTableWidget(groupClients);
        if (tableClients->columnCount() < 5)
            tableClients->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableClients->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableClients->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableClients->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableClients->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableClients->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableClients->setObjectName("tableClients");
        tableClients->setAlternatingRowColors(true);
        tableClients->setSelectionBehavior(QAbstractItemView::SelectItems);

        layoutClients->addWidget(tableClients);

        splitter->addWidget(groupClients);
        groupData = new QGroupBox(splitter);
        groupData->setObjectName("groupData");
        layoutData = new QVBoxLayout(groupData);
        layoutData->setObjectName("layoutData");
        tableData = new QTableWidget(groupData);
        if (tableData->columnCount() < 4)
            tableData->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        tableData->setObjectName("tableData");
        tableData->setAlternatingRowColors(true);
        tableData->setSelectionBehavior(QAbstractItemView::SelectItems);

        layoutData->addWidget(tableData);

        splitter->addWidget(groupData);
        groupLog = new QGroupBox(splitter);
        groupLog->setObjectName("groupLog");
        layoutLog = new QVBoxLayout(groupLog);
        layoutLog->setObjectName("layoutLog");
        textLog = new QTextEdit(groupLog);
        textLog->setObjectName("textLog");
        QFont font;
        font.setFamilies({QString::fromUtf8("Monospace")});
        textLog->setFont(font);
        textLog->setReadOnly(true);

        layoutLog->addWidget(textLog);

        splitter->addWidget(groupLog);

        verticalLayoutMain->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1100, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName("menuSettings");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuSettings->addAction(actionClientParams);
        menuSettings->addAction(actionSaveConfig);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Server", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionClientParams->setText(QCoreApplication::translate("MainWindow", "Client Parameters...", nullptr));
        actionSaveConfig->setText(QCoreApplication::translate("MainWindow", "Save Configuration", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        btnStartServer->setText(QCoreApplication::translate("MainWindow", "Start Server", nullptr));
        btnStopServer->setText(QCoreApplication::translate("MainWindow", "Stop Server", nullptr));
        btnStartClients->setText(QCoreApplication::translate("MainWindow", "Start Clients", nullptr));
        btnStopClients->setText(QCoreApplication::translate("MainWindow", "Stop Clients", nullptr));
        btnSettings->setText(QCoreApplication::translate("MainWindow", "Client Settings...", nullptr));
        btnClearLog->setText(QCoreApplication::translate("MainWindow", "Clear Log", nullptr));
        lblServerStatus->setText(QCoreApplication::translate("MainWindow", "Server: Stopped", nullptr));
        groupClients->setTitle(QCoreApplication::translate("MainWindow", "Connected Clients", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableClients->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Client ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableClients->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "IP Address", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableClients->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Port", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableClients->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableClients->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Connected At", nullptr));
        groupData->setTitle(QCoreApplication::translate("MainWindow", "Client Data", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableData->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Client ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableData->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableData->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Content", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableData->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Received At", nullptr));
        groupLog->setTitle(QCoreApplication::translate("MainWindow", "Event Log", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
