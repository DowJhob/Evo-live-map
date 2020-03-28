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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *live_tab;
    QGridLayout *gridLayout_2;
    QSplitter *splitter_2;
    QWidget *verticalLayoutWidget_5;
    QGridLayout *gridLayout_17;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_18;
    QVBoxLayout *verticalLayout_5;
    QPushButton *StartButton;
    QPushButton *read_RAM_Button;
    QPushButton *RAM_reset_Button;
    QPushButton *save_trace_pushButton;
    QPushButton *debugButton;
    QPushButton *stop_live;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_19;
    QGridLayout *gridLayout_20;
    QLabel *BaudRatelabel_7;
    QLineEdit *BaudRatelineEdit;
    QLineEdit *logger_rate_textedit;
    QLabel *BaudRatelabel_8;
    QLabel *BaudRatelabel_9;
    QFrame *line_4;
    QRadioButton *radioButton_7;
    QRadioButton *radioButton_8;
    QRadioButton *radioButton_9;
    QLabel *label_3;
    QLabel *trace_time_label;
    QLabel *label_7;
    QPushButton *inno_initButton;
    QGroupBox *groupBox_mapalloc;
    QGridLayout *gridLayout_21;
    QGridLayout *gridLayout_mapalloc;
    QSpacerItem *verticalSpacer_3;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QListWidget *listWidget;
    QWidget *patcher;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QListView *listView;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QLabel *label;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_7;
    QLabel *label_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QPushButton *create;
    QPushButton *delete_2;
    QPushButton *loadbinbutton;
    QPushButton *savebinbutton;
    QPushButton *loadpatchbutton;
    QPushButton *savepatchbutton;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_8;
    QListView *listView_2;
    QPlainTextEdit *RAWfileVIEWplainTextEdit;
    QWidget *about;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_9;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_10;
    QLabel *label_4;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_11;
    QLabel *label_5;
    QFrame *line_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(934, 650);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(0, 0));
        centralWidget->setMaximumSize(QSize(10000, 10000));
        centralWidget->setBaseSize(QSize(1000, 800));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setAutoFillBackground(false);
        live_tab = new QWidget();
        live_tab->setObjectName(QString::fromUtf8("live_tab"));
        sizePolicy.setHeightForWidth(live_tab->sizePolicy().hasHeightForWidth());
        live_tab->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(live_tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        splitter_2 = new QSplitter(live_tab);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setFrameShape(QFrame::WinPanel);
        splitter_2->setFrameShadow(QFrame::Sunken);
        splitter_2->setMidLineWidth(5);
        splitter_2->setOrientation(Qt::Horizontal);
        verticalLayoutWidget_5 = new QWidget(splitter_2);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        gridLayout_17 = new QGridLayout(verticalLayoutWidget_5);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        gridLayout_17->setContentsMargins(0, 0, 0, 0);
        groupBox_7 = new QGroupBox(verticalLayoutWidget_5);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_7->sizePolicy().hasHeightForWidth());
        groupBox_7->setSizePolicy(sizePolicy1);
        gridLayout_18 = new QGridLayout(groupBox_7);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        StartButton = new QPushButton(groupBox_7);
        StartButton->setObjectName(QString::fromUtf8("StartButton"));
        sizePolicy.setHeightForWidth(StartButton->sizePolicy().hasHeightForWidth());
        StartButton->setSizePolicy(sizePolicy);
        StartButton->setMaximumSize(QSize(3000, 3000));
        StartButton->setBaseSize(QSize(200, 200));
        StartButton->setLayoutDirection(Qt::LeftToRight);
        StartButton->setAutoFillBackground(true);
        StartButton->setAutoDefault(true);
        StartButton->setFlat(false);

        verticalLayout_5->addWidget(StartButton);

        read_RAM_Button = new QPushButton(groupBox_7);
        read_RAM_Button->setObjectName(QString::fromUtf8("read_RAM_Button"));
        read_RAM_Button->setEnabled(true);
        sizePolicy1.setHeightForWidth(read_RAM_Button->sizePolicy().hasHeightForWidth());
        read_RAM_Button->setSizePolicy(sizePolicy1);
        read_RAM_Button->setCheckable(false);

        verticalLayout_5->addWidget(read_RAM_Button);

        RAM_reset_Button = new QPushButton(groupBox_7);
        RAM_reset_Button->setObjectName(QString::fromUtf8("RAM_reset_Button"));
        sizePolicy1.setHeightForWidth(RAM_reset_Button->sizePolicy().hasHeightForWidth());
        RAM_reset_Button->setSizePolicy(sizePolicy1);
        RAM_reset_Button->setMinimumSize(QSize(0, 20));

        verticalLayout_5->addWidget(RAM_reset_Button);

        save_trace_pushButton = new QPushButton(groupBox_7);
        save_trace_pushButton->setObjectName(QString::fromUtf8("save_trace_pushButton"));
        sizePolicy1.setHeightForWidth(save_trace_pushButton->sizePolicy().hasHeightForWidth());
        save_trace_pushButton->setSizePolicy(sizePolicy1);

        verticalLayout_5->addWidget(save_trace_pushButton);

        debugButton = new QPushButton(groupBox_7);
        debugButton->setObjectName(QString::fromUtf8("debugButton"));
        sizePolicy1.setHeightForWidth(debugButton->sizePolicy().hasHeightForWidth());
        debugButton->setSizePolicy(sizePolicy1);
        debugButton->setAutoDefault(true);

        verticalLayout_5->addWidget(debugButton);

        stop_live = new QPushButton(groupBox_7);
        stop_live->setObjectName(QString::fromUtf8("stop_live"));
        sizePolicy1.setHeightForWidth(stop_live->sizePolicy().hasHeightForWidth());
        stop_live->setSizePolicy(sizePolicy1);

        verticalLayout_5->addWidget(stop_live);


        gridLayout_18->addLayout(verticalLayout_5, 0, 0, 1, 1);


        gridLayout_17->addWidget(groupBox_7, 1, 0, 1, 1);

        groupBox_8 = new QGroupBox(verticalLayoutWidget_5);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_8->sizePolicy().hasHeightForWidth());
        groupBox_8->setSizePolicy(sizePolicy2);
        gridLayout_19 = new QGridLayout(groupBox_8);
        gridLayout_19->setSpacing(6);
        gridLayout_19->setContentsMargins(11, 11, 11, 11);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        gridLayout_20 = new QGridLayout();
        gridLayout_20->setSpacing(6);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        BaudRatelabel_7 = new QLabel(groupBox_8);
        BaudRatelabel_7->setObjectName(QString::fromUtf8("BaudRatelabel_7"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(BaudRatelabel_7->sizePolicy().hasHeightForWidth());
        BaudRatelabel_7->setSizePolicy(sizePolicy3);
        BaudRatelabel_7->setFrameShape(QFrame::StyledPanel);
        BaudRatelabel_7->setFrameShadow(QFrame::Sunken);
        BaudRatelabel_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_20->addWidget(BaudRatelabel_7, 3, 0, 1, 1);

        BaudRatelineEdit = new QLineEdit(groupBox_8);
        BaudRatelineEdit->setObjectName(QString::fromUtf8("BaudRatelineEdit"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(BaudRatelineEdit->sizePolicy().hasHeightForWidth());
        BaudRatelineEdit->setSizePolicy(sizePolicy4);
        BaudRatelineEdit->setMaximumSize(QSize(50, 16777215));
        BaudRatelineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_20->addWidget(BaudRatelineEdit, 3, 1, 1, 1);

        logger_rate_textedit = new QLineEdit(groupBox_8);
        logger_rate_textedit->setObjectName(QString::fromUtf8("logger_rate_textedit"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(logger_rate_textedit->sizePolicy().hasHeightForWidth());
        logger_rate_textedit->setSizePolicy(sizePolicy5);
        logger_rate_textedit->setMaximumSize(QSize(50, 16777215));
        logger_rate_textedit->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        gridLayout_20->addWidget(logger_rate_textedit, 1, 1, 1, 1);

        BaudRatelabel_8 = new QLabel(groupBox_8);
        BaudRatelabel_8->setObjectName(QString::fromUtf8("BaudRatelabel_8"));
        sizePolicy3.setHeightForWidth(BaudRatelabel_8->sizePolicy().hasHeightForWidth());
        BaudRatelabel_8->setSizePolicy(sizePolicy3);
        BaudRatelabel_8->setFrameShadow(QFrame::Raised);

        gridLayout_20->addWidget(BaudRatelabel_8, 1, 2, 1, 1);

        BaudRatelabel_9 = new QLabel(groupBox_8);
        BaudRatelabel_9->setObjectName(QString::fromUtf8("BaudRatelabel_9"));
        sizePolicy3.setHeightForWidth(BaudRatelabel_9->sizePolicy().hasHeightForWidth());
        BaudRatelabel_9->setSizePolicy(sizePolicy3);
        BaudRatelabel_9->setLayoutDirection(Qt::LeftToRight);
        BaudRatelabel_9->setFrameShadow(QFrame::Sunken);

        gridLayout_20->addWidget(BaudRatelabel_9, 1, 0, 1, 1);

        line_4 = new QFrame(groupBox_8);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(line_4->sizePolicy().hasHeightForWidth());
        line_4->setSizePolicy(sizePolicy6);
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_20->addWidget(line_4, 2, 0, 1, 4);

        radioButton_7 = new QRadioButton(groupBox_8);
        radioButton_7->setObjectName(QString::fromUtf8("radioButton_7"));
        sizePolicy3.setHeightForWidth(radioButton_7->sizePolicy().hasHeightForWidth());
        radioButton_7->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(radioButton_7, 5, 2, 1, 1);

        radioButton_8 = new QRadioButton(groupBox_8);
        radioButton_8->setObjectName(QString::fromUtf8("radioButton_8"));
        sizePolicy3.setHeightForWidth(radioButton_8->sizePolicy().hasHeightForWidth());
        radioButton_8->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(radioButton_8, 4, 2, 1, 1);

        radioButton_9 = new QRadioButton(groupBox_8);
        radioButton_9->setObjectName(QString::fromUtf8("radioButton_9"));
        sizePolicy3.setHeightForWidth(radioButton_9->sizePolicy().hasHeightForWidth());
        radioButton_9->setSizePolicy(sizePolicy3);
        radioButton_9->setChecked(true);

        gridLayout_20->addWidget(radioButton_9, 3, 2, 1, 1);

        label_3 = new QLabel(groupBox_8);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(label_3, 3, 3, 1, 1);

        trace_time_label = new QLabel(groupBox_8);
        trace_time_label->setObjectName(QString::fromUtf8("trace_time_label"));

        gridLayout_20->addWidget(trace_time_label, 4, 1, 1, 1);

        label_7 = new QLabel(groupBox_8);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_20->addWidget(label_7, 4, 0, 1, 1);

        inno_initButton = new QPushButton(groupBox_8);
        inno_initButton->setObjectName(QString::fromUtf8("inno_initButton"));

        gridLayout_20->addWidget(inno_initButton, 5, 0, 1, 1);


        gridLayout_19->addLayout(gridLayout_20, 1, 0, 1, 2);


        gridLayout_17->addWidget(groupBox_8, 0, 0, 1, 1);

        groupBox_mapalloc = new QGroupBox(verticalLayoutWidget_5);
        groupBox_mapalloc->setObjectName(QString::fromUtf8("groupBox_mapalloc"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(groupBox_mapalloc->sizePolicy().hasHeightForWidth());
        groupBox_mapalloc->setSizePolicy(sizePolicy7);
        gridLayout_21 = new QGridLayout(groupBox_mapalloc);
        gridLayout_21->setSpacing(6);
        gridLayout_21->setContentsMargins(11, 11, 11, 11);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        gridLayout_mapalloc = new QGridLayout();
        gridLayout_mapalloc->setSpacing(6);
        gridLayout_mapalloc->setObjectName(QString::fromUtf8("gridLayout_mapalloc"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_mapalloc->addItem(verticalSpacer_3, 0, 0, 1, 1);


        gridLayout_21->addLayout(gridLayout_mapalloc, 0, 0, 1, 1);


        gridLayout_17->addWidget(groupBox_mapalloc, 0, 1, 2, 1);

        splitter_2->addWidget(verticalLayoutWidget_5);
        verticalLayoutWidget_6 = new QWidget(splitter_2);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(verticalLayoutWidget_6);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        listWidget->setFrameShape(QFrame::WinPanel);
        listWidget->setFrameShadow(QFrame::Raised);
        listWidget->setLayoutMode(QListView::SinglePass);
        listWidget->setSpacing(0);
        listWidget->setViewMode(QListView::ListMode);
        listWidget->setModelColumn(0);
        listWidget->setWordWrap(true);
        listWidget->setSelectionRectVisible(false);

        verticalLayout_6->addWidget(listWidget);

        splitter_2->addWidget(verticalLayoutWidget_6);

        gridLayout_2->addWidget(splitter_2, 0, 0, 1, 1);

        tabWidget->addTab(live_tab, QString());
        patcher = new QWidget();
        patcher->setObjectName(QString::fromUtf8("patcher"));
        gridLayout_3 = new QGridLayout(patcher);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_2 = new QGroupBox(patcher);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        listView = new QListView(groupBox_2);
        listView->setObjectName(QString::fromUtf8("listView"));

        gridLayout_4->addWidget(listView, 1, 1, 1, 2);

        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_6->addWidget(label, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_3, 0, 1, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_7 = new QGridLayout(groupBox_4);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_7->addWidget(label_2, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_4, 0, 2, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 3, 1, 1);

        groupBox = new QGroupBox(patcher);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        create = new QPushButton(groupBox);
        create->setObjectName(QString::fromUtf8("create"));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(create->sizePolicy().hasHeightForWidth());
        create->setSizePolicy(sizePolicy8);

        verticalLayout->addWidget(create);

        delete_2 = new QPushButton(groupBox);
        delete_2->setObjectName(QString::fromUtf8("delete_2"));

        verticalLayout->addWidget(delete_2);

        loadbinbutton = new QPushButton(groupBox);
        loadbinbutton->setObjectName(QString::fromUtf8("loadbinbutton"));

        verticalLayout->addWidget(loadbinbutton);

        savebinbutton = new QPushButton(groupBox);
        savebinbutton->setObjectName(QString::fromUtf8("savebinbutton"));

        verticalLayout->addWidget(savebinbutton);

        loadpatchbutton = new QPushButton(groupBox);
        loadpatchbutton->setObjectName(QString::fromUtf8("loadpatchbutton"));

        verticalLayout->addWidget(loadpatchbutton);

        savepatchbutton = new QPushButton(groupBox);
        savepatchbutton->setObjectName(QString::fromUtf8("savepatchbutton"));

        verticalLayout->addWidget(savepatchbutton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout_3->addWidget(groupBox, 0, 1, 1, 1);

        groupBox_5 = new QGroupBox(patcher);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        sizePolicy.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy);
        gridLayout_8 = new QGridLayout(groupBox_5);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        listView_2 = new QListView(groupBox_5);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));

        gridLayout_8->addWidget(listView_2, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_5, 0, 2, 1, 1);

        RAWfileVIEWplainTextEdit = new QPlainTextEdit(patcher);
        RAWfileVIEWplainTextEdit->setObjectName(QString::fromUtf8("RAWfileVIEWplainTextEdit"));
        QSizePolicy sizePolicy9(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(RAWfileVIEWplainTextEdit->sizePolicy().hasHeightForWidth());
        RAWfileVIEWplainTextEdit->setSizePolicy(sizePolicy9);

        gridLayout_3->addWidget(RAWfileVIEWplainTextEdit, 1, 1, 1, 3);

        tabWidget->addTab(patcher, QString());
        about = new QWidget();
        about->setObjectName(QString::fromUtf8("about"));
        gridLayout = new QGridLayout(about);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setSpacing(6);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        groupBox_11 = new QGroupBox(about);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        gridLayout_10 = new QGridLayout(groupBox_11);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        label_4 = new QLabel(groupBox_11);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy10(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy10.setHorizontalStretch(93);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy10);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_10->addWidget(label_4, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_11, 0, 0, 1, 1);

        groupBox_12 = new QGroupBox(about);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        gridLayout_11 = new QGridLayout(groupBox_12);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        label_5 = new QLabel(groupBox_12);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_11->addWidget(label_5, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_12, 0, 1, 1, 1);


        gridLayout->addLayout(gridLayout_9, 0, 0, 1, 1);

        tabWidget->addTab(about, QString());

        horizontalLayout->addWidget(tabWidget);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setEnabled(true);
        line_2->setFrameShadow(QFrame::Raised);
        line_2->setLineWidth(1);
        line_2->setMidLineWidth(1);
        line_2->setFrameShape(QFrame::HLine);

        horizontalLayout->addWidget(line_2);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        sizePolicy2.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy2);
        statusBar->setMinimumSize(QSize(0, 20));
        statusBar->setFocusPolicy(Qt::ClickFocus);
        statusBar->setAutoFillBackground(false);
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "eulle@ya.ru     v 1.1", nullptr));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Control section", nullptr));
        StartButton->setText(QApplication::translate("MainWindow", "Start", nullptr));
        read_RAM_Button->setText(QApplication::translate("MainWindow", "read RAM", nullptr));
        RAM_reset_Button->setText(QApplication::translate("MainWindow", "reset RAM", nullptr));
        save_trace_pushButton->setText(QApplication::translate("MainWindow", "save_trace", nullptr));
        debugButton->setText(QApplication::translate("MainWindow", "debug", nullptr));
        stop_live->setText(QApplication::translate("MainWindow", "Stop for debug", nullptr));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Connection parameter", nullptr));
        BaudRatelabel_7->setText(QApplication::translate("MainWindow", "Baud rate:", nullptr));
        BaudRatelineEdit->setText(QApplication::translate("MainWindow", "15625", nullptr));
        logger_rate_textedit->setText(QApplication::translate("MainWindow", "100", nullptr));
        logger_rate_textedit->setPlaceholderText(QString());
        BaudRatelabel_8->setText(QApplication::translate("MainWindow", "Hz", nullptr));
        BaudRatelabel_9->setText(QApplication::translate("MainWindow", "Logging rate:", nullptr));
        radioButton_7->setText(QApplication::translate("MainWindow", "115200", nullptr));
        radioButton_8->setText(QApplication::translate("MainWindow", "62200", nullptr));
        radioButton_9->setText(QApplication::translate("MainWindow", "15625", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "baud", nullptr));
        trace_time_label->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270 \321\202\321\200\320\265\320\271\321\201\320\265\321\200\320\260 \320\262\321\201\320\265\321\205 \321\202\320\260\320\261\320\273\320\270\321\206", nullptr));
        inno_initButton->setText(QApplication::translate("MainWindow", "inno_init", nullptr));
        groupBox_mapalloc->setTitle(QString());
        tabWidget->setTabText(tabWidget->indexOf(live_tab), QApplication::translate("MainWindow", "live", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "edit", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Address", nullptr));
        label->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Lenght", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "control", nullptr));
        create->setText(QApplication::translate("MainWindow", "Create", nullptr));
        delete_2->setText(QApplication::translate("MainWindow", "Delete", nullptr));
        loadbinbutton->setText(QApplication::translate("MainWindow", "Load .bin", nullptr));
        savebinbutton->setText(QApplication::translate("MainWindow", "Save .bin", nullptr));
        loadpatchbutton->setText(QApplication::translate("MainWindow", "Load .patch", nullptr));
        savepatchbutton->setText(QApplication::translate("MainWindow", "Save .patch", nullptr));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "patch list", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(patcher), QApplication::translate("MainWindow", "patcher", nullptr));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "HowTo", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">  After pushing &quot;Start&quot; button program trying to connect controller</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">using MUT protocol. In success, program reads calibration </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">numbers from controller's memory. By using calibr"
                        "ation number, </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">program searching for xml file in home folder. The name of file </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">should include calibrartion number - ROM ID. If xml found, then it </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">parses and screen forms tables will be created. The structure of </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">xml file is identical to xml structure of ECUFLASH with extended </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12p"
                        "x; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">syntax. </span><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">In fact, to the description of the table you want to add </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">RAM_addr parameter address table in RAM (1D tables are not</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">supported yet). And a separate parameter DEAD_var address </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Ar"
                        "ial,sans-serif'; font-size:10pt; color:#1d2129;\">of this variable. I have ready a patch for GTA 80770010 evo </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">(airtrek) and 90550001 evo7 Ralliart my rom without tephra mod.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">  After edit cells (select another cell or push enter) value </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">Immediately send's to ECU. This client worked with two interface,</span></p>\n"
"<p style=\" margin-top:"
                        "12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\"> OpenPort1.3 and OpenPort2.0 (slowly). For make patch use </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">tuner pro from </span><a href=\"http://www.tunerpro.net/downloadApp.htm\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">http://www.tunerpro.net/downloadApp.htm</span></a><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">, and </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetica,Arial,sans-serif'; font-size:10pt; color:#1d2129;\">xdf file.</span></p></b"
                        "ody></html>", nullptr));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "Parents", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">John Banks: aka JCSBanks on </span><a href=\"http://evolutionM.net\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">http://evolutionM.net</span></a><span style=\" font-size:10pt;\"> creator </span></p><p><span style=\" font-size:10pt;\">DMA logging for Mitsu.</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(about), QApplication::translate("MainWindow", "about", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
