#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

//#include <QObject>
//#include <QWidget>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>

using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
    Q_OBJECT
public:
    SurfaceGraph(Q3DSurface *surface);
    ~SurfaceGraph();

    void enableHeightMapModel(bool enable);
    void enableSqrtSinModel(bool enable);

    //! [0]
    void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }
    //! [0]

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

public Q_SLOTS:
    void changeTheme(int theme);

private:
    Q3DSurface *m_graph;
    QHeightMapSurfaceDataProxy *m_heightMapProxy;
    QSurfaceDataProxy *m_sqrtSinProxy;
    QSurface3DSeries *m_heightMapSeries;
    QSurface3DSeries *m_sqrtSinSeries;

    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;
    int m_heightMapWidth;
    int m_heightMapHeight;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void fillSqrtSinProxy();






float fuelMap[15][18]
{{13.34468085106383, 13.34468085106383, 13.34468085106383, 13.34468085106383, 13.34468085106383, 13.34468085106383, 13.34468085106383, 11.98471337579618,11.68695652173913,11.40363636363636,11.13372781065089,10.87630057803468,10.63050847457627,10.39558011049724,10.17081081081081,9.955555555555556,9.749222797927461,9.551269035532995},
{14.7,               14.7, 14.7,14.7,14.7,14.7,14.7,13.06666666666667,12.71351351351351,12.37894736842105,12.06153846153846,11.76,11.47317073170732,11.2,10.93953488372093,10.69090909090909,10.45333333333333,10.22608695652174},
{14.7,14.7,14.7,14.7,14.7,14.7,14.7,12.88767123287671,12.71351351351351,12.544,12.37894736842105,12.06153846153846,11.76,11.47317073170732,11.2,10.93953488372093,10.69090909090909,10.45333333333333},
{14.7,14.7,14.7,14.7,14.7,14.7,14.04179104477612,13.06666666666667,12.97655172413793,12.71351351351351,12.544,12.37894736842105,12.06153846153846,11.76,11.47317073170732,11.2,10.93953488372093,10.69090909090909},
{14.7,14.7,14.7,14.7,14.7,14.7,14.04179104477612,13.25070422535211,13.15804195804196,12.88767123287671,12.71351351351351,12.544,12.21818181818182,11.90886075949367,11.61481481481481,11.33493975903614,11.06823529411765,10.81379310344828},
{14.7,14.7,14.7,14.7,14.7,14.7,14.04179104477612,13.44,13.34468085106383,13.06666666666667,12.88767123287671,12.37894736842105,11.61481481481481,11.33493975903614,11.06823529411765,10.81379310344828,10.57078651685393,10.33846153846154},
{14.7,14.7,14.7,14.7,14.7,14.7,14.04179104477612,13.44,13.25070422535211,12.97655172413793,12.71351351351351,11.90886075949367,11.40363636363636,10.69090909090909,10.11612903225806,9.749222797927461,9.649230769230769,9.503030303030302},
{14.7,14.7,14.7,14.7,14.7,14.7,14.04179104477612,13.34468085106383,13.06666666666667,12.8,12.544,11.76,11.13372781065089,10.45333333333333,9.903157894736841,9.749222797927461,9.649230769230769,9.503030303030302},
{14.7,14.7,14.7,14.7,14.7,14.7,14.04179104477612,13.34468085106383,12.97655172413793,12.62818791946309,12.29803921568627,11.61481481481481,11.06823529411765,10.33846153846154,9.851308900523559,9.749222797927461,9.649230769230769,9.503030303030302},
{14.7,14.7,14.7,14.7,14.7,14.7,14.04179104477612,13.06666666666667,12.8,12.46092715231788,11.83396226415094,11.47317073170732,10.69090909090909,10.11612903225806,9.851308900523559,9.749222797927461,9.649230769230769,9.503030303030302},
{14.7,14.7,14.7,14.7,14.36335877862595,14.04179104477612,13.44,12.71351351351351,12.46092715231788,11.98471337579618,11.54355828220859,11.26706586826347,10.33846153846154,9.955555555555556,9.749222797927461,9.649230769230769,9.551269035532995,9.455276381909547},
{13.83529411764706,13.83529411764706,13.83529411764706,13.83529411764706,13.63478260869565,13.44,12.88767123287671,12.544,12.06153846153846,11.47317073170732,11.00350877192982,10.752,10.22608695652174,9.851308900523559,9.749222797927461,9.649230769230769,9.551269035532995,9.455276381909547},
{13.44,13.44,13.44,13.44,13.25070422535211,13.06666666666667,12.62818791946309,12.29803921568627,11.40363636363636,10.81379310344828,10.45333333333333,10.28196721311475,10.11612903225806,9.851308900523559,9.749222797927461,9.649230769230769,9.551269035532995,9.455276381909547},
{13.06666666666667,13.06666666666667,13.06666666666667,13.06666666666667,12.88767123287671,12.71351351351351,12.13935483870968,11.83396226415094,11.2,10.752,10.39558011049724,10.17081081081081,10.00851063829787,9.851308900523559,9.749222797927461,9.649230769230769,9.551269035532995,9.455276381909547},
{13.06666666666667,13.06666666666667,13.06666666666667,13.06666666666667,12.88767123287671,12.71351351351351,12.06153846153846,11.40363636363636,11.2,10.752,10.28196721311475,10.17081081081081,10.00851063829787,9.851308900523559,9.749222797927461,9.649230769230769,9.551269035532995,9.455276381909547}};


};

#endif // SURFACEGRAPH_H
