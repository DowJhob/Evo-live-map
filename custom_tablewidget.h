#ifndef CUSTOMTABLEWIIDGET_H
#define CUSTOMTABLEWIIDGET_H

#include <QTableWidget>
#include <QObject>
#include <QAction>
#include <QApplication>
#include <QClipboard>

#include "ecu.h"

class CustomTableWidget: public QTableWidget
{
    Q_OBJECT
public:
    CustomTableWidget(int row, int column, QWidget *parent = nullptr//, tableDeclaration *Table_Decl = nullptr
                      ): QTableWidget(parent)
    {
        setRowCount(row);
        setColumnCount(column);
        setContextMenuPolicy(Qt::ActionsContextMenu);
    }
protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if( event==QKeySequence::ZoomIn )
            slotIncrease();
        else if( event->matches( QKeySequence::ZoomOut ) )
            slotDecrease();
        else if( event->matches( QKeySequence::Copy ) )
            slotCopy();
        else if( event->matches( QKeySequence::Paste ) )
            slotPaste();
    }
private:

private slots:
    void slotCopy(){
        QString text;
        QItemSelectionRange range = selectionModel()->selection().first();
        for (auto i = range.top(); i <= range.bottom(); ++i)
        {
            QStringList rowContents;
            for (auto j = range.left(); j <= range.right(); ++j)
                rowContents << model()->index(i,j).data().toString();
            text += rowContents.join("\t");
            text += "\n";
        }
        QApplication::clipboard()->setText(text);
    }
    void slotPaste(){
        QString text = QApplication::clipboard()->text();
        QStringList rowContents = text.split("\n", QString::SkipEmptyParts);

        QModelIndex initIndex = selectionModel()->selectedIndexes().at(0);
        auto initRow = initIndex.row();
        auto initCol = initIndex.column();

        for (auto i = 0; i < rowContents.size(); ++i) {
            QStringList columnContents = rowContents.at(i).split("\t");
            for (auto j = 0; j < columnContents.size(); ++j) {
                model()->setData(model()->index(initRow + i, initCol + j), columnContents.at(j));
            }
        }
    }
    void slotIncrease()
    {

    }
    void slotDecrease()
    {

    }

};

#endif // CUSTOMTABLEWIIDGET_H
