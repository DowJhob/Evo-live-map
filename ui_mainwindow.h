/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_18;
    QVBoxLayout *verticalLayout_5;
    QPushButton *read_RAM_Button;
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1034, 596);
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
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(0, 0));
        centralWidget->setMaximumSize(QSize(10000, 10000));
        centralWidget->setBaseSize(QSize(1000, 800));
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(20);
        horizontalLayoutWidget = new QWidget(splitter);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(horizontalLayoutWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("ROM metadata"));
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignCenter);
        __qtreewidgetitem->setBackground(0, QColor(120, 120, 120));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
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
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(horizontalLayoutWidget_2);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setElideMode(Qt::ElideRight);
        live_tab = new QWidget();
        live_tab->setObjectName(QStringLiteral("live_tab"));
        sizePolicy.setHeightForWidth(live_tab->sizePolicy().hasHeightForWidth());
        live_tab->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(live_tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter_2 = new QSplitter(live_tab);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setFrameShape(QFrame::WinPanel);
        splitter_2->setFrameShadow(QFrame::Sunken);
        splitter_2->setMidLineWidth(5);
        splitter_2->setOrientation(Qt::Horizontal);
        verticalLayoutWidget_5 = new QWidget(splitter_2);
        verticalLayoutWidget_5->setObjectName(QStringLiteral("verticalLayoutWidget_5"));
        gridLayout_17 = new QGridLayout(verticalLayoutWidget_5);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QStringLiteral("gridLayout_17"));
        gridLayout_17->setContentsMargins(0, 0, 0, 0);
        groupBox_8 = new QGroupBox(verticalLayoutWidget_5);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_8->sizePolicy().hasHeightForWidth());
        groupBox_8->setSizePolicy(sizePolicy2);
        gridLayout_19 = new QGridLayout(groupBox_8);
        gridLayout_19->setSpacing(6);
        gridLayout_19->setContentsMargins(11, 11, 11, 11);
        gridLayout_19->setObjectName(QStringLiteral("gridLayout_19"));
        gridLayout_20 = new QGridLayout();
        gridLayout_20->setSpacing(6);
        gridLayout_20->setObjectName(QStringLiteral("gridLayout_20"));
        BaudRatelabel_7 = new QLabel(groupBox_8);
        BaudRatelabel_7->setObjectName(QStringLiteral("BaudRatelabel_7"));
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
        BaudRatelineEdit->setObjectName(QStringLiteral("BaudRatelineEdit"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(BaudRatelineEdit->sizePolicy().hasHeightForWidth());
        BaudRatelineEdit->setSizePolicy(sizePolicy4);
        BaudRatelineEdit->setMaximumSize(QSize(50, 16777215));
        BaudRatelineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_20->addWidget(BaudRatelineEdit, 3, 1, 1, 1);

        logger_rate_textedit = new QLineEdit(groupBox_8);
        logger_rate_textedit->setObjectName(QStringLiteral("logger_rate_textedit"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(logger_rate_textedit->sizePolicy().hasHeightForWidth());
        logger_rate_textedit->setSizePolicy(sizePolicy5);
        logger_rate_textedit->setMaximumSize(QSize(50, 16777215));
        logger_rate_textedit->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        gridLayout_20->addWidget(logger_rate_textedit, 1, 1, 1, 1);

        BaudRatelabel_8 = new QLabel(groupBox_8);
        BaudRatelabel_8->setObjectName(QStringLiteral("BaudRatelabel_8"));
        sizePolicy3.setHeightForWidth(BaudRatelabel_8->sizePolicy().hasHeightForWidth());
        BaudRatelabel_8->setSizePolicy(sizePolicy3);
        BaudRatelabel_8->setFrameShadow(QFrame::Raised);

        gridLayout_20->addWidget(BaudRatelabel_8, 1, 2, 1, 1);

        BaudRatelabel_9 = new QLabel(groupBox_8);
        BaudRatelabel_9->setObjectName(QStringLiteral("BaudRatelabel_9"));
        sizePolicy3.setHeightForWidth(BaudRatelabel_9->sizePolicy().hasHeightForWidth());
        BaudRatelabel_9->setSizePolicy(sizePolicy3);
        BaudRatelabel_9->setLayoutDirection(Qt::LeftToRight);
        BaudRatelabel_9->setFrameShadow(QFrame::Sunken);

        gridLayout_20->addWidget(BaudRatelabel_9, 1, 0, 1, 1);

        line_4 = new QFrame(groupBox_8);
        line_4->setObjectName(QStringLiteral("line_4"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(line_4->sizePolicy().hasHeightForWidth());
        line_4->setSizePolicy(sizePolicy6);
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_20->addWidget(line_4, 2, 0, 1, 4);

        radioButton_7 = new QRadioButton(groupBox_8);
        radioButton_7->setObjectName(QStringLiteral("radioButton_7"));
        sizePolicy3.setHeightForWidth(radioButton_7->sizePolicy().hasHeightForWidth());
        radioButton_7->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(radioButton_7, 5, 2, 1, 1);

        radioButton_8 = new QRadioButton(groupBox_8);
        radioButton_8->setObjectName(QStringLiteral("radioButton_8"));
        sizePolicy3.setHeightForWidth(radioButton_8->sizePolicy().hasHeightForWidth());
        radioButton_8->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(radioButton_8, 4, 2, 1, 1);

        radioButton_9 = new QRadioButton(groupBox_8);
        radioButton_9->setObjectName(QStringLiteral("radioButton_9"));
        sizePolicy3.setHeightForWidth(radioButton_9->sizePolicy().hasHeightForWidth());
        radioButton_9->setSizePolicy(sizePolicy3);
        radioButton_9->setChecked(true);

        gridLayout_20->addWidget(radioButton_9, 3, 2, 1, 1);

        label_3 = new QLabel(groupBox_8);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);

        gridLayout_20->addWidget(label_3, 3, 3, 1, 1);

        trace_time_label = new QLabel(groupBox_8);
        trace_time_label->setObjectName(QStringLiteral("trace_time_label"));

        gridLayout_20->addWidget(trace_time_label, 4, 1, 1, 1);

        label_7 = new QLabel(groupBox_8);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_20->addWidget(label_7, 4, 0, 1, 1);


        gridLayout_19->addLayout(gridLayout_20, 1, 0, 1, 2);


        gridLayout_17->addWidget(groupBox_8, 0, 0, 1, 1);

        groupBox_7 = new QGroupBox(verticalLayoutWidget_5);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(groupBox_7->sizePolicy().hasHeightForWidth());
        groupBox_7->setSizePolicy(sizePolicy7);
        gridLayout_18 = new QGridLayout(groupBox_7);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        read_RAM_Button = new QPushButton(groupBox_7);
        read_RAM_Button->setObjectName(QStringLiteral("read_RAM_Button"));
        read_RAM_Button->setEnabled(true);
        sizePolicy7.setHeightForWidth(read_RAM_Button->sizePolicy().hasHeightForWidth());
        read_RAM_Button->setSizePolicy(sizePolicy7);
        read_RAM_Button->setCheckable(false);

        verticalLayout_5->addWidget(read_RAM_Button);

        save_trace_pushButton = new QPushButton(groupBox_7);
        save_trace_pushButton->setObjectName(QStringLiteral("save_trace_pushButton"));
        sizePolicy7.setHeightForWidth(save_trace_pushButton->sizePolicy().hasHeightForWidth());
        save_trace_pushButton->setSizePolicy(sizePolicy7);

        verticalLayout_5->addWidget(save_trace_pushButton);

        stop_live = new QPushButton(groupBox_7);
        stop_live->setObjectName(QStringLiteral("stop_live"));
        sizePolicy7.setHeightForWidth(stop_live->sizePolicy().hasHeightForWidth());
        stop_live->setSizePolicy(sizePolicy7);

        verticalLayout_5->addWidget(stop_live);


        gridLayout_18->addLayout(verticalLayout_5, 0, 0, 1, 1);


        gridLayout_17->addWidget(groupBox_7, 1, 0, 1, 1);

        splitter_2->addWidget(verticalLayoutWidget_5);
        verticalLayoutWidget_6 = new QWidget(splitter_2);
        verticalLayoutWidget_6->setObjectName(QStringLiteral("verticalLayoutWidget_6"));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(verticalLayoutWidget_6);
        listWidget->setObjectName(QStringLiteral("listWidget"));
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
        patcher->setObjectName(QStringLiteral("patcher"));
        gridLayout_3 = new QGridLayout(patcher);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        groupBox_2 = new QGroupBox(patcher);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        listView = new QListView(groupBox_2);
        listView->setObjectName(QStringLiteral("listView"));

        gridLayout_4->addWidget(listView, 1, 1, 1, 2);

        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_6->addWidget(label, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_3, 0, 1, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_7 = new QGridLayout(groupBox_4);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_7->addWidget(label_2, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_4, 0, 2, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 3, 1, 1);

        groupBox = new QGroupBox(patcher);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        create = new QPushButton(groupBox);
        create->setObjectName(QStringLiteral("create"));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(create->sizePolicy().hasHeightForWidth());
        create->setSizePolicy(sizePolicy8);

        verticalLayout->addWidget(create);

        delete_2 = new QPushButton(groupBox);
        delete_2->setObjectName(QStringLiteral("delete_2"));

        verticalLayout->addWidget(delete_2);

        loadbinbutton = new QPushButton(groupBox);
        loadbinbutton->setObjectName(QStringLiteral("loadbinbutton"));

        verticalLayout->addWidget(loadbinbutton);

        savebinbutton = new QPushButton(groupBox);
        savebinbutton->setObjectName(QStringLiteral("savebinbutton"));

        verticalLayout->addWidget(savebinbutton);

        loadpatchbutton = new QPushButton(groupBox);
        loadpatchbutton->setObjectName(QStringLiteral("loadpatchbutton"));

        verticalLayout->addWidget(loadpatchbutton);

        savepatchbutton = new QPushButton(groupBox);
        savepatchbutton->setObjectName(QStringLiteral("savepatchbutton"));

        verticalLayout->addWidget(savepatchbutton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout_3->addWidget(groupBox, 0, 1, 1, 1);

        groupBox_5 = new QGroupBox(patcher);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        sizePolicy.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy);
        gridLayout_8 = new QGridLayout(groupBox_5);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        listView_2 = new QListView(groupBox_5);
        listView_2->setObjectName(QStringLiteral("listView_2"));

        gridLayout_8->addWidget(listView_2, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_5, 0, 2, 1, 1);

        RAWfileVIEWplainTextEdit = new QPlainTextEdit(patcher);
        RAWfileVIEWplainTextEdit->setObjectName(QStringLiteral("RAWfileVIEWplainTextEdit"));
        QSizePolicy sizePolicy9(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(RAWfileVIEWplainTextEdit->sizePolicy().hasHeightForWidth());
        RAWfileVIEWplainTextEdit->setSizePolicy(sizePolicy9);

        gridLayout_3->addWidget(RAWfileVIEWplainTextEdit, 1, 1, 1, 3);

        tabWidget->addTab(patcher, QString());
        about = new QWidget();
        about->setObjectName(QStringLiteral("about"));
        gridLayout = new QGridLayout(about);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setSpacing(6);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        groupBox_11 = new QGroupBox(about);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        gridLayout_10 = new QGridLayout(groupBox_11);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        label_4 = new QLabel(groupBox_11);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy10(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy10.setHorizontalStretch(93);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy10);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_10->addWidget(label_4, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_11, 0, 0, 1, 1);

        groupBox_12 = new QGroupBox(about);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        gridLayout_11 = new QGridLayout(groupBox_12);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        label_5 = new QLabel(groupBox_12);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_11->addWidget(label_5, 0, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_12, 0, 1, 1, 1);


        gridLayout->addLayout(gridLayout_9, 0, 0, 1, 1);

        tabWidget->addTab(about, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_12 = new QGridLayout(tab);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        RAMeditorLayout = new QGridLayout();
        RAMeditorLayout->setSpacing(6);
        RAMeditorLayout->setObjectName(QStringLiteral("RAMeditorLayout"));
        start_addr_lineEdit = new QLineEdit(tab);
        start_addr_lineEdit->setObjectName(QStringLiteral("start_addr_lineEdit"));
        QFont font;
        font.setPointSize(14);
        start_addr_lineEdit->setFont(font);

        RAMeditorLayout->addWidget(start_addr_lineEdit, 2, 0, 1, 1);

        count_lineEdit = new QLineEdit(tab);
        count_lineEdit->setObjectName(QStringLiteral("count_lineEdit"));
        count_lineEdit->setFont(font);

        RAMeditorLayout->addWidget(count_lineEdit, 2, 1, 1, 1);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy8.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy8);

        RAMeditorLayout->addWidget(label_6, 0, 1, 1, 1);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy8.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy8);

        RAMeditorLayout->addWidget(label_8, 0, 0, 1, 1);


        gridLayout_12->addLayout(RAMeditorLayout, 2, 0, 1, 1);

        tabWidget->addTab(tab, QString());

        horizontalLayout_3->addWidget(tabWidget);

        splitter->addWidget(horizontalLayoutWidget_2);

        gridLayout_5->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        sizePolicy2.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy2);
        statusBar->setMinimumSize(QSize(0, 20));
        statusBar->setFocusPolicy(Qt::ClickFocus);
        statusBar->setAutoFillBackground(false);
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setIconSize(QSize(128, 128));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "eulle@ya.ru     v 0.1", 0));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Connection parameter", 0));
        BaudRatelabel_7->setText(QApplication::translate("MainWindow", "Baud rate:", 0));
        BaudRatelineEdit->setText(QApplication::translate("MainWindow", "15625", 0));
        logger_rate_textedit->setText(QApplication::translate("MainWindow", "100", 0));
        logger_rate_textedit->setPlaceholderText(QString());
        BaudRatelabel_8->setText(QApplication::translate("MainWindow", "Hz", 0));
        BaudRatelabel_9->setText(QApplication::translate("MainWindow", "Logging rate:", 0));
        radioButton_7->setText(QApplication::translate("MainWindow", "115200", 0));
        radioButton_8->setText(QApplication::translate("MainWindow", "62200", 0));
        radioButton_9->setText(QApplication::translate("MainWindow", "15625", 0));
        label_3->setText(QApplication::translate("MainWindow", "baud", 0));
        trace_time_label->setText(QApplication::translate("MainWindow", "0", 0));
        label_7->setText(QApplication::translate("MainWindow", "redraw time all tables", 0));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Control section", 0));
        read_RAM_Button->setText(QApplication::translate("MainWindow", "read RAM", 0));
        save_trace_pushButton->setText(QApplication::translate("MainWindow", "save_trace", 0));
        stop_live->setText(QApplication::translate("MainWindow", "Stop for debug", 0));
        tabWidget->setTabText(tabWidget->indexOf(live_tab), QApplication::translate("MainWindow", "live", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "edit", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Address", 0));
        label->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Lenght", 0));
        label_2->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "control", 0));
        create->setText(QApplication::translate("MainWindow", "Create", 0));
        delete_2->setText(QApplication::translate("MainWindow", "Delete", 0));
        loadbinbutton->setText(QApplication::translate("MainWindow", "Load .bin", 0));
        savebinbutton->setText(QApplication::translate("MainWindow", "Save .bin", 0));
        loadpatchbutton->setText(QApplication::translate("MainWindow", "Load .patch", 0));
        savepatchbutton->setText(QApplication::translate("MainWindow", "Save .patch", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "patch list", 0));
        tabWidget->setTabText(tabWidget->indexOf(patcher), QApplication::translate("MainWindow", "patcher", 0));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "HowTo", 0));
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
                        "ody></html>", 0));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "Parents", 0));
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">John Banks: aka JCSBanks on </span><a href=\"http://evolutionM.net\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">http://evolutionM.net</span></a><span style=\" font-size:10pt;\"> creator </span></p><p><span style=\" font-size:10pt;\">DMA logging for Mitsu.</span></p></body></html>", 0));
        tabWidget->setTabText(tabWidget->indexOf(about), QApplication::translate("MainWindow", "about", 0));
        start_addr_lineEdit->setText(QApplication::translate("MainWindow", "ffffa800", 0));
        count_lineEdit->setText(QApplication::translate("MainWindow", "100", 0));
        label_6->setText(QApplication::translate("MainWindow", "count", 0));
        label_8->setText(QApplication::translate("MainWindow", "Start addr", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "direct RAM-ROM reader", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
