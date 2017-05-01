/*
* File    : CFileOperationManager.h
*
* Date    : 2017-04-25
*
* Author  :
*
* Brief   : �ļ���������
*
* Note    :
*/

#ifndef _CFILE_OPERATION_MANAGER_H_
#define _CFILE_OPERATION_MANAGER_H_


#include "QtCore/QFile"
#include "QtXml/QDomDocument"
#include "QtCore/QStringList"
#include "type.h"

class CFileOperationManager
{
public:
    CFileOperationManager(const QString& filePath);
    ~CFileOperationManager();

    // ��ȡxml�ļ�
    bool ReadXmlFile(QMap<int, QList<WidgetProperty>>& mapTmpItems);

    // дxml�ļ�
    bool writeXmlFile(QMap<int, QList<WidgetProperty>>& mapTmpItems);


private:
	QDomDocument *m_doc;
	QFile *m_file;
	QString m_strFilePath;
};

#endif