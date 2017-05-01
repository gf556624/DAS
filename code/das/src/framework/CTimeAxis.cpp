#include "CTimeAxis.h"

#include "QtWidgets/QLabel"
#include "QtWidgets/QSlider"
#include "QtWidgets/QHBoxLayout"
#include "QtCore/QDateTime"


CTimeAxis::CTimeAxis(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent)
    , m_pLbStartTime(nullptr)
    , m_pLbEndTime(nullptr)
    , m_pSlider(nullptr)
{
    initLayout();
}

CTimeAxis::~CTimeAxis()
{

}



ITEMTYPE CTimeAxis::type()
{
    return Item_TimeAxis;
}



/*
* ���ÿ�ʼ����ʱ��
* strStart ��ʼʱ�� strEnd ����ʱ��  ʱ���ʽ yyyy/MM/dd hh:mm:ss
*/
void CTimeAxis::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_pLbStartTime->setText(strStart);
    m_pLbEndTime->setText(strEnd);

    uint iTmpMin = QDateTime::fromString(strStart, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint iTmpMax = QDateTime::fromString(strEnd, "yyyy/MM/dd hh:mm:ss").toTime_t();

    m_pSlider->setRange(iTmpMin, iTmpMax);
}

// ���û���λ��
void CTimeAxis::setSliderPosition(const QString& strValue)
{
    uint iTmpValue = QDateTime::fromString(strValue, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint iOffset = m_pSlider->maximum() - m_pSlider->minimum();
    if (iOffset == 0)
    {
        return;
    }
    int iProgressValue = (iTmpValue - (uint)(m_pSlider->minimum())) / (iOffset / (uint)(m_pSlider->maximum()));

    m_pSlider->setValue(iProgressValue);
}


QString CTimeAxis::getStartTime() const 
{ 
	return m_pLbStartTime->text().trimmed();
}


QString CTimeAxis::getEndTime() const 
{ 
	return m_pLbEndTime->text().trimmed();
}


QString CTimeAxis::getSliderPosition() const 
{ 
	int iSliderValue = m_pSlider->value();
	uint iOffset = m_pSlider->maximum() - m_pSlider->minimum();
	uint iTmpValue = iSliderValue * (iOffset / (uint)(m_pSlider->maximum())) + (uint)(m_pSlider->minimum());
	QString strValue = QDateTime::fromTime_t(iTmpValue).toString("yyyy/MM/dd hh:mm:ss");
	return strValue;
}


// ��ʼ������
void CTimeAxis::initLayout()
{
    m_pLbStartTime = new QLabel(this);
    m_pLbEndTime = new QLabel(this);
    m_pSlider = new QSlider(Qt::Horizontal, this);

    QHBoxLayout* pTmpLayout = new QHBoxLayout(this);
    pTmpLayout->addWidget(m_pLbStartTime);
    pTmpLayout->addWidget(m_pSlider);
    pTmpLayout->addWidget(m_pLbEndTime);

    this->setLayout(pTmpLayout);
}
