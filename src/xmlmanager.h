#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QObject>
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
    xmlManager();
    void _parser(QIODevice *device, QString includeID);

private:

    QDomElement getXMLDom(QIODevice *device);

    virtual void serialize(QDomElement el);

    virtual void subSerialize(QDomNode node);

};

#endif // XMLMANAGER_H
