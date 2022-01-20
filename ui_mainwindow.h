/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QTreeWidget *treeWidget;
    QTabWidget *tabWidget;
    QWidget *Settings;
    QGridLayout *gridLayout_2;
    QWidget *directHex;
    QGridLayout *gridLayout_3;
    QWidget *about;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_10;
    QLabel *label_4;
    QListWidget *systemLog;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(673, 645);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setMaximumSize(QSize(10000, 10000));
        MainWindow->setBaseSize(QSize(1000, 800));
        MainWindow->setLayoutDirection(Qt::LeftToRight);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMinimumSize(QSize(0, 0));
        centralWidget->setMaximumSize(QSize(10000, 10000));
        centralWidget->setBaseSize(QSize(1000, 800));
        centralWidget->setAutoFillBackground(true);
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        treeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("ROM metadata"));
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignCenter);
        __qtreewidgetitem->setBackground(0, QColor(120, 120, 120));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy2);
        treeWidget->setFrameShape(QFrame::WinPanel);
        treeWidget->setHeaderHidden(true);
        treeWidget->setColumnCount(1);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setDefaultSectionSize(100);

        gridLayout_4->addWidget(treeWidget, 0, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy3);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setElideMode(Qt::ElideRight);
        Settings = new QWidget();
        Settings->setObjectName(QString::fromUtf8("Settings"));
        sizePolicy1.setHeightForWidth(Settings->sizePolicy().hasHeightForWidth());
        Settings->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(Settings);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget->addTab(Settings, QString());
        directHex = new QWidget();
        directHex->setObjectName(QString::fromUtf8("directHex"));
        gridLayout_3 = new QGridLayout(directHex);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidget->addTab(directHex, QString());
        about = new QWidget();
        about->setObjectName(QString::fromUtf8("about"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(about->sizePolicy().hasHeightForWidth());
        about->setSizePolicy(sizePolicy4);
        gridLayout = new QGridLayout(about);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_11 = new QGroupBox(about);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        sizePolicy2.setHeightForWidth(groupBox_11->sizePolicy().hasHeightForWidth());
        groupBox_11->setSizePolicy(sizePolicy2);
        gridLayout_10 = new QGridLayout(groupBox_11);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        label_4 = new QLabel(groupBox_11);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy5.setHorizontalStretch(93);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy5);
        label_4->setTextFormat(Qt::PlainText);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_10->addWidget(label_4, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox_11, 0, 0, 1, 1);

        tabWidget->addTab(about, QString());

        gridLayout_4->addWidget(tabWidget, 0, 1, 1, 1);

        systemLog = new QListWidget(centralWidget);
        systemLog->setObjectName(QString::fromUtf8("systemLog"));
        sizePolicy3.setHeightForWidth(systemLog->sizePolicy().hasHeightForWidth());
        systemLog->setSizePolicy(sizePolicy3);
        systemLog->setFrameShape(QFrame::WinPanel);
        systemLog->setFrameShadow(QFrame::Sunken);
        systemLog->setLayoutMode(QListView::SinglePass);
        systemLog->setSpacing(0);
        systemLog->setViewMode(QListView::ListMode);
        systemLog->setModelColumn(0);
        systemLog->setWordWrap(true);
        systemLog->setSelectionRectVisible(false);

        gridLayout_4->addWidget(systemLog, 1, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy6);
        statusBar->setMinimumSize(QSize(0, 20));
        QFont font;
        statusBar->setFont(font);
        statusBar->setFocusPolicy(Qt::ClickFocus);
        statusBar->setAutoFillBackground(true);
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        tabWidget->setTabText(tabWidget->indexOf(Settings), QApplication::translate("MainWindow", "Connection parameters", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(directHex), QApplication::translate("MainWindow", "direct RAM-ROM reader", nullptr));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "HowTo", nullptr));
        label_4->setText(QApplication::translate("MainWindow", " After pushing \"Start\" button program trying to connect controller\n"
"using MUT protocol. In success, program reads calibration \n"
"numbers from controller's memory. By using calibration number, \n"
"program searching for xml file in home folder. The name of file \n"
"should include calibrartion number - ROM ID. If xml found, then it \n"
"parses and screen forms tables will be created. The structure of \n"
"xml file is identical to xml structure of ECUFLASH with extended \n"
"syntax. In fact, to the description of the table you want to add \n"
"RAM_addr parameter address table in RAM (1D tables are not\n"
"supported yet). And a separate parameter DEAD_var address \n"
"of this variable. I have ready a patch for GTA 80770010 evo \n"
"(airtrek) and 90550001 evo7 Ralliart my rom without tephra mod.\n"
"  After edit cells (select another cell or push enter) value \n"
"Immediately send's to ECU. This client worked with two interface,\n"
" OpenPort1.3 and OpenPort2.0. For make patch use \n"
"tuner pro from http:/"
                        "/www.tunerpro.net/downloadApp.htm, and \n"
"xdf file.\n"
"\n"
"John Banks: aka JCSBanks on http://evolutionM.net creator \n"
"DMA logging for Mitsu.", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(about), QApplication::translate("MainWindow", "about", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
