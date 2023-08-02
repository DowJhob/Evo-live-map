#ifndef XMLMANAGER_H
#define XMLMANAGER_H

//#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QDomElement>

class xmlManager// : public QObject
{
//    Q_OBJECT
public:

    void _parser(QIODevice *device, QString includeID);

private:

    QDomElement getXMLDom(QIODevice *device);

    virtual void serialize(QDomElement el) = 0;

    virtual void subSerialize(QDomNode node) = 0;

};

#endif // XMLMANAGER_H
