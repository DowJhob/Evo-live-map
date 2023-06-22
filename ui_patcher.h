/********************************************************************************
** Form generated from reading UI file 'patcher.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATCHER_H
#define UI_PATCHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_Patcher
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *selROMpushButton;
    QTreeWidget *treeWidget;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *selXMLpushButton;
    QPushButton *Undo_pushButton;
    QPushButton *Save_pushButton;
    QPushButton *Apply_pushButton;
    QPushButton *Undo_patch_pushButton;

    void setupUi(QGroupBox *Patcher)
    {
        if (Patcher->objectName().isEmpty())
            Patcher->setObjectName(QString::fromUtf8("Patcher"));
        Patcher->resize(844, 497);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Patcher->sizePolicy().hasHeightForWidth());
        Patcher->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(Patcher);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(7);
        selROMpushButton = new QPushButton(Patcher);
        selROMpushButton->setObjectName(QString::fromUtf8("selROMpushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(selROMpushButton->sizePolicy().hasHeightForWidth());
        selROMpushButton->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(selROMpushButton, 1, 7, 1, 1);

        treeWidget = new QTreeWidget(Patcher);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(treeWidget, 2, 0, 2, 3);

        label_2 = new QLabel(Patcher);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 1, 1, 6);

        label = new QLabel(Patcher);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(Patcher);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        label_4 = new QLabel(Patcher);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 1, 1, 6);

        selXMLpushButton = new QPushButton(Patcher);
        selXMLpushButton->setObjectName(QString::fromUtf8("selXMLpushButton"));

        gridLayout_2->addWidget(selXMLpushButton, 0, 7, 1, 1);

        Undo_pushButton = new QPushButton(Patcher);
        Undo_pushButton->setObjectName(QString::fromUtf8("Undo_pushButton"));
        sizePolicy1.setHeightForWidth(Undo_pushButton->sizePolicy().hasHeightForWidth());
        Undo_pushButton->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(Undo_pushButton, 2, 3, 1, 1);

        Save_pushButton = new QPushButton(Patcher);
        Save_pushButton->setObjectName(QString::fromUtf8("Save_pushButton"));

        gridLayout_2->addWidget(Save_pushButton, 2, 4, 1, 1);

        Apply_pushButton = new QPushButton(Patcher);
        Apply_pushButton->setObjectName(QString::fromUtf8("Apply_pushButton"));

        gridLayout_2->addWidget(Apply_pushButton, 2, 5, 1, 1);

        Undo_patch_pushButton = new QPushButton(Patcher);
        Undo_patch_pushButton->setObjectName(QString::fromUtf8("Undo_patch_pushButton"));

        gridLayout_2->addWidget(Undo_patch_pushButton, 2, 6, 1, 1);


        retranslateUi(Patcher);
        QObject::connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), Patcher, SLOT(itemClicked(QTreeWidgetItem*, int)));
        QObject::connect(selROMpushButton, SIGNAL(clicked()), Patcher, SLOT(selectROMfilename()));
        QObject::connect(selXMLpushButton, SIGNAL(clicked()), Patcher, SLOT(selectXMLfilename()));
        QObject::connect(Undo_pushButton, SIGNAL(clicked()), Patcher, SLOT(Undo()));
        QObject::connect(Save_pushButton, SIGNAL(clicked()), Patcher, SLOT(Save()));
        QObject::connect(Apply_pushButton, SIGNAL(clicked()), Patcher, SLOT(Apply()));
        QObject::connect(Undo_patch_pushButton, SIGNAL(clicked()), Patcher, SLOT(Undo_patch()));

        QMetaObject::connectSlotsByName(Patcher);
    } // setupUi

    void retranslateUi(QGroupBox *Patcher)
    {
        Patcher->setWindowTitle(QCoreApplication::translate("Patcher", "GroupBox", nullptr));
        selROMpushButton->setText(QCoreApplication::translate("Patcher", "Select ROM filename", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("Patcher", "Status", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("Patcher", "Patch name", nullptr));
        label_2->setText(QString());
        label->setText(QCoreApplication::translate("Patcher", "Selected xml definition file:", nullptr));
        label_3->setText(QCoreApplication::translate("Patcher", "\"Selected ROM file:\"", nullptr));
        label_4->setText(QString());
        selXMLpushButton->setText(QCoreApplication::translate("Patcher", "Select xml filename", nullptr));
        Undo_pushButton->setText(QCoreApplication::translate("Patcher", "Undo", nullptr));
        Save_pushButton->setText(QCoreApplication::translate("Patcher", "Save", nullptr));
        Apply_pushButton->setText(QCoreApplication::translate("Patcher", "Apply patch", nullptr));
        Undo_patch_pushButton->setText(QCoreApplication::translate("Patcher", "Undo patch", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Patcher: public Ui_Patcher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATCHER_H
