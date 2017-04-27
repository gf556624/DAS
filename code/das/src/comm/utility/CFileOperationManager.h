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

class CFileOperationManager
{
public:
    CFileOperationManager(const QString& filePath);
    ~CFileOperationManager();

    // ��ȡxml�ļ�
    bool ReadXmlFile();

    // дxml�ļ�
    bool writeXmlFile();


	//��xml�ļ�
	bool OpenFile();

	//��ȡ��ȡ�ļ���Դ��ǣ�TRUE-XLS��FALSE-�������ļ�
	bool GetFromXLSFileFlag();

private:
	QDomDocument *m_doc;
	QFile *m_file;
	QString m_strFilePath;
};

#endif