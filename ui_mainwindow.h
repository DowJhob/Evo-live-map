/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
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
#include <QtWidgets/QHeaderView>
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
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_5;
    QSplitter *splitter;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *tabWidget;
    QWidget *live_tab;
    QGridLayout *gridLayout_2;
    QSplitter *splitter_2;
    QWidget *verticalLayoutWidget_5;
    QGridLayout *gridLayout_17;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_19;
    QGridLayout *gridLayout_20;
    QLabel *BaudRate_label;
    QLineEdit *BaudRatelineEdit;
    QLineEdit *logger_rate_textedit;
    QLabel *logRate_hz_label;
    QLabel *logRate_label;
    QFrame *line_4;
    QRadioButton *_115200_radioButton;
    QRadioButton *_62200_radioButton;
    QRadioButton *_15625_radioButton;
    QLabel *baud_label;
    QLabel *trace_time_label;
    QLabel *redraw_time_label;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_18;
    QVBoxLayout *verticalLayout_5;
    QPushButton *save_trace_pushButton;
    QPushButton *stop_live;
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
    QWidget *tab;
    QGridLayout *gridLayout_12;
    QGridLayout *RAMeditorLayout;
    QLineEdit *start_addr_lineEdit;
    QLineEdit *count_lineEdit;
    QLabel *label_6;
    QLabel *label_8;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1157, 636);
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
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(20);
        horizontalLayoutWidget = new QWidget(splitter);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(horizontalLayoutWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("ROM metadata"));
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignCenter);
        __qtreewidgetitem->setBackground(0, QColor(120, 120, 120));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        treeWidget->setFrameShape(QFrame::WinPanel);
        treeWidget->setHeaderHidden(true);
        treeWidget->setColumnCount(1);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setDefaultSectionSize(100);

        horizontalLayout->addWidget(treeWidget);

        splitter->addWidget(horizontalLayoutWidget);
        horizontalLayoutWidget_2 = new QWidget(splitter);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(horizontalLayoutWidget_2);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setElideMode(Qt::ElideRight);
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
        BaudRate_label = new QLabel(groupBox_8);
        BaudRate_label->setObjectName(QString::fromUtf8("BaudRate_label"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(BaudRate_label->sizePolicy().hasHeightForWidth());
        BaudRate_label->setSizePolicy(sizePolicy3);
        BaudRate_label->setFrameShape(QFrame::StyledPanel);
        BaudRate_label->setFrameShadow(QFrame::Sunken);
        BaudRate_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_20->addWidget(BaudRate_label, 3, 0, 1, 1);

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

        logRate_hz_label = new QLabel(groupBox_8);
        logRate_hz_label->setObjectName(QString::fromUtf8("logRate_hz_label"));
        sizePolicy3.setHeightForWidth(logRate_hz_label->sizePolicy().hasHeightForWidth());
        logRate_hz_label->setSizePolicy(sizePolicy3);
        logRate_hz_label->setFrameShadow(QFrame::Raised);

        gridLayout_20->addWidget(logRate_hz_label, 1, 2, 1, 1);

        logRate_label = new QLabel(groupBox_8);
        logRate_label->setObjectName(QString::fromUtf8("logRate_label"));
        sizePolicy3.setHeightForWidth(logRate_label->sizePolicy().hasHeightForWidth());
        logRate_label->setSizePolicy(sizePolicy3);
        logRate_label->setLayoutDirection(Qt::LeftToRight);
        logRate_label->setFrameShadow(QFrame::Sunken);

        gridLayout_20->addWidget(logRate_label, 1, 0, 1, 1);

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

        _115200_radioButton = new QRadioButton(groupBox_8);
        _115200_radioButton->setObjectName(QString::fromUtf8("_115200_radioButton"));
        sizePolicy3.setHeightForWidth(_115200_radioButton->sizePolicy().hasHeightForWidth());
        _115200_radioButton->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(_115200_radioButton, 5, 2, 1, 1);

        _62200_radioButton = new QRadioButton(groupBox_8);
        _62200_radioButton->setObjectName(QString::fromUtf8("_62200_radioButton"));
        sizePolicy3.setHeightForWidth(_62200_radioButton->sizePolicy().hasHeightForWidth());
        _62200_radioButton->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(_62200_radioButton, 4, 2, 1, 1);

        _15625_radioButton = new QRadioButton(groupBox_8);
        _15625_radioButton->setObjectName(QString::fromUtf8("_15625_radioButton"));
        sizePolicy3.setHeightForWidth(_15625_radioButton->sizePolicy().hasHeightForWidth());
        _15625_radioButton->setSizePolicy(sizePolicy3);
        _15625_radioButton->setChecked(true);

        gridLayout_20->addWidget(_15625_radioButton, 3, 2, 1, 1);

        baud_label = new QLabel(groupBox_8);
        baud_label->setObjectName(QString::fromUtf8("baud_label"));
        sizePolicy3.setHeightForWidth(baud_label->sizePolicy().hasHeightForWidth());
        baud_label->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(baud_label, 3, 3, 1, 1);

        trace_time_label = new QLabel(groupBox_8);
        trace_time_label->setObjectName(QString::fromUtf8("trace_time_label"));

        gridLayout_20->addWidget(trace_time_label, 4, 1, 1, 1);

        redraw_time_label = new QLabel(groupBox_8);
        redraw_time_label->setObjectName(QString::fromUtf8("redraw_time_label"));

        gridLayout_20->addWidget(redraw_time_label, 4, 0, 1, 1);


        gridLayout_19->addLayout(gridLayout_20, 1, 0, 1, 2);


        gridLayout_17->addWidget(groupBox_8, 0, 0, 1, 1);

        groupBox_7 = new QGroupBox(verticalLayoutWidget_5);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(groupBox_7->sizePolicy().hasHeightForWidth());
        groupBox_7->setSizePolicy(sizePolicy7);
        gridLayout_18 = new QGridLayout(groupBox_7);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        save_trace_pushButton = new QPushButton(groupBox_7);
        save_trace_pushButton->setObjectName(QString::fromUtf8("save_trace_pushButton"));
        sizePolicy7.setHeightForWidth(save_trace_pushButton->sizePolicy().hasHeightForWidth());
        save_trace_pushButton->setSizePolicy(sizePolicy7);

        verticalLayout_5->addWidget(save_trace_pushButton);

        stop_live = new QPushButton(groupBox_7);
        stop_live->setObjectName(QString::fromUtf8("stop_live"));
        sizePolicy7.setHeightForWidth(stop_live->sizePolicy().hasHeightForWidth());
        stop_live->setSizePolicy(sizePolicy7);

        verticalLayout_5->addWidget(stop_live);


        gridLayout_18->addLayout(verticalLayout_5, 0, 0, 1, 1);


        gridLayout_17->addWidget(groupBox_7, 1, 0, 1, 1);

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
        sizePolicy1.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy1);
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
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_12 = new QGridLayout(tab);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        RAMeditorLayout = new QGridLayout();
        RAMeditorLayout->setSpacing(6);
        RAMeditorLayout->setObjectName(QString::fromUtf8("RAMeditorLayout"));
        start_addr_lineEdit = new QLineEdit(tab);
        start_addr_lineEdit->setObjectName(QString::fromUtf8("start_addr_lineEdit"));
        QFont font;
        font.setPointSize(14);
        start_addr_lineEdit->setFont(font);

        RAMeditorLayout->addWidget(start_addr_lineEdit, 2, 0, 1, 1);

        count_lineEdit = new QLineEdit(tab);
        count_lineEdit->setObjectName(QString::fromUtf8("count_lineEdit"));
        count_lineEdit->setFont(font);

        RAMeditorLayout->addWidget(count_lineEdit, 2, 1, 1, 1);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy8.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy8);

        RAMeditorLayout->addWidget(label_6, 0, 1, 1, 1);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy8.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy8);

        RAMeditorLayout->addWidget(label_8, 0, 0, 1, 1);


        gridLayout_12->addLayout(RAMeditorLayout, 2, 0, 1, 1);

        tabWidget->addTab(tab, QString());

        horizontalLayout_3->addWidget(tabWidget);

        splitter->addWidget(horizontalLayoutWidget_2);

        gridLayout_5->addWidget(splitter, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        sizePolicy2.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy2);
        statusBar->setMinimumSize(QSize(0, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Segoe UI Semibold"));
        statusBar->setFont(font1);
        statusBar->setFocusPolicy(Qt::ClickFocus);
        statusBar->setAutoFillBackground(false);
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setIconSize(QSize(128, 128));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "eulle@ya.ru     v 1.0", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "Connection parameter", nullptr));
        BaudRate_label->setText(QCoreApplication::translate("MainWindow", "Baud rate:", nullptr));
        BaudRatelineEdit->setText(QCoreApplication::translate("MainWindow", "15625", nullptr));
        logger_rate_textedit->setText(QCoreApplication::translate("MainWindow", "25", nullptr));
        logger_rate_textedit->setPlaceholderText(QString());
        logRate_hz_label->setText(QCoreApplication::translate("MainWindow", "Hz", nullptr));
        logRate_label->setText(QCoreApplication::translate("MainWindow", "Logging rate:", nullptr));
        _115200_radioButton->setText(QCoreApplication::translate("MainWindow", "115200", nullptr));
        _62200_radioButton->setText(QCoreApplication::translate("MainWindow", "62200", nullptr));
        _15625_radioButton->setText(QCoreApplication::translate("MainWindow", "15625", nullptr));
        baud_label->setText(QCoreApplication::translate("MainWindow", "baud", nullptr));
        trace_time_label->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        redraw_time_label->setText(QCoreApplication::translate("MainWindow", "redraw time all tables", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "Control section", nullptr));
        save_trace_pushButton->setText(QCoreApplication::translate("MainWindow", "save_trace", nullptr));
        stop_live->setText(QCoreApplication::translate("MainWindow", "Stop for debug", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(live_tab), QCoreApplication::translate("MainWindow", "live", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "edit", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Address", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Lenght", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "control", nullptr));
        create->setText(QCoreApplication::translate("MainWindow", "Create", nullptr));
        delete_2->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        loadbinbutton->setText(QCoreApplication::translate("MainWindow", "Load .bin", nullptr));
        savebinbutton->setText(QCoreApplication::translate("MainWindow", "Save .bin", nullptr));
        loadpatchbutton->setText(QCoreApplication::translate("MainWindow", "Load .patch", nullptr));
        savepatchbutton->setText(QCoreApplication::translate("MainWindow", "Save .patch", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "patch list", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(patcher), QCoreApplication::translate("MainWindow", "patcher", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("MainWindow", "HowTo", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        groupBox_12->setTitle(QCoreApplication::translate("MainWindow", "Parents", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">John Banks: aka JCSBanks on </span><a href=\"http://evolutionM.net\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">http://evolutionM.net</span></a><span style=\" font-size:10pt;\"> creator </span></p><p><span style=\" font-size:10pt;\">DMA logging for Mitsu.</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(about), QCoreApplication::translate("MainWindow", "about", nullptr));
        start_addr_lineEdit->setText(QCoreApplication::translate("MainWindow", "ffffa800", nullptr));
        count_lineEdit->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "count", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Start addr", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "direct RAM-ROM reader", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
