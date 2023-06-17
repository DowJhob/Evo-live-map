#include "ecu-definition.h"

ecuDefinition::ecuDefinition()
{

}

ecuDefinition::~ecuDefinition()
{
    qDebug() << "=========== ~ecuDefinition ================";
    reset();
}

void ecuDefinition::reset()
{
    scalingsMaps.clear();
    for(auto c : qAsConst(RAMtables))
    {
        delete c;
    }
    RAMtables.clear();
}

bool ecuDefinition::fromFile(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    _parser(file, "");        // парсим файл
    delete file;
    return true;
}

bool ecuDefinition::fromROMID(QString ROMID)
{
    return fromFile(getFile(QApplication::applicationDirPath() + "/xml/", ROMID));
}

QString ecuDefinition::getFile(QString path, QString CalID)       // Для поиска файлов в каталоге
{
    // Пытаемся найти правильные файлы, в текущем каталоге
    QStringList listFiles = QDir(path).entryList((CalID + "*.xml ").split(" "), QDir::Files);  //выборка файлов по маскам
    if (listFiles.size()  == 0)            // если файл не найдем вернем егог
        return "";
    //return QFileDialog::getOpenFileName(nullptr,  tr("Open xml"), path, tr("xml files (*.xml)"));
    else
        return path + listFiles.at(0);
}

//void ecuDefinition::_parser(QIODevice *device, QString includeID)
//{
//    QDomElement root = getXMLDom(device);

//    QDomNode romid = root.firstChildElement("romid");
//    QDomNode xmlid_node = romid.firstChildElement("xmlid");
//    if(!includeID.isEmpty() && xmlid_node.toElement().text() != includeID)
//        return;
//    QDomNode node = root.firstChild();

//    qDebug() << "T============================================== includeID =============================================================" << includeID << endl;

//    while (!node.isNull())
//    {
//        const QDomElement el = node.toElement();
//        if(el.tagName() == "include")
//        {
//            QString includeID2 = el.text();
//            //            qDebug() << "T==================================================== includeID =======================================================" << includeID;
//            if(!includeID2.isEmpty())
//            {
//                QDir dir(QApplication::applicationDirPath() + "/xml");
//                for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
//                {
//                    qDebug() << "T======================================= file.fileName() ======================================" << includeID << xmlid_node.toElement().text() << file.fileName();
//                    QFile iod(file.absoluteFilePath());
//                    if (!iod.open(QIODevice::ReadOnly | QIODevice::Text))
//                    {
//                       // QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(iod.fileName()));
//                        return;
//                    }
////                    qDebug() << "T============================================== xmlid_node =============================================================";
//                    _parser(&iod, includeID2);
//                }
//            }
//        }
//        else
//        {
//            if (el.tagName() == "scaling")                                                 //сохраним все скалинги
//                    getScaling(el);
//            if (el.tagName() == "table")                                                   // находим таблицу
//            {
//                if (!el.attribute("RAM_addr").isEmpty() )           // Если есть адрес в оперативке - парсим
//                    getLivemap(el);
//            }
//            if (el.tagName() == "live")                                                   // параметры
//            {
//                QString nodeName = el.attribute("name");
//                if (nodeName == "varDEAD")
//                    ramMut.DEAD_var = el.attribute("address").toUInt(nullptr, 16);
//                if (nodeName == "RAM_MUT")
//                {
//                    ramMut.addr = el.attribute("address").toUInt(nullptr, 16);
//                    getMUTparam(el);
//                }
//                qDebug() << " live " << ramMut.addr;
//            }
//        }

//        node = node.nextSibling();
//    }

//    for(auto c : qAsConst(RAMtables))
//    {
//        c->setScaling(&scalingsMaps);    // проставим скалинги
//    }
//}

//QDomElement ecuDefinition::getXMLDom(QIODevice *device)
//{
//    QString errorStr;
//    int errorLine;
//    int errorColumn;
//    QDomDocument doc;
//    //открываем документ
//    if (doc.setContent(device, true, &errorStr, &errorLine, &errorColumn))
//    {
//        QDomElement root = doc.documentElement();
//        if (root.tagName() == "rom")
//        {
//            return root;
//        }
//        else
//        {
//            qDebug() << "The file is not a rom xml";
//        }
//    }
//    else
//    {
//        //            lastError = "Line %1, column %2";
//        //            lastError = lastError.arg( errorLine).arg(errorColumn);
//        qDebug() << errorStr << errorLine << errorColumn ;
//    }
//    return QDomElement();
//}

void ecuDefinition::getMUTparam(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    while (!node.isNull())
    {
        QDomElement el = node.toElement();
        if (el.tagName() == "ram_mut") //находим параметры мут таблицы
        {
            mutParam _mut_param(el);
            _mut_param.setScaling(scalingsMaps.value(el.attribute("scaling")));

            if( ramMut.size() < _mut_param.number + 1 )
                ramMut.resize(_mut_param.number + 1);
            ramMut[_mut_param.number] = _mut_param;
            //RAM_MUTh.insert("", _mut_param);
        }
        node = node.nextSibling();
    }
    int offset = 0;
    for(int i = 0; i < ramMut.size(); i++)
    {
        ramMut[i].offset = offset;
        offset += ramMut[i].scaling.getElementSize();
    }

    ramMut.byteSize = offset;
    //RAM_MUT.resize(RAM_MUT_size);
}

void ecuDefinition::getLivemap(const QDomElement &element)
{
    Map *mainTableDeclaration = new Map;
    mainTableDeclaration->fromXML(element);
    RAMtables.insert(mainTableDeclaration->Name, mainTableDeclaration);
}

void ecuDefinition::getScaling(const QDomElement &el)
{
    Scaling sc(el);
    scalingsMaps.insert(sc.name, sc);
}

void ecuDefinition::serialize(QDomElement el)
{
    if (el.tagName() == "scaling")                                                 //сохраним все скалинги
            getScaling(el);
    if (el.tagName() == "table")                                                   // находим таблицу
    {
        if (!el.attribute("RAM_addr").isEmpty() )           // Если есть адрес в оперативке - парсим
            getLivemap(el);
    }
    if (el.tagName() == "live")                                                   // параметры
    {
        QString nodeName = el.attribute("name");
        if (nodeName == "varDEAD")
            ramMut.DEAD_var = el.attribute("address").toUInt(nullptr, 16);
        if (nodeName == "RAM_MUT")
        {
            ramMut.addr = el.attribute("address").toUInt(nullptr, 16);
            getMUTparam(el);
        }
        qDebug() << " live " << ramMut.addr;
    }
}

void ecuDefinition::subSerialize(QDomNode node)
{
    for(auto c : qAsConst(RAMtables))
    {
        c->setScaling(&scalingsMaps);    // проставим скалинги
    }
}
