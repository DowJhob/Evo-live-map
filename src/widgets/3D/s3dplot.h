#ifndef S3DPLOT_H
#define S3DPLOT_H

#include <QWidget>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

#include "surfacegraph.h"


class s3Dplot : public QWidget
{
    Q_OBJECT
public:
    s3Dplot();
};

#endif // S3DPLOT_H
