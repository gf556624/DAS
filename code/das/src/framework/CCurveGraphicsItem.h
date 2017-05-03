/*
* File    : CCurveGraphicsItem.h
*
* Date    : 2017-04-26
*
* Author  :
*
* Brief   : ����ͼ�ؼ�
*
* Note    :
*/


#ifndef CCURVE_GRAPHICSITEM_H
#define CCURVE_GRAPHICSITEM_H

#include "QtWidgets/QGraphicsItem"
#include "QtCore/QObject"
#include "type.h"
#include "CCustomWidgetBase.h"

typedef struct CURVE_LINE
{
    QColor m_color;
    QString m_strName;
    qreal m_realMin;
    qreal m_realMax;
    QVector<QPointF> m_vecPoints;

    CURVE_LINE()
    {
        m_color = Qt::black;
        m_strName = "";
        m_realMin = 0.0;
        m_realMax = 1.0;
        m_vecPoints.clear();
    }
}CurveLine;

class CCurveGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit CCurveGraphicsItem(QGraphicsItem * parent = 0);
    ~CCurveGraphicsItem();

    void setTimeRange(const QString& strStart, const QString& strEnd);
    void setXAxisTicksCount(const int& iCount) { m_iXTicksCount = iCount; }
    void setYAxisTicksCount(const int& iCount) { m_iYTicksCount = iCount; }
    void setXAxisRange(const qreal& dbMin, const qreal& dbMax) { m_dbXAxisMin = dbMin; m_dbXAxisMax = dbMax; m_realXDefault = m_dbXAxisMax; }
    void setYAxisRange(const qreal& dbMin, const qreal& dbMax) { m_dbYAxisMin = dbMin; m_dbYAxisMax = dbMax; }

    void setEnableEditMode(bool enable);
    void setTitle(const QString& strTitle);

    void setType(ITEMTYPE iType) { m_iType = iType; };
    void setLines(const QList<CurveLine>& lstTmpVec) { m_lstLines = lstTmpVec; }
    ITEMTYPE getType() { return m_iType; };


    QRectF boundingRect() const;
    void resetItemSize(const QRectF &rect);

private:
    QPointF& mapToAxis(QPointF& point) const;

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    virtual void hoverLeaveEnvet(QGraphicsSceneHoverEvent *event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent * event);

private:
    QRectF m_itemRectF;
    QPointF m_startPos;        // ��ק��ʼ��
    QPointF m_endPos;          // ��ק������
    DragDirection m_dragDir;   // ��ק����

    QString m_strStartTime;
    QString m_strEndTime;

    qreal m_dbXAxisMin;       //X��С�̶�
    qreal m_dbXAxisMax;      // X���̶�
    qreal m_realX;             // X��ÿ���̶ȴ�С
    qreal m_realXLength;        // X��ÿ���̶ȳ���

    qreal m_dbYAxisMin;       //Y��С�̶�
    qreal m_dbYAxisMax;      // Y���̶�
    qreal m_realY;              // Y ��ÿ���̶ȴ�С
    qreal m_realYLength;        // Y��ÿ���̶ȳ���

    int m_iXTicksCount;   // X��̶ȱ����
    int m_iYTicksCount;   // Y��̶ȱ����

    int m_iOffset;       // ��������߿���ƫ����

    qreal m_realScale;   // ���ű���

    qreal m_realXDefault;  // X�����ֵ��ʼֵ

    bool m_bEditFlag;    // �Ƿ��Ǳ༭ģʽ
    ITEMTYPE m_iType;

    QString m_strTitle;  // ͼ�α���

    QList<CurveLine> m_lstLines;

    QLine m_line;

signals:

};


#endif
