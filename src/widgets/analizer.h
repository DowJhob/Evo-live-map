#ifndef ANALIZER_H
#define ANALIZER_H

#include <QFrame>
#include <QWidget>

#include <qwt3d_surfaceplot.h>

using namespace Qwt3D;

class Analizer : public QWidget
{
    Q_OBJECT
public:
    Analizer();

    SurfacePlot *pw;

    bool openColorMap(ColorVector &cv, QString fname);

    QFrame *frame;

};

#endif // ANALIZER_H
