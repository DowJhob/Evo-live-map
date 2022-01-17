#include "mapmodel.h"

mapModel::mapModel(QObject *parent, Map *decl, QVector<QColor> *colormap) : QAbstractTableModel(parent), declaration(decl), coloringMap(colormap)
{
    horizontalHeaderData = new float[declaration->X_axis.elements];
    verticalHeaderData = new float[declaration->Y_axis.elements];
    int count = declaration->X_axis.elements * declaration->Y_axis.elements;
    mapData2 = new float[count];

    backgroundColorMap = new QBrush[count];
    //qDebug() << "count" << count;
}

mapModel::~mapModel()
{
    delete horizontalHeaderData;
    delete verticalHeaderData;
    delete mapData2;
    delete backgroundColorMap;
}

int mapModel::rowCount(const QModelIndex &parent) const
{
    //qDebug() << "rowCount" << parent.row();
    return declaration->Y_axis.elements;
}

int mapModel::columnCount(const QModelIndex &parent) const
{
    return declaration->X_axis.elements;
}

QVariant mapModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
            return QVariant();
        }


    int offset = index.row() * declaration->X_axis.elements + index.column();
    float value = mapData2[offset];
    switch(role){
    case Qt::DisplayRole :
    {
        return QString().asprintf(declaration->rom_scaling.format.toLatin1(), value);
    } break;
    case Qt::BackgroundRole:
    {
        return backgroundColorMap[offset];
        //return getColor(value, declaration->rom_scaling.min, mapColorDiscret);
    }
        break;
    case Qt::UserRole: return value; break;
    }
    return QVariant();
}

bool mapModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch(role){
    case Qt::EditRole :
        float val = value.toFloat();
        int offset = index.row() * declaration->X_axis.elements + index.column();
        mapData2[offset] = val;
        backgroundColorMap[offset] = getColor(val, declaration->rom_scaling.min, mapColorDiscret);
        c_updateRAM(val, index);
        emit dataChanged(index, index);
        break;
    }
    return true;
}

QVariant mapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    float value = 0;
    Scaling *s = &declaration->X_axis.rom_scaling;
    if( orientation == Qt::Horizontal)
    {
        value = horizontalHeaderData[section];
        s = &declaration->X_axis.rom_scaling;
    }
    if ( orientation == Qt::Vertical)
    {
        value = verticalHeaderData[section];
        s = &declaration->Y_axis.rom_scaling;
    }
    switch(role){
    case Qt::DisplayRole :
    {
        return QString().asprintf(s->format.toLatin1(), value);
    } break;
//    case Qt::BackgroundRole :
//    {
//        float ColorDiscret = 0;
//        if( orientation == Qt::Horizontal)
//            ColorDiscret = xAxisColorDiscret;
//        if( orientation == Qt::Vertical)
//            ColorDiscret = yAxisColorDiscret;
//        auto c = getColor(value, s->min, ColorDiscret);
//        qDebug() << "headerData ColorDiscret" << ColorDiscret << "value" << value << "color" << c;
//        QPalette pal;
//        pal.setBrush(QPalette::Background, c);
//        return c;

//    }
//        break;

    case Qt::UserRole: return value; break;
    }
    return QVariant();
}

bool mapModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if ( orientation == Qt::Horizontal)
            horizontalHeaderData[section] = value.toFloat();
        if ( orientation == Qt::Vertical)
            verticalHeaderData[section] = value.toFloat();
    }
    //emit headerDataChanged(orientation, 0, 1);
    return true;
}

void mapModel::setColorMap(QVector<QColor> *colormap)
{
    this->coloringMap = colormap;
    float a = colormap->count();
    mapColorDiscret = (declaration->rom_scaling.max - declaration->rom_scaling.min) / a;
    xAxisColorDiscret = (declaration->X_axis.rom_scaling.max - declaration->X_axis.rom_scaling.min) / a;
    yAxisColorDiscret = (declaration->Y_axis.rom_scaling.max - declaration->Y_axis.rom_scaling.min) / a;
    //qDebug() << "mapColorDiscret" << mapColorDiscret << "xAxisColorDiscret" << xAxisColorDiscret << "yAxisColorDiscret" << yAxisColorDiscret;
}

QBrush mapModel::getColor(float value, int min, float ColorDiscret) const
{
    int colorIndex = (value - min) / ColorDiscret - 1;
    if (colorIndex < 0)
        colorIndex = 0;
    if (colorIndex > coloringMap->size()-1)
        colorIndex = coloringMap->size()-1;
    //qDebug() << "colorIndex" << colorIndex << min << ColorDiscret << "coloringMap->size()" << coloringMap->count();
    QBrush Background(coloringMap->at(colorIndex));
    return Background;
}
//говнокод, подумай об этом
void mapModel::fillMap(QVector<float> *map)
{
    int count = declaration->X_axis.elements * declaration->Y_axis.elements;
    uint c = 0;
    //qDebug() << "fillMap map.size" << map->size();
    if ( declaration->swapxy )
        for (int y = 0; y < declaration->Y_axis.elements; y++)
            for (int x = 0; x < declaration->X_axis.elements; x++)
            {
                // qDebug() << "fillMap c" << (y * declaration->X_axis.elements + x);
                float value = (*map)[ (x * declaration->Y_axis.elements + y)];
                mapData2[c] = value;
                backgroundColorMap[c] = getColor(value, declaration->rom_scaling.min, mapColorDiscret);
                c++;
            }
    else{
        memcpy(mapData2, map->data(), count * sizeof (float));
        for (int i = 0; i < count; i++)
        {
            backgroundColorMap[c] = getColor(mapData2[i], declaration->rom_scaling.min, mapColorDiscret);
        }
        //qDebug() << "fillMap memcpy";
    }


    ////------------------- сохраняем текущее положение для след расчета -------------------------------------------
    saveTracer();
}

