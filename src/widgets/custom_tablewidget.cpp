#include <QElapsedTimer>
#include "custom_tablewidget.h"

CustomTableWidget::CustomTableWidget(Map *declaration) : declaration(declaration)
{
    setSortingEnabled(false);
    //setUpdatesEnabled(false);
    setUpdatesEnabled(true);
    ////------------------- сохраняем текущее положение для след расчета -------------------------------------------
    //saveTracer();

    setRowCount(declaration->Y_axis.elements);
    setColumnCount(declaration->X_axis.elements);
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

    createParentWdgt();
}

void CustomTableWidget::fillTable(QVector<float> *map)
{
    fillHeaders();
    fillMap(map);
    //setHorizontalHeaderLabels("fghshshf");
    resizeColumnsToContents();      // Не нужно потому что в конструкторе ресайзится под хидер, там числа больше и следовательно колонки шире
    resizeRowsToContents();           // А в высоту они все одинаковые
    fixSize();
    //        hide();
    //        show();
}

void CustomTableWidget::traceUpdate()
{
    //qDebug() << "scaledRAM_MUTvalue: "<<scaledRAM_MUTvalue;
//    float x = 19;
//    float y = 750;
    if (x >100 || x <1)
        test = !test;
    if (test)
    {
        x += 5;
        y += 70;
    }
    else
    {
        x -= 5;
        y -= 70;
    }

 //           if (debug)
            {
                    x = QCursor::pos().x()
                            /7.3
                            //*2.3
                            ;
                    y = QCursor::pos().y()
                           *7.3
                            ;
            }
    //qDebug() << "update: " << declaration.X_axis.ram_mut_number << declaration.Y_axis.ram_mut_number;
    //   if ( declaration.X_axis.ram_mut_number >= 0  //&& !debug
    //         )
    //    {       x = scaledRAM_MUTvalue->value(declaration.X_axis.ram_mut_number);}
    //    if ( declaration.Y_axis.ram_mut_number >= 0 //&& !debug
    //         )
    //    {   y = scaledRAM_MUTvalue->value(declaration.Y_axis.ram_mut_number);}
    //if ( declaration.X_axis.ram_mut_number >= 0 && declaration.Y_axis.ram_mut_number >= 0 )
    tracer_calc(x, y);
    //delete &scaledRAM_MUTvalue;
}

void CustomTableWidget::fillMap(QVector<float> *map)
{
    uint c = 0;
    int swapXyLen, swapYxLen;
    if ( declaration->swapxy )
    {
        swapXyLen = declaration->X_axis.elements;
        swapYxLen = declaration->Y_axis.elements;
    }
    else
    {
        swapYxLen = declaration->X_axis.elements;
        swapXyLen = declaration->Y_axis.elements;
    }
    int swapxyX, swapxyY;
    QColor color;
    int britghtnes = 200;
    int minDesireHUE = 0;
    int maxDesireHUE = 240;
    int desire_hue;

    // normalize input
    float inRange = declaration->rom_scaling.max - declaration->rom_scaling.min;
    int kHue = maxDesireHUE - minDesireHUE;

    for (int x = 0; x < swapXyLen; x++)
    {
        for (int y = 0; y < swapYxLen; y++)
        {
            //создаем обновляем итем
            if ( this->declaration->swapxy ) {swapxyX = x; swapxyY = y;}
            else {swapxyX = y; swapxyY = x;}
            if (item(swapxyY, swapxyX) == nullptr)  //если итема нет создадим
                setItem(swapxyY, swapxyX, new QTableWidgetItem(Qt::DisplayRole));

            // normalize input
            float kColor = fabs(  (map->at(c) - declaration->rom_scaling.min) / inRange);
            britghtnes = 200;
            if ( map->at(c) > declaration->rom_scaling.max)
            {
                britghtnes = 80;
                kColor = 1;
            }
            else if ( map->at(c) < declaration->rom_scaling.min )
            {
                britghtnes = 80;
                kColor = 0;
            }

            {
                // normalize hue
                desire_hue = qRound(kHue * kColor);
                desire_hue += minDesireHUE;
            }
            color.setHsv( desire_hue , saturation, britghtnes, alpha);
            item(swapxyY, swapxyX)->setBackground(color);
            item(swapxyY, swapxyX)->setData(Qt::DisplayRole, //map->at(c)
                                            QString().asprintf(declaration->rom_scaling.format.toLatin1(), (*map)[c]));
            c++;
        }
    }
    //init marker
    saveTracer();
    getSaturation(0, 0);
    renderTracer(&current_marker);
}

