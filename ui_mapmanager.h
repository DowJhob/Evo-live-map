/********************************************************************************
** Form generated from reading UI file 'mapmanager.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPMANAGER_H
#define UI_MAPMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_mapManager
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QGroupBox *groupBox;

    void setupUi(QGroupBox *mapManager)
    {
        if (mapManager->objectName().isEmpty())
            mapManager->setObjectName(QString::fromUtf8("mapManager"));
        mapManager->resize(624, 705);
        horizontalLayout = new QHBoxLayout(mapManager);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeWidget = new QTreeWidget(mapManager);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        horizontalLayout->addWidget(treeWidget);

        groupBox = new QGroupBox(mapManager);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(groupBox);


        retranslateUi(mapManager);

        QMetaObject::connectSlotsByName(mapManager);
    } // setupUi

    void retranslateUi(QGroupBox *mapManager)
    {
        mapManager->setWindowTitle(QCoreApplication::translate("mapManager", "GroupBox", nullptr));
        groupBox->setTitle(QCoreApplication::translate("mapManager", "GroupBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mapManager: public Ui_mapManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPMANAGER_H
