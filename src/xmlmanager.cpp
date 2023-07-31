#include "xmlmanager.h"


void xmlManager::_parser(QIODevice *device, QString includeID)
{
    QDomElement root = getXMLDom(device);

    QDomNode romid = root.firstChildElement("romid");
    QDomNode xmlid_node = romid.firstChildElement("xmlid");
    if(!includeID.isEmpty() && xmlid_node.toElement().text() != includeID)
        return;
    QDomNode node = root.firstChild();

    qDebug() << "T============================================== includeID =============================================================" << includeID << endl;

    while (!node.isNull())
    {
        const QDomElement el = node.toElement();
        if(el.tagName() == "include")
        {
            QString includeID2 = el.text();
            //            qDebug() << "T==================================================== includeID =======================================================" << includeID;
            if(!includeID2.isEmpty())
            {
                QDir dir(QApplication::applicationDirPath() + "/xml");
                for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
                {
                    qDebug() << "T======================================= file.fileName() ======================================" << includeID << xmlid_node.toElement().text() << file.fileName();
                    QFile iod(file.absoluteFilePath());
                    if (!iod.open(QIODevice::ReadOnly | QIODevice::Text))
                    {
                        //                        QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(iod.fileName()));
                        return;
                    }
                    //                    qDebug() << "T============================================== xmlid_node =============================================================";
                    _parser(&iod, includeID2);
                }
            }
        }
        else
        {
            serialize(el);
        }
        node = node.nextSibling();
    }

    //qDebug() << " live " << ramMut.addr;
    node = root.firstChild();
    subSerialize(node);
}

QDomElement xmlManager::getXMLDom(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    //открываем документ
    if (doc.setContent(device, true, &errorStr, &errorLine, &errorColumn))
    {
        QDomElement root = doc.documentElement();
        if (root.tagName() == "rom")
        {
            return root;
        }
        else
        {
            qDebug() << "The file is not a rom xml";
        }
    }
    else
    {
        //            lastError = "Line %1, column %2";
        //            lastError = lastError.arg( errorLine).arg(errorColumn);
        qDebug() << errorStr << errorLine << errorColumn ;
    }
    return QDomElement();
}
