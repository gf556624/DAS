/*
* File    : CTableView.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : table��ؼ�
*
* Note    :
*/


#ifndef CTABLEVIEW_H
#define CTABLEVIEW_H


#include "CCustomWidgetBase.h"
#include "QtCore/QMap"


class CFrozenTableView;
class QStandardItemModel;

class CTableView : public CCustomWidgetBase
{
    Q_OBJECT

public:
    explicit CTableView(QWidget* parent = 0);
    ~CTableView();

    void setHorizontalHeader(QMap<QString, QString>& mapHeader);
    void setTimeRange(const QString& strStart, const QString& strEnd);

private:
    void initLayout();

protected:


private:
    CFrozenTableView* m_pTableView;
    QStandardItemModel* m_pModel;

    QMap<QString, QString> m_mapHorizontalHeader;  // ˮƽ��ͷ <�ֶΣ� ��ʾ����>
    QString m_strStartTime;
    QString m_strEndTime;
};

#endif // CTABLEVIEW_H
