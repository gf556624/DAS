#include "CFileOperationManager.h"
#include "QtCore/QDebug"
#include "QtXlsx/QtXlsx.h"


CFileOperationManager::CFileOperationManager()
{

}

CFileOperationManager::CFileOperationManager(const QString& filePath)
{
    m_strFilePath = filePath;
    if (filePath.isEmpty())
	{
        m_strFilePath = QString("");
	}
	m_doc = new QDomDocument();
    m_file = new QFile(m_strFilePath);
}


CFileOperationManager::~CFileOperationManager()
{

}

// XML�ļ���ȡ
bool CFileOperationManager::ReadXmlFile(QMap<int, QList<WidgetProperty_t>>& mapTmpItems)
{
    if (!m_file->open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!m_doc->setContent(m_file))
    {
        m_file->close();
        return false;
    }
    m_file->close();

    QDomElement docElem = m_doc->documentElement();

    QDomNode n = docElem.firstChild();
    while (!n.isNull()) 
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if (!e.isNull()) 
        {
            WidgetProperty_t tmpWgtPro;
            QDomNode tmpNode = e.firstChild();
            while (!tmpNode.isNull())
            {
                QDomElement tmpElement = tmpNode.toElement();
                if ("Type" == tmpElement.tagName())
                {
                    tmpWgtPro.m_type = (ITEMTYPE)tmpElement.text().toInt();
                }
                else if ("Channel" == tmpElement.tagName())
                {
                    tmpWgtPro.m_iChannel = tmpElement.text().toInt();
                }
                else if ("X" == tmpElement.tagName())
                {
                    tmpWgtPro.m_realX = tmpElement.text().toDouble();
                }
                else if ("Y" == tmpElement.tagName())
                {
                    tmpWgtPro.m_realY = tmpElement.text().toDouble();
                }
                else if ("Width" == tmpElement.tagName())
                {
                    tmpWgtPro.m_realWidth = tmpElement.text().toDouble();
                }
                else if ("Heigth" == tmpElement.tagName())
                {
                    tmpWgtPro.m_realHeight = tmpElement.text().toDouble();
                }
                else if ("StartTime" == tmpElement.tagName())
                {
                    tmpWgtPro.m_strStart = tmpElement.text();
                }
                else if ("EndTime" == tmpElement.tagName())
                {
                    tmpWgtPro.m_strEnd = tmpElement.text();
                }
                else if ("PlayPos" == tmpElement.tagName())
                {
                    tmpWgtPro.m_strPlayPos = tmpElement.text();
                }
                //qDebug() << tmpElement.attribute("value");
                tmpNode = tmpNode.nextSibling();
            }
            mapTmpItems[tmpWgtPro.m_type].append(tmpWgtPro);
        }
        n = n.nextSibling();
    }

    return true;
}


bool CFileOperationManager::ReadXmlFile(QList<CurveLine_t>& lstTmpItems)
{
    if (!m_file->open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!m_doc->setContent(m_file))
    {
        m_file->close();
        return false;
    }
    m_file->close();

    QDomElement docElem = m_doc->documentElement();

    QDomNode n = docElem.firstChild();
    while (!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if (!e.isNull())
        {
            CurveLine_t tmpCurveline;
            //qDebug() << e.tagName(); // the node really is an element.
            QDomNode tmpNode = e.firstChild();
            while (!tmpNode.isNull())
            {
                QDomElement tmpElement = tmpNode.toElement();
                if ("name" == tmpElement.tagName())
                {
                    tmpCurveline.m_strName = tmpElement.text();
                }
                else if ("displayname" == tmpElement.tagName())
                {
                    tmpCurveline.m_strDisplayName = QString::fromUtf8(tmpElement.text().toUtf8());
                }
                else if ("range" == tmpElement.tagName())
                {
                    QDomNode tmpNode2 = tmpElement.firstChild();
                    while (!tmpNode2.isNull())
                    {
                        QDomElement tmpElement2 = tmpNode2.toElement();
                        if ("minvalue" == tmpElement2.tagName())
                        {
                            tmpCurveline.m_realMin = tmpElement2.text().toDouble();
                        }
                        else if ("maxvalue" == tmpElement2.tagName())
                        {
                            tmpCurveline.m_realMax = tmpElement2.text().toDouble();
                        }
                        tmpNode2 = tmpNode2.nextSibling();
                    }
                }
                tmpNode = tmpNode.nextSibling();
            }
            lstTmpItems.append(tmpCurveline);
        }
        n = n.nextSibling();
    }

    return true;
}


