/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QTabWidget *tabWidget;
    QGroupBox *about;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QWidget *sysLog;
    QGridLayout *gridLayout_2;
    QListWidget *systemLog;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1400, 800);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setMaximumSize(QSize(1400, 800));
        MainWindow->setBaseSize(QSize(1200, 800));
        MainWindow->setLayoutDirection(Qt::LeftToRight);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMinimumSize(QSize(0, 0));
        centralWidget->setMaximumSize(QSize(1400, 1400));
        centralWidget->setBaseSize(QSize(1000, 800));
        centralWidget->setAutoFillBackground(true);
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setSizeConstraint(QLayout::SetMinimumSize);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(3);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setElideMode(Qt::ElideRight);
        about = new QGroupBox();
        about->setObjectName(QString::fromUtf8("about"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(about->sizePolicy().hasHeightForWidth());
        about->setSizePolicy(sizePolicy3);
        gridLayout = new QGridLayout(about);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(about);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy4);
        label_4->setTextFormat(Qt::PlainText);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        tabWidget->addTab(about, QString());
        sysLog = new QWidget();
        sysLog->setObjectName(QString::fromUtf8("sysLog"));
        gridLayout_2 = new QGridLayout(sysLog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        systemLog = new QListWidget(sysLog);
        systemLog->setObjectName(QString::fromUtf8("systemLog"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(systemLog->sizePolicy().hasHeightForWidth());
        systemLog->setSizePolicy(sizePolicy5);
        systemLog->setFrameShape(QFrame::WinPanel);
        systemLog->setFrameShadow(QFrame::Sunken);
        systemLog->setLayoutMode(QListView::SinglePass);
        systemLog->setSpacing(0);
        systemLog->setViewMode(QListView::ListMode);
        systemLog->setModelColumn(0);
        systemLog->setWordWrap(true);
        systemLog->setSelectionRectVisible(false);

        gridLayout_2->addWidget(systemLog, 0, 0, 1, 1);

        tabWidget->addTab(sysLog, QString());

        gridLayout_4->addWidget(tabWidget, 0, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(1);
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

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        label_4->setText(QCoreApplication::translate("MainWindow", " After pushing \"Start\" button program trying to connect controller\n"
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
        tabWidget->setTabText(tabWidget->indexOf(about), QCoreApplication::translate("MainWindow", "about", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(sysLog), QCoreApplication::translate("MainWindow", "Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
