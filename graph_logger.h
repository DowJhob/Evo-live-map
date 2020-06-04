#ifndef GRAPH_LOGGER_H
#define GRAPH_LOGGER_H

#include <QHBoxLayout>
#include <QWidget>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_grid.h>

#include <qwt/qwt_legend.h>

#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_symbol.h>

#include <qwt/qwt_plot_magnifier.h>

#include <qwt/qwt_plot_panner.h>

#include <qwt/qwt_plot_picker.h>
#include <qwt/qwt_picker_machine.h>

#include <qwt/qwt_date.h>


#include "gauge_widget.h"

class graph_logger: public QWidget//, public IncrementalPlot
{
    Q_OBJECT
public:
    QList<double> xData;
    QList<double> yData;
    QwtPlotCurve *curve;


    graph_logger(QString name, uint DigitNum = 4, uint offset = 0, Scaling *scaling = nullptr, QWidget* parent = nullptr):QWidget(parent), offset(offset), scaling(scaling)
    {
        QHBoxLayout *lay = new QHBoxLayout(this);

        d_plot = new QwtPlot( this );
        d_plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        d_plot->setAutoReplot(true);

//            d_plot->canvas()->setAttribute( Qt::WA_PaintOnScreen, true );

        // Параметры осей координат
        d_plot->setAxisTitle(QwtPlot::yLeft, name);
        d_plot->setAxisTitle(QwtPlot::xBottom, "time, ms from start");
        // Включить сетку
        QwtPlotGrid *grid = new QwtPlotGrid(); //
        grid->setMajorPen(QPen( Qt::gray, 2 )); // цвет линий и толщина
        grid->attach( d_plot ); // добавить сетку к полю графика
        // Кривая
        curve = new QwtPlotCurve();
        curve->setPen( Qt::black, 2 ); // цвет и толщина кривой
        curve->setRenderHint ( QwtPlotItem::RenderAntialiased, true ); // сглаживание
        curve->attach(d_plot);


        curve->setSamples(xData.toVector(), yData.toVector());

//QwtPlotCanvas *plotCanvas = qobject_cast<QwtPlotCanvas *>( d_plot->canvas() );
//plotCanvas->setPaintAttribute( QwtPlotCanvas::BackingStore, false );

d_plot->setAxisScale(QwtPlot::xBottom, 0, 1);

        lay->addWidget(d_plot);
        setLayout(lay);
    }
    gauge_widget *gauge_lcd;
    QwtPlot *d_plot;
ulong start_time = QDateTime::currentMSecsSinceEpoch();
    uint offset;
    Scaling *scaling;

public slots:
    void display(float in)
    {
ulong t = QDateTime::currentMSecsSinceEpoch();

        xData.append((double) ( t - start_time ));
        yData.append( in );

        if (xData.size() > 1000)
        {
            xData.removeFirst();
            yData.removeFirst();
        }
        curve->setSamples(xData.toVector(), yData.toVector());
d_plot->setAxisScale(QwtPlot::xBottom, t - start_time - 1000, t - start_time);

    }

};

#endif // GRAPH_LOGGER_H
