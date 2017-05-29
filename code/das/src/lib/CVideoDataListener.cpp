#include "CVideoDataListener.h"

#include "QtCore/QDateTime"
#include "QtGui/QImage"

#include "type.h"


CVideoDataListener::CVideoDataListener(QObject *parent)
    : QObject(parent)
{
}


CVideoDataListener::~CVideoDataListener()
{
}


int CVideoDataListener::init(const QString& strStorage, int iChannel)
{
    m_videoSession.Init(strStorage.toStdString().c_str(), iChannel);
    m_videoSession.GetStreamMgr()->AddStream(this);

    return 1;
}


int CVideoDataListener::setScape(const QDateTime& dtBegin, const QDateTime& dtEnd)
{
    // time format eg:20170527143910-20170528143910
    QString strTime = dtBegin.toString("yyyyMMddhhmmss") + "-" + dtEnd.toString("yyyyMMddhhmmss");
    m_videoSession.SetScape(strTime.toStdString().c_str());

    return 1;
}


int CVideoDataListener::skipTo(const QDateTime& dtPos)
{
    QString strCurrentTime = dtPos.toString("yyyyMMddhhmmss");
    m_videoSession.SkipTo(strCurrentTime.toStdString().c_str());

    return 1;
}


int CVideoDataListener::play()
{
    m_videoSession.Play();
    m_videoSession.SkipTo("20170411131010");

    return 1;
}


int CVideoDataListener::pause()
{
    m_videoSession.Pause();

    return 1;
}

#include "windows.h"

// ������Ƶ�Ľӽ����ߣ�����Ҫ�������ͼ�����ݵĸ߿�pCustomData 
// ָ����һ��struct{ unsigned int width; unsigned int height; } ���͵����� 
// payload ����can��video 
// buffer��������Ƶ�����ߣ�Ϊ������yuv420p�����ݻ�����������Can���ݽ����ߣ�ΪCCanData���͵Ļ����� 
// length�� ������Ƶ��Ϊ�����ĳ��ȣ�����Can���ݽ���ΪCCanData���������� 
// secs��Ϊ����1970������������һ���ܴ��������usecsΪ����������Χ0 - 1000�� 
void CVideoDataListener::OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
    unsigned long secs, unsigned long msecs, void* pCustomData)
{
    // 1 - video data, 3 - can data 
    if (payload == 1)   // video data 
    {
        imageInfo_t* imageInfo = (imageInfo_t*)pCustomData;

        // yuv -> rgb ????  
        QImage image(buffer, imageInfo->iWidth, imageInfo->iHeight, QImage::Format_RGB32);

        // current time 
        QDateTime currentDateTime = QDateTime::fromMSecsSinceEpoch(secs * 1000 + msecs);

        emit sigVideoUpdate(image, currentDateTime);
    }
    else if (payload == 3)  // can data 
    {
        return;
    }
}

