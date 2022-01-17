#include "mapview.h"

//#include <QAbstractItemView>

mapView::mapView(Map *declaration):declaration(declaration)
{
    setEditTriggers(QAbstractItemView::SelectedClicked|QAbstractItemView::EditKeyPressed);

    setSortingEnabled(false);
    //setUpdatesEnabled(false);
    setUpdatesEnabled(true);
    ////------------------- сохраняем текущее положение для след расчета -------------------------------------------
    //saveTracer();

    //setRowCount(declaration->Y_axis.elements);
    //setColumnCount(declaration->X_axis.elements);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    horizontalHeader()->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    verticalHeader()->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    ////setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ////setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
extern QElapsedTimer t;
void mapView::logReady(QVector<float> scaledValue)
{
    //setUpdatesEnabled(false);
    //qDebug() << "mapView::tracer_calc: " << QString::number( t.nsecsElapsed()/1000000.0) << declaration->Name;
    //{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    mapModel *m = static_cast<mapModel*>(model());
    m->logReady(scaledValue);

    //repaint();
    //setUpdatesEnabled(true);
}

void mapView::keyPressEvent(QKeyEvent *event)
{
    if( event==QKeySequence::ZoomIn || event->matches(QKeySequence::MoveToNextPage) )
        actionIncrease();
    else if( event->matches( QKeySequence::ZoomOut) || event->matches(QKeySequence::MoveToPreviousPage) )
        actionDecrease();
    else if( event->matches( QKeySequence::Copy ) )
        actionCopy();
    else if( event->matches( QKeySequence::Paste ) )
        actionPaste();
}

void mapView::actionCopy(){
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

void mapView::actionPaste(){
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

void mapView::actionIncrease()
{
    auto sIx= selectedIndexes() ;
    for (auto index : qAsConst(sIx))
    {
        auto value =  model()->index(index.row(),index.column()).data(Qt::UserRole).toFloat()
                + declaration->rom_scaling.increment
                ;
        qDebug()<< "value"<<value;
        model()->setData( index, value , Qt::EditRole );
        //update(index);
    }
}

void mapView::actionDecrease()
{
    auto sIx= selectedIndexes() ;
    for (auto index : qAsConst(sIx))
    {
        auto value =  model()->index(index.row(),index.column()).data(Qt::UserRole).toFloat()
                - declaration->rom_scaling.increment
                ;
        qDebug()<< "value"<<value;
        model()->setData( index, value , Qt::EditRole );
        //update(index);
    }
}