void CustomTableWidget::keyPressEvent(QKeyEvent *event)
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

void CustomTableWidget::fillHeaders()
{
    for (int i = 0; i < declaration->X_axis.elements; i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData( Qt::DisplayRole, QString::number(x_axis.at(i)));
        setHorizontalHeaderItem(i, item);
    }
    //----------------------------------------------------------------------------------------------
    for (int i = 0; i < declaration->Y_axis.elements; i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData( Qt::DisplayRole, QString::number(y_axis.at(i)));
        setVerticalHeaderItem(i, item);
    }
}

void CustomTableWidget::saveTracer()
{//сохраняем  текущее положение для след расчета
//    QElapsedTimer t;
//    t.start();
    int c = 0;
    pred_marker.X = current_marker.X;
    pred_marker.Y = current_marker.Y;
    for(int Yi = 0; Yi < 2; ++Yi)
        for(int Xj = 0; Xj < 2; ++Xj)
        {
            QTableWidgetItem *_item = item( current_marker.Y + Yi, current_marker.X + Xj);
            if( _item != nullptr)
                pred_marker.color[c++] = _item->background().color();
        }
//    qDebug() << "saveTracer: " << QString::number( t.nsecsElapsed());
}

void CustomTableWidget::renderTracer(Tracer_marker *marker)
{
//    QElapsedTimer t;
//    t.start();
    //         рисуем новое положение маркера
    int c = 0;
    for( int Yi = 0; Yi < 2; Yi++)
        for( int Xj = 0; Xj < 2; Xj++)
        {
            QTableWidgetItem *_item = item( marker->Y + Yi, marker->X + Xj );
            if( _item != nullptr )
                _item->setBackground( marker->color[ c++ ]);
        }
//   qDebug() << "render: " << QString::number( t.nsecsElapsed());
}

void CustomTableWidget::getSaturation(float x, float y)
{
//   QElapsedTimer t;
//   t.start();
    float minX = x_axis[current_marker.X];
    float minY = y_axis[current_marker.Y];
    float maxX = minX+1;                // Что бы не было деления на ноль
    float maxY = minY+1;

    if( (current_marker.X + 1) < x_axis.size() )
        maxX = x_axis[current_marker.X + 1];
    if( (current_marker.Y + 1) < y_axis.size() )
        maxY = y_axis[current_marker.Y + 1];

    //коэффициент нормирования
    float kX = 255.0/(maxX - minX);
    float kY = 255.0/(maxY - minY);
    //нормированные координаты
    int rightNormalX  = (maxX-x)*kX;
    int leftNormalX = 255 - rightNormalX;
    int downNormalY    = (maxY-y)*kY;
    int upNormalY  = 255 - downNormalY;
    //модули векторов,
    int leftUP    = modul(leftNormalX, upNormalY);
    int rightUP   = modul(rightNormalX, upNormalY);
    int leftDOWN  = modul(leftNormalX, downNormalY);
    int rightDOWN = modul(rightNormalX, downNormalY);

    current_marker.leftUpColor->setHsv(240, leftUP, 255, 255);
    current_marker.rightUpColor->setHsv(240, rightUP, 255, 255);
    current_marker.leftDownColor->setHsv(240, leftDOWN, 255, 255);
    current_marker.rightDownColor->setHsv(240, rightDOWN, 255, 255);

//    qDebug() << "getSaturation: " << QString::number( t.nsecsElapsed());
}

