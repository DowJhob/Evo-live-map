#ifndef ANALIZER_H
#define ANALIZER_H

#include <QFrame>
#include <QWidget>

//#include <qwt3d_surfaceplot.h>
#include <qwt3d_meshplot.h>

using namespace Qwt3D;

class Analizer : public QWidget
{
    Q_OBJECT
public:
    Analizer();

    Qwt3D::MeshPlot *pw;

    bool openColorMap(ColorVector &cv, QString fname);

    QFrame *frame;

};

#endif // ANALIZER_H
