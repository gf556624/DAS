#include "CTableView.h"
#include "CFrozenTableView.h"
#include "CFileOperationManager.h"
#include "constant.h"

#include "QtWidgets/QVBoxLayout"
#include "QtGui/QStandardItemModel"
#include "QtWidgets/QHeaderView"
#include "QtCore/QEvent"
#include "QtGui/QMouseEvent"
#include "QtWidgets/QPushButton"
#include "QtWidgets/QFileDialog"
#include "QtWidgets/QMessageBox"

const char* cstExportSuccess = "Export Success!";
const char* cstExportFailed = "Export Failed!";
const char* cstTips = "Tips";
const char* cstExport = "Export excel";

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

    QVBoxLayout* pTmpLayout = new QVBoxLayout(this);
    pTmpLayout->addWidget(m_pTableView);

    m_pExcelBtn = new QPushButton(trFormString(cstExport), this);
    connect(m_pExcelBtn, &QPushButton::clicked, this, &CTableView::OnExport);

    pTmpLayout->addWidget(m_pExcelBtn, 0, Qt::AlignRight);

    this->setLayout(pTmpLayout);
}


// ���������
void CTableView::setTableViewData()
{

}

// ����excel
void CTableView::OnExport()
{
    QString strFileName = QFileDialog::getSaveFileName(nullptr, trFormString(cstExport), "./", "*.xlsx");
    if (strFileName.isEmpty())
    {
        return;
    }

    if (QFileInfo(strFileName).suffix().isEmpty())
    {
        strFileName += ".xlsx";
    }

    // ��ͷ
   // m_mapHorizontalHeader["aa"] = "aa";
    //m_mapHorizontalHeader["bb"] = "bb";

    QStringList slstHeader;
    for (auto& itr = m_mapHorizontalHeader.begin(); itr != m_mapHorizontalHeader.end(); ++itr)
    {
        slstHeader << itr.value();
    }

    // ����Դ
    QList<QList<QVariant> > lstData;
    //lstData.append(QList<QVariant>() << "aa" << "bb");
    CFileOperationManager oTmpFileMananger;
    if (!oTmpFileMananger.writeExcelFile(strFileName, slstHeader, lstData))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(trFormString(cstTips));
        msgBox.setText(trFormString(cstExportFailed));
        msgBox.exec();
        return;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(trFormString(cstTips));
    msgBox.setText(trFormString(cstExportSuccess));
    msgBox.exec();
}
