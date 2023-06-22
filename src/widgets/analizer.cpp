#include "analizer.h"
#include "qgridlayout.h"

Analizer::Analizer()
{


    frame = new QFrame(this);
    frame->setObjectName(QString::fromUtf8("frame"));
    auto qgrl = new QGridLayout(this);
    qgrl->addWidget(frame);
setLayout(qgrl);

    pw = new SurfacePlot(frame);

    pw->makeCurrent();

    pw->setRotation(30,0,15);
//    pw->setScale(1,1,1);
//    pw->setShift(0.15,0,0);
    pw->setZoom(0.9);


    //     pw->setCoordinateStyle(BOX);


    Triple	a(0, 0, 10);
    Triple	b(0, 11, 0.51);
    Triple	c(11, 11, 31);
    Triple	d(15, 0, 9);
    Triple	e(22, 3, 9);
    Triple	f(17, 7, 9);
    Triple	g(8, 9, 9);
    Triple	h(4, 23, 9);
    Triple	i(9, 43, 9);


    TripleField	ii {
    Triple{0.0, 0.0,0.0},Triple{0.7,0.0,2.0},Triple{0.652730560583,0.252869166331,2.0}
    };




    TripleField vtf{a, b, c, d, e, f, g, h, i};


    //! Holds indices in a TripleField interpreted as counterclockwise node numbering for a convex polygon
    Cell cell {0, 2, 1};
    Cell cell2{1, 2, 3};
    Cell cell3{1, 2, 3};
    Cell cell4{3, 0, 1};
    Cell cell5{0, 1, 2, 3, 4, 5, 6, 7, 8};
    //! Vector of convex polygons. You need a TripleField as base for the node data
    CellField cf{cell/*, cell2, cell3, cell4, cell5*/};


pw->setPlotStyle(Qwt3D::FILLED);


//pw->setFloorStyle(FLOORISO);


//ColorVector cv;

//if (!openColorMap(cv, "fileName"))
//    return;

//auto col_ = new StandardColor(pw);
//col_->setColorVector(cv);
//pw->setDataColor(col_);




    pw->setUpdatesEnabled(true);
//    pw->loadFromData(vtf, cf);
    pw->loadFromData(ii, cf);

//    pw->createDataRepresentation(vtf, cf);



    pw->legend()->setMajors(7);
    pw->legend()->setMinors(5);
    double start, stop;
    pw->coordinates()->axes[Z1].limits(start, stop);
    pw->legend()->setLimits(start, stop);

    pw->coordinates()->axes[X1].setLabelString(QString("X1"));
    pw->coordinates()->axes[X2].setLabelString(QString("X2"));
    pw->coordinates()->axes[X3].setLabelString(QString("X3"));
    pw->coordinates()->axes[X4].setLabelString(QString("X4"));

    pw->coordinates()->axes[Y1].setLabelString(QString("Y1"));
    pw->coordinates()->axes[Y2].setLabelString(QString("Y2"));
    pw->coordinates()->axes[Y3].setLabelString(QString("Y3"));
    pw->coordinates()->axes[Y4].setLabelString(QString("Y4"));

    pw->coordinates()->axes[Z1].setLabelString(QString("Z1"));
    pw->coordinates()->axes[Z2].setLabelString(QString("Z2"));
    pw->coordinates()->axes[Z3].setLabelString(QString("Z3"));
    pw->coordinates()->axes[Z4].setLabelString(QString("Z4"));

//    double l = 3;
//    pw->createCoordinateSystem(Triple(-l, -l, -l), Triple(l, l, l));

//    for (unsigned i=0; i!=pw->coordinates()->axes.size(); ++i)
//    {
//      pw->coordinates()->axes[i].setMajors(7);
//      pw->coordinates()->axes[i].setMinors(5);
//    }

//        pw->setResolution(300);

    pw->setMeshColor(RGBA(200, 0, 0));
    pw->setMeshLineWidth(10);
//    pw->updateData();
//         pw->updateGL();

//pw->update();
pw->resize(1200, 1600);

    pw->show();

show();

}

bool Analizer::openColorMap(ColorVector &cv, QString fname)
{
//    if (fname.isEmpty())
//        return false;

//    std::ifstream file(QWT3DLOCAL8BIT(fname));

//    if (!file)
//        return false;

//    RGBA rgb;
    RGBA rgb(200,0,0,0);
    cv.clear();

//    while (file) {
//        file >> rgb.r >> rgb.g >> rgb.b;
//        file.ignore(1000, '\n');
//        if (!file.good())
//            break;
//        else {
//            rgb.a = 1;
//            rgb.r /= 255;
//            rgb.g /= 255;
//            rgb.b /= 255;
//            cv.push_back(rgb);
//        }
//    }

    cv.push_back(rgb);
    cv.push_back(rgb);
    cv.push_back(rgb);
    cv.push_back(rgb);
    cv.push_back(rgb);
    return true;
}
