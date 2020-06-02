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

#include "gauge_widget.h"

class graph_logger: public QWidget
{
    Q_OBJECT
public:
    graph_logger(QString name, uint DigitNum = 4, uint offset = 0, Scaling *scaling = nullptr, QWidget* parent = nullptr):QWidget(parent), offset(offset), scaling(scaling)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        //resize(parent->size().width() - 15, 10);
        QHBoxLayout *lay = new QHBoxLayout(this);
        setLayout(lay);


        gauge_lcd = new gauge_widget(name, DigitNum, offset, scaling, this);
        layout()->addWidget(gauge_lcd);



        QWidget *qwtplotwidget = new QWidget(this);
        qwtplotwidget->resize(800, 100);
        qwtplotwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        qwtplotwidget->setStyleSheet("background-color:red;");
        layout()->addWidget(qwtplotwidget);

        d_plot = new QwtPlot( qwtplotwidget );

        //d_plot->setMinimumSize(0, 50);
        //d_plot->setMaximumSize(this->size().width() - 15, 50);
        d_plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        d_plot->resize(800, 100);
d_plot->setAutoReplot(true);
//layout()->addWidget(d_plot);
        //d_plot->resize(this->size().width() - 15, 10);

    }
    gauge_widget *gauge_lcd;
    QwtPlot *d_plot;

    uint offset;
    Scaling *scaling;

public slots:
    void display(QString s)
    {
        gauge_lcd->display(s);
    }
};

#endif // GRAPH_LOGGER_H