void CustomTableWidget::tracer_calc(float x, float y)
{    /*QElapsedTimer t;
     t.start();*/
    //qDebug() << "declaration: " << declaration.map.Name;
    //----------------------- вычисляем координаты маркера------------------------------------------------
    current_marker.X = axis_lookup(x, &x_axis);
    current_marker.Y = axis_lookup(y, &y_axis);
    //----------------------- блокируем обновления редакции ----------------------------------------------------------
    blockSignals( true );
    // Если координаты маркера не менялись то нет нужды сохранять основные цвета и перерисовывать предыдущий
    // достаточно просто пересчитать насыщенность
    if ((pred_marker.X != current_marker.X) ||(pred_marker.Y != current_marker.Y))
    {
        //        bool save_trace = false;
        //        if (!save_trace)
        //
        //------------------- гашение предыдущего маркера таблицы ----------------------------------------------------
        renderTracer(&pred_marker);
        //------------------- сохраняем текущее положение для след расчета -------------------------------------------
        saveTracer();
    }
    //------------------- вычисляем насыщенность ячеек маркера ---------------------------------------------------
    getSaturation(x, y);
    //------------------- рисуем новое положение маркера ---------------------------------------------------------
    renderTracer(&current_marker);
    //----------------------- разблокируем обновления редакции -------------------------------------------------------
    //setUpdatesEnabled(true);
    //update(pred_marker.X,pred_marker.Y,1,1);
    //update(current_marker.X,current_marker.Y,1,1);
    //setUpdatesEnabled(false);
    //
    blockSignals(false);//

//    qDebug() << "tracer_calc: " << QString::number( t.nsecsElapsed());
}

int CustomTableWidget::modul(int a, int b)
{
    int mod = //qRound(
            sqrt(pow(a, 2) + pow(b, 2))//)
            ;
    if (mod > 255)
        mod = 255;

    mod = (255 - mod);
    if (mod < 30)
        mod = 30;
    return mod;
}

int CustomTableWidget::axis_lookup(float in, QVector<float> *axis)    //возвращает меньший индекс
{
    int axis_lenght = axis->size();
    if (axis_lenght <= 1 )
        return 0;
    if (in < (*axis)[0])
        return 0;
    if (in > (*axis)[axis_lenght - 1])
        return axis_lenght - 1;
    for (int i = 0; i < axis_lenght-1; i++)
        if (  in >= (*axis)[i] && in < (*axis)[i+1])
            return i;
    return axis_lenght - 1;
}

void CustomTableWidget::createParentWdgt()
{
    QWidget *mapWidget = new QWidget(nullptr, Qt::Window | Qt::WindowCloseButtonHint );
    mapWidget->setWindowTitle(declaration->Name);
    setParent(mapWidget);

    //mapWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //mapWidget->setFixedSize( 1,1 );
    mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout *_layout = new QGridLayout(mapWidget);
    _layout->setSizeConstraint(QLayout::SetFixedSize);

    _layout->addWidget(this);
    mapWidget->setLayout(_layout);
}

void CustomTableWidget::fixSize()
{
    //QSize Size = size();
    //Size.setWidth( Size.width() + 150);
    //Size.setHeight( Size.height() + 150);

    //        Size.setWidth( horizontalHeader()->length());
    //        Size.setHeight( verticalHeader()->length());

    //setFixedSize( Size );
    //parentWidget()->resize(Size);
    //parentWidget()->setFixedSize( Size );

}

void CustomTableWidget::actionCopy(){
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

void CustomTableWidget::actionPaste(){
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

void CustomTableWidget::actionIncrease()
{
    QItemSelectionRange range = selectionModel()->selection().first();
    for (auto i = range.top(); i <= range.bottom(); ++i)
        for (auto j = range.left(); j <= range.right(); ++j)
            model()->setData( model()->index(i,j), model()->index(i,j).data().toFloat() + declaration->rom_scaling.increment, Qt::DisplayRole );
}

void CustomTableWidget::actionDecrease()
{
    QItemSelectionRange range = selectionModel()->selection().first();
    for (auto i = range.top(); i <= range.bottom(); ++i)
        for (auto j = range.left(); j <= range.right(); ++j)
            model()->setData( model()->index(i,j), model()->index(i,j).data().toFloat() - declaration->rom_scaling.increment, Qt::DisplayRole );
}