void mapModel::fillHeader(QVector<float> *map, Qt::Orientation orientation)
{
    float *p_header;
    if( orientation == Qt::Horizontal)
        p_header = horizontalHeaderData;
    if ( orientation == Qt::Vertical)
        p_header = verticalHeaderData;
    memcpy(p_header, map->data(), map->size() * sizeof (float));
}

void mapModel::c_updateRAM(float value, const QModelIndex &index)
{
    //mapModel *table = qobject_cast<mapModel*>( sender() );

    int offset;
    if (declaration->swapxy)
    {
        offset = index.column() * declaration->Y_axis.elements + index.row();
    }
    else
    {
        offset = index.row() * declaration->X_axis.elements + index.column();
    }

    abstractMemoryScaled buff(&declaration->rom_scaling, value);
    qDebug()<< "mapModel::c_updateRAM" << index.row() << index.column() << buff.toHex(':');
    //qDebug() <<"hop";
    offset *= declaration->rom_scaling.getElementSize();
    buff.addr = declaration->addr + offset;

    emit updateRAM(buff);
}

void mapModel::logReady(QVector<float> scaledValue)
{
    //qDebug() << "mapModel::logReady scaledValue" << scaledValue;
    //qDebug() << "mapModel::logReady scaled" << scaledValue.size() << declaration->X_axis.ram_mut_number << declaration->Y_axis.ram_mut_number;

    float x = scaledValue[declaration->X_axis.ram_mut_number];
    float y = scaledValue.at(declaration->Y_axis.ram_mut_number);
    //tracer_calc( 100,  2000);
    tracer_calc( x,  y);


}

QBrush mapModel::background(int x, int y)
{
    int offset = y * declaration->X_axis.elements + x;
    return backgroundColorMap[offset];
}

void mapModel::setBackground(int x, int y, QBrush color)
{
    int offset = y * declaration->X_axis.elements + x;
    backgroundColorMap[offset] = color;
}

extern QElapsedTimer t;
void mapModel::tracer_calc(float x, float y)
{
    //qDebug() << "mapModel::tracer_calc: " << QString::number( t.nsecsElapsed()/1000000.0) << declaration->Name;

    // QElapsedTimer t;
    //        t.start();
    //----------------------- вычисляем координаты маркера------------------------------------------------
    current_marker.X = axis_lookup(x, horizontalHeaderData, declaration->X_axis.elements);
    current_marker.Y = axis_lookup(y, verticalHeaderData, declaration->Y_axis.elements);
    //qDebug() << "current_marker.X" << current_marker.X;
    //----------------------- блокируем обновления редакции ----------------------------------------------------------
//    blockSignals( true );
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
//    blockSignals(false);//

    //qDebug() << "current_marker" << current_marker.X << current_marker.Y << current_marker.X+1 << current_marker.Y+1;
    auto i = index(current_marker.Y, current_marker.X);

    auto j = index(current_marker.Y+1, current_marker.X+1);
//qDebug() << "mapModel::tracer_calc indexes" << i.flags() << j;
    emit dataChanged(i, j, QVector<int>{Qt::BackgroundRole});

}

void mapModel::saveTracer()
{//сохраняем  текущее положение для след расчета
    //    QElapsedTimer t;
    //    t.start();
    int c = 0;
    pred_marker.X = current_marker.X;
    pred_marker.Y = current_marker.Y;
    for(int Yi = 0; Yi < 2; ++Yi)
        for(int Xj = 0; Xj < 2; ++Xj)
        {
            pred_marker.color[c++] = background(current_marker.X + Xj, current_marker.Y + Yi).color();
        }
    //    qDebug() << "saveTracer: " << QString::number( t.nsecsElapsed());
}

void mapModel::renderTracer(Tracer_marker2 *marker)
{
    //    QElapsedTimer t;
    //    t.start();
    //         рисуем новое положение маркера
    int c = 0;
    for( int Yi = 0; Yi < 2; Yi++)
        for( int Xj = 0; Xj < 2; Xj++)
        {
            //                QTableWidgetItem *_item = item( marker->Y + Yi, marker->X + Xj );
            //                if( _item != nullptr )
            setBackground(marker->X + Xj, marker->Y + Yi, QBrush(marker->color[ c++ ]));
        }
    //   qDebug() << "render: " << QString::number( t.nsecsElapsed());
}

void mapModel::getSaturation(float x, float y)
{
    //   QElapsedTimer t;
    //   t.start();
    float minX = horizontalHeaderData[current_marker.X];
    float minY = verticalHeaderData[current_marker.Y];
    float maxX = minX+1;                // Что бы не было деления на ноль
    float maxY = minY+1;

    if( (current_marker.X + 1) < declaration->X_axis.elements )
        maxX = horizontalHeaderData[current_marker.X + 1];
    if( (current_marker.Y + 1) < declaration->Y_axis.elements )
        maxY = verticalHeaderData[current_marker.Y + 1];

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

int mapModel::modul(int a, int b)
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

int mapModel::axis_lookup(float in, float *axis, int size)    //возвращает меньший индекс
{
    int axis_lenght = size;
    if (axis_lenght <= 1 )
        return 0;
    if (in < axis[0])
        return 0;
    if (in > axis[axis_lenght - 1])
        return axis_lenght - 1;
    for (int i = 0; i < axis_lenght-1; i++)
        if (  in >= axis[i] && in < axis[i+1])
            return i;
    return axis_lenght - 1;
}

//Qt::ItemFlags mapModel::flags(const QModelIndex &) const
//{
//    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
//}