// дXML�ļ�
bool CFileOperationManager::writeXmlFile(QMap<int, QList<WidgetProperty_t>>& mapTmpItems)
{
    if (!m_file->exists())
    {
        m_file->remove();
    }
    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        return false;
    }

    m_doc->appendChild(m_doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
    QDomElement root = m_doc->createElement("WidgetXml");
    m_doc->appendChild(root);

    for (auto& itr = mapTmpItems.begin(); itr != mapTmpItems.end(); ++itr)
    {
        QDomElement note;
        if (Item_TimeAxis == itr.key())
        {
            for (auto& tmpObj : itr.value())
            {
                note = m_doc->createElement("TimeAxis");
                root.appendChild(note);
                QDomElement tmpEle = m_doc->createElement("Type");
                QDomText tmpText = m_doc->createTextNode(QString::number(tmpObj.m_type));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("X");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realX));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Y");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realY));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Width");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realWidth));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Heigth");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realHeight));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("StartTime");
                tmpText = m_doc->createTextNode(tmpObj.m_strStart);
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("EndTime");
                tmpText = m_doc->createTextNode(tmpObj.m_strEnd);
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("PlayPos");
                tmpText = m_doc->createTextNode(tmpObj.m_strPlayPos);
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);
            }
        }
        else if (Item_Video == itr.key())
        {
            for (auto& tmpObj : itr.value())
            {
                note = m_doc->createElement("ItemVideo");
                root.appendChild(note);
                QDomElement tmpEle = m_doc->createElement("Type");
                QDomText tmpText = m_doc->createTextNode(QString::number(tmpObj.m_type));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Channel");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_iChannel));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("X");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realX));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Y");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realY));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Width");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realWidth));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Heigth");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realHeight));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);
            }
        }
        else if (Item_Chart == itr.key())
        {
            for (auto& tmpObj : itr.value())
            {
                note = m_doc->createElement("ItemChart");
                root.appendChild(note);
                QDomElement tmpEle = m_doc->createElement("Type");
                QDomText tmpText = m_doc->createTextNode(QString::number(tmpObj.m_type));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Channel");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_iChannel));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("X");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realX));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Y");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realY));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Width");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realWidth));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Heigth");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realHeight));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);
            }
        }
        else if (Item_Table == itr.key())
        {
            for (auto& tmpObj : itr.value())
            {
                note = m_doc->createElement("ItemTable");
                root.appendChild(note);
                QDomElement tmpEle = m_doc->createElement("Type");
                QDomText tmpText = m_doc->createTextNode(QString::number(tmpObj.m_type));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Channel");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_iChannel));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("X");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realX));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Y");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realY));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Width");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realWidth));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);

                tmpEle = m_doc->createElement("Heigth");
                tmpText = m_doc->createTextNode(QString::number(tmpObj.m_realHeight));
                tmpEle.appendChild(tmpText);
                note.appendChild(tmpEle);
            }
        }
    }

    QTextStream out(m_file);
    out.setCodec("UTF-8");
    m_doc->save(out, 4, QDomNode::EncodingFromTextStream);
    m_file->close();

    return true;
}


/*
* ����excel�ļ�
* strFileName �ļ���(��·��)
* slstHeader ��ͷ
* lstExcelData ����Դ
*
*/
bool CFileOperationManager::writeExcelFile(const QString& strFileName, const QStringList& slstHeader, QList< QList<QVariant> >& lstExcelData)
{
    if (strFileName.isEmpty())
    {
        return false;
    }

    /*if (0 == lstExcelData.count())
    {
    return false;
    }*/

    QXlsx::Document oTmpXlsx(strFileName);
    QXlsx::Format format;
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format.setBorderStyle(QXlsx::Format::BorderThin);

    // ��ͷ
    for (int i = 0; i < slstHeader.size(); ++i)
    {
        if (!oTmpXlsx.write(1, i + 1, slstHeader.at(i), format))   // ��һ�б�ͷ
        {
            return false;
        }
    }

    // д��ʵ������
    for (int iRow = 0; iRow < lstExcelData.size(); ++iRow)
    {
        if (lstExcelData.at(iRow).size() < slstHeader.size())
        {
            return false;
        }
        for (int iCol = 0; iCol < slstHeader.size(); ++iCol)
        {
            // ��һ���Ǳ�ͷ �ӵڶ��� ��һ�п�ʼд
            if (!oTmpXlsx.write(iRow + 2, iCol + 1, lstExcelData.at(iRow).at(iCol).toString().replace("T", " "), format))
            {
                return false;
            }
        }
    }
    oTmpXlsx.save();


    return true;
}