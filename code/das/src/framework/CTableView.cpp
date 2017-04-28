#include "CTableView.h"
#include "CFrozenTableView.h"

#include "QtWidgets/QHBoxLayout"
#include "QtGui/QStandardItemModel"
#include "QtWidgets/QHeaderView"
#include "QtCore/QEvent"
#include "QtGui/QMouseEvent"


CTableView::CTableView(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent)
    , m_pTableView(nullptr)
    , m_pModel(nullptr)
    , m_strStartTime("")
    , m_strEndTime("")
{
    m_mapHorizontalHeader.clear();
    initLayout();
}

CTableView::~CTableView()
{

}


ITEMTYPE CTableView::type()
{
    return Item_Table;
}



// ����ˮƽ��ͷ���� ����xml�ļ���ȡ������
void CTableView::setHorizontalHeader(QMap<QString, QString>& mapHeader)
{
    m_mapHorizontalHeader = mapHeader;
}

/*
* ���ÿ�ʼ����ʱ��
* strStart ��ʼʱ�� strEnd ����ʱ��  ʱ���ʽ yyyy/MM/dd hh:mm:ss
*/
void CTableView::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_strStartTime = strStart;
    m_strEndTime = strEnd;
}


// ��ʼ������
void CTableView::initLayout()
{
    m_pModel = new QStandardItemModel(this);
    m_pModel->setColumnCount(m_mapHorizontalHeader.size());
    m_pTableView = new CFrozenTableView(m_pModel, 1);  // ���ж���
    m_pTableView->resizeColumnsToContents();
    m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pTableView->setAlternatingRowColors(true);

    QHBoxLayout* pTmpLayout = new QHBoxLayout(this);
    pTmpLayout->addWidget(m_pTableView);

    this->setLayout(pTmpLayout);
}

