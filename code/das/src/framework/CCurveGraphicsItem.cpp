#include "CCurveGraphicsItem.h"

#include "QtGui/QPainter"
#include "QtWidgets/QStyleOptionGraphicsItem"
#include "QtWidgets/QGraphicsSceneHoverEvent"
#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QGraphicsScene"
#include "QtGui/QKeyEvent"
#include "QtWidgets/QGraphicsSceneContextMenuEvent"
#include "QtCore/QDebug"
#include "QtWidgets/QMenu"

#include "CLogManager.h"


static int index = 1;
CCurveGraphicsItem::CCurveGraphicsItem(QGraphicsItem * parent /*= 0*/)
    : QGraphicsItem(parent)
    , m_startPos(0, 0)
    , m_endPos(0, 0)
    , m_dragDir(NONE)
    , m_strStartTime("")
    , m_strEndTime("")
    , m_dbXAxisMin(0.0)
    , m_dbXAxisMax(1.0)
    , m_dbYAxisMin(0.0)
    , m_dbYAxisMax(1.0)
    , m_iXTicksCount(5)
    , m_iYTicksCount(5)
    , m_iOffset(50)
    , m_realScale(1.0)
    , m_realXDefault(1.0)
    , m_bEditFlag(false)
    , m_iType(Item_Chart),
    m_iChannel(1)
{
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setFlags(ItemIsFocusable);
    //setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);


    // ��ʼ��С
    m_itemRectF = QRectF(2, 2, 400, 300);
    m_strTitle = QString(tr("chart %1")).arg(index++);
    m_lstLines.clear();

    m_line = QLine(0, 0, 0, 0);
}


CCurveGraphicsItem::~CCurveGraphicsItem()
{

}


void CCurveGraphicsItem::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_strStartTime = strStart;
    m_strEndTime = strEnd;
}

void CCurveGraphicsItem::setEnableEditMode(bool enable)
{ 
    m_bEditFlag = enable; 
    if (m_bEditFlag)
    {
        setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);
    }
    else
    {
        setFlags(ItemIsFocusable);
    }
}

QRectF CCurveGraphicsItem::boundingRect() const
{
    return QRectF(m_itemRectF.x() - 2, m_itemRectF.y() - 2, m_itemRectF.width() + 4, m_itemRectF.height() + 4);
}


// item��С������ק�ı�
void CCurveGraphicsItem::resetItemSize(const QRectF &rect)
{ 
    m_itemRectF = rect; 
    update(boundingRect());
}


// ��ת��Ϊ���������ĵ�
QPointF& CCurveGraphicsItem::mapToAxis(const QString& strKeyName, QPointF& point) const
{
    // ������ģ�0,0����
    qreal x = m_itemRectF.bottomLeft().x() + m_iOffset;
    qreal y = m_itemRectF.bottomLeft().y() - m_iOffset;

    if (m_realX != 0)
    {
        qreal px = (point.x() - m_dbXAxisMin) / m_realX * m_realXLength;
        point.setX(px + x);
    }
    if (m_realY != 0)
    {
        qreal ymin = m_mapAxis.value(strKeyName).m_dbYAxisMin;
        qreal ylen = m_mapAxis.value(strKeyName).m_realYLength;
        qreal yTmp = m_mapAxis.value(strKeyName).m_realY;
        qreal py = (point.y() - ymin) / yTmp * ylen;
        point.setY(y - py);
    }

    return point;
}



void CCurveGraphicsItem::setLines(const QList<CurveLine_t>& lstTmpVec)
{
    m_lstLines = lstTmpVec;
    CAxisData_t TmpAxisData;
    for (auto& TmpData : lstTmpVec)
    {
        TmpAxisData.m_dbYAxisMax = TmpData.m_realMax;
        TmpAxisData.m_dbYAxisMin = TmpData.m_realMin;
        TmpAxisData.m_realY = (TmpData.m_realMax - TmpData.m_realMin) / m_iYTicksCount;;
        TmpAxisData.m_realYLength = ((m_itemRectF.bottomLeft().y() - m_iOffset) - (m_itemRectF.topLeft().y() + m_iOffset)) / m_iYTicksCount;
        m_mapAxis[TmpData.m_strDisplayName] = TmpAxisData;
    }
    //scene()->update();
    update(boundingRect());

}


// ��������ͼ
void CCurveGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0*/)
{
    Q_UNUSED(widget);

    if ((option->state & QStyle::State_Selected) == QStyle::State_Selected && m_bEditFlag)
    {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter->drawLine(m_itemRectF.topLeft(), m_itemRectF.topRight());
        painter->drawLine(m_itemRectF.topLeft(), m_itemRectF.bottomLeft());
        painter->drawLine(m_itemRectF.bottomRight(), m_itemRectF.bottomLeft());
        painter->drawLine(m_itemRectF.bottomRight(), m_itemRectF.topRight());

    }
    painter->drawRect(m_itemRectF);
    painter->fillRect(m_itemRectF, Qt::white);

    painter->drawLine(m_line);


    // title
    painter->drawText(m_itemRectF.topLeft() + QPointF(15, 15), "aaa");

    // x������
    painter->drawLine(m_itemRectF.bottomLeft() + QPointF(m_iOffset, -m_iOffset), m_itemRectF.bottomRight() + QPointF(-m_iOffset, -m_iOffset));
    // ÿ���̶�ֵ
    m_realX = (m_dbXAxisMax - m_dbXAxisMin) / m_iXTicksCount;
    //m_realX = floor(m_realX*10 + 0.5) / 10;
    // ÿ���̶ȳ���
    m_realXLength = ((m_itemRectF.bottomRight().x() - m_iOffset) - (m_itemRectF.bottomLeft().x() + m_iOffset)) / m_iXTicksCount;

    for (int i = 0; i < m_iXTicksCount; ++i)
    {
        painter->setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
        painter->drawLine(m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1), m_itemRectF.bottomLeft().y() - m_iOffset, m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1), m_itemRectF.topLeft().y() + m_iOffset /*- 5*/);
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
        QString strText = QDateTime::fromMSecsSinceEpoch(m_realX*(i + 1) + m_dbXAxisMin).toString("hh:mm:ss:zzz");
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1) - 5, m_itemRectF.bottomLeft().y() - m_iOffset + 10, strText/* QString::number(m_realX*(i + 1) + m_dbXAxisMin)*/);
    }
    QString strTextMin = QDateTime::fromMSecsSinceEpoch(m_dbXAxisMin).toString("hh:mm:ss:zzz");
    painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 5, m_itemRectF.bottomLeft().y() - m_iOffset + 10, strTextMin/*QString::number(m_dbXAxisMin)*/);

    // y ������
    painter->drawLine(m_itemRectF.bottomLeft() + QPointF(m_iOffset, -m_iOffset), m_itemRectF.topLeft() + QPointF(m_iOffset, m_iOffset));
    int iTmpLineCount = m_lstLines.count();
    // ÿ��ͼ��λ�ó���
    int iLength = ((m_itemRectF.bottomRight().x()) - m_iOffset * 2 - (m_itemRectF.bottomLeft().x() + m_iOffset * 2)) / iTmpLineCount;
    
    m_mutex.lock();
    for (int n = 0; n < iTmpLineCount; ++n)
    {
        //setYAxisRange(m_lstLines.at(n).m_realMin, m_lstLines.at(n).m_realMax);

        // ÿ���̶�ֵ
        //m_realY = (m_dbYAxisMax - m_dbYAxisMin) / m_iYTicksCount;
        m_realY = m_mapAxis[m_lstLines[n].m_strDisplayName].m_realY;
        // ÿ���̶ȳ���
        //m_realYLength = ((m_itemRectF.bottomLeft().y() - m_iOffset) - (m_itemRectF.topLeft().y() + m_iOffset)) / m_iYTicksCount;
        m_realYLength = m_mapAxis[m_lstLines[n].m_strDisplayName].m_realYLength;
        m_dbYAxisMin = m_mapAxis[m_lstLines[n].m_strDisplayName].m_dbYAxisMin;
        m_dbYAxisMax = m_mapAxis[m_lstLines[n].m_strDisplayName].m_dbYAxisMax;
        for (int i = 0; i < m_iYTicksCount; ++i)
        {
            painter->setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
            painter->drawLine(m_itemRectF.bottomLeft().x() + m_iOffset, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1), m_itemRectF.bottomRight().x() - m_iOffset/* + 5*/, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1));
            painter->setPen(QPen(m_lstLines.at(n).m_color, 1, Qt::SolidLine));
            painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 20, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1) - n * 10 + 10, QString::number(m_realY*(i + 1) + m_dbYAxisMin));
        }
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 20, m_itemRectF.bottomLeft().y() - m_iOffset - n * 10 + 10, QString::number(m_dbYAxisMin));

        // ͼ��
        painter->fillRect(m_itemRectF.bottomLeft().x() + m_iOffset * 2 + iLength*n, m_itemRectF.bottomLeft().y() - 20, 5, 5, m_lstLines.at(n).m_color);
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset * 2 + iLength*n + 15, m_itemRectF.bottomLeft().y() - 15, m_lstLines.at(n).m_strDisplayName);

        // ��������ͼ
        painter->setPen(QPen(m_lstLines.at(n).m_color, 1, Qt::SolidLine));
        painter->drawLines(m_lstLines[n].m_vecPoints);
    }
    m_mutex.unlock();
}


void CCurveGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (!isSelected() && m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
        m_dragDir = NONE;
        return QGraphicsItem::hoverEnterEvent(event);
    }

    if (!m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
        return QGraphicsItem::hoverEnterEvent(event);
    }

    QPointF pos1 = event->scenePos();
    QPointF LeftTopPointF = this->scenePos() + QPointF(m_itemRectF.x(), m_itemRectF.y());
    QPointF RightBottomPointF = this->scenePos() + QPointF(m_itemRectF.x() + m_itemRectF.width(), m_itemRectF.y() + m_itemRectF.height());


    if ((event->pos().x() < 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = LEFTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = RIGHTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = RIGHTBOTTOM;
    }
    else if ((event->pos().x() < 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = LEFTBOTTOM;
    }
    else if ((pos1.y() >= LeftTopPointF.y() - 2)
        && (pos1.y() <= LeftTopPointF.y() + 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = UP;
    }
    else if ((pos1.y() <= RightBottomPointF.y() + 2)
        && (pos1.y() >= RightBottomPointF.y() - 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = DOWN;
    }
    else if ((pos1.x() >= LeftTopPointF.x() - 2)
        && (pos1.x() <= LeftTopPointF.x() + 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = LEFT;
    }

    else if ((pos1.x() <= RightBottomPointF.x() + 2)
        && (pos1.x() >= RightBottomPointF.x() - 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = RIGHT;
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
        m_dragDir = NONE;
    }

    QGraphicsItem::hoverEnterEvent(event);
}


void CCurveGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!isSelected() && m_bEditFlag) 
    {
        setCursor(Qt::ArrowCursor);
        m_dragDir = NONE;
        return QGraphicsItem::hoverEnterEvent(event);
    }

    if (!m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
        if (event->pos().x() >= m_itemRectF.bottomLeft().x() + m_iOffset && event->pos().x() <= m_itemRectF.bottomRight().x() - m_iOffset
            && event->pos().y() >= m_itemRectF.topLeft().y() + m_iOffset && event->pos().y() <= m_itemRectF.bottomLeft().y() - m_iOffset)
        {
            m_line.setLine(event->pos().x(), m_itemRectF.topLeft().y() + m_iOffset, event->pos().x(), m_itemRectF.bottomLeft().y() - m_iOffset);
        }
        else
        {
            m_line.setLine(0, 0, 0, 0);
        }
        return QGraphicsItem::hoverEnterEvent(event);
    }

    m_line.setLine(0, 0, 0, 0);

    QPointF pos1 = event->scenePos();
    QPointF LeftTopPointF = this->scenePos() + QPointF(m_itemRectF.x(), m_itemRectF.y());
    QPointF RightBottomPointF = this->scenePos() + QPointF(m_itemRectF.x() + m_itemRectF.width(), m_itemRectF.y() + m_itemRectF.height());


    if ((event->pos().x() < 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = LEFTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = RIGHTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = RIGHTBOTTOM;
    }
    else if ((event->pos().x() < 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = LEFTBOTTOM;
    }
    else if ((pos1.y() >= LeftTopPointF.y() - 2)
        && (pos1.y() <= LeftTopPointF.y() + 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = UP;
    }
    else if ((pos1.y() <= RightBottomPointF.y() + 2)
        && (pos1.y() >= RightBottomPointF.y() - 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = DOWN;
    }
    else if ((pos1.x() >= LeftTopPointF.x() - 2)
        && (pos1.x() <= LeftTopPointF.x() + 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = LEFT;
    }

    else if ((pos1.x() <= RightBottomPointF.x() + 2)
        && (pos1.x() >= RightBottomPointF.x() - 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = RIGHTBOTTOM;
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
        m_dragDir = NONE;
    }

    QGraphicsItem::hoverMoveEvent(event);
}


void CCurveGraphicsItem::hoverLeaveEnvet(QGraphicsSceneHoverEvent *event)
{
    if (m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverLeaveEvent(event);
}


void CCurveGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragDir != NONE && m_bEditFlag)
    {
        m_startPos = event->scenePos();
    }

    QGraphicsItem::mousePressEvent(event);
}


void CCurveGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragDir == NONE)
        return  QGraphicsItem::mouseMoveEvent(event);

    if (!m_bEditFlag)
    {
        return  QGraphicsItem::mouseMoveEvent(event);
    }

    QPointF dis;
    dis = event->scenePos() - m_startPos;
    m_startPos = event->scenePos();

    switch (m_dragDir)
    {
    case NONE:
        break;
    case LEFT:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() - dis.x(), m_itemRectF.height());
        this->resetItemSize(tempRect);
        this->moveBy(dis.x(), 0);
        break;
    }
    case RIGHT:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() + dis.x(), m_itemRectF.height());
        this->resetItemSize(tempRect);
        this->moveBy(0, 0);
        scene()->update();
        break;
    }
    case UP:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width(), m_itemRectF.height() - dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, dis.y());
        break;
    }
    case DOWN:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width(), m_itemRectF.height() + dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, 0);
        scene()->update();
        break;
    }
    case LEFTTOP:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() - dis.x(), m_itemRectF.height() - dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(dis.x(), dis.y());
        break;
    }
    case RIGHTTOP:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() + dis.x(), m_itemRectF.height() - dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, dis.y());
        scene()->update();
        break;
    }
    case LEFTBOTTOM:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() - dis.x(), m_itemRectF.height() + dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(dis.x(), 0);
        break;
    }
    case RIGHTBOTTOM:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() + dis.x(), m_itemRectF.height() + dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, 0);
        scene()->update();
        break;
    }
    default:
        break;
    }
}


void CCurveGraphicsItem::keyPressEvent(QKeyEvent * event)
{
    if (m_bEditFlag)
    {
        return QGraphicsItem::keyPressEvent(event);
    }

    if (event->key() == Qt::Key_Plus)
    {
        if (m_dbXAxisMax > m_realXDefault*0.4)
        {
            m_dbXAxisMax -= m_dbXAxisMax*0.20;
           // m_iXTicksCount += m_iXTicksCount*0.5;
            update();
        }

    }
    else if (event->key() == Qt::Key_Minus)
    {
        
        if (m_dbXAxisMax < m_realXDefault*1.6)
        {
            m_dbXAxisMax += m_dbXAxisMax*0.20;
           // m_iXTicksCount -= m_iXTicksCount*0.5;
            update();
        }
    }
    
    QGraphicsItem::keyPressEvent(event);
}


void CCurveGraphicsItem::OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
	CCustomDateTime* pTime, void* pCustomData)
{
    QList<CurveLine_t> lstCanData;
    CurveLine_t canData;

    //char buf[256];
    m_mutex.lock();
    CCanData* pData = (CCanData*)buffer;
    for (int i = 0; i < length; i++)
    {
        canData.m_strDisplayName = QString::fromLocal8Bit(pData[i].m_pCanItem->m_DispName.c_str());
        canData.m_strName = QString::fromStdString(pData[i].m_pCanItem->m_Name);
        canData.m_strValue = QString::number(pData[i].m_Value, 'f', 2);
        canData.m_realMax = pData[i].m_pCanItem->m_maxValue;
        canData.m_realMin = pData[i].m_pCanItem->m_minValue;
        if (m_lstLines.contains(canData))
        {
            for (auto& TmpData : m_lstLines)
            {
                if (TmpData.m_strDisplayName == canData.m_strDisplayName)
                {
					QDateTime dt(QDate(pTime->year, pTime->month, pTime->mday), QTime(pTime->hour, pTime->min, pTime->sec, pTime->msec));
					quint64 x = dt.toMSecsSinceEpoch();

                    QPointF TmpPoint(x, pData[i].m_Value);
                    if (TmpData.m_vecPoints.count() > 1)
                    {
                        TmpData.m_vecPoints.append(TmpData.m_vecPoints.last());
                    }
                    TmpData.m_vecPoints.append(mapToAxis(TmpData.m_strDisplayName, TmpPoint));
                    break;
                }
            }
        }
    }
    m_mutex.unlock();

    //scene()->update();
    update(boundingRect());

    CLogManager::getInstance()->log(eLogDebug, "CCurveGraphicsItem::OnMedia", "------CCurveGraphicsItem::OnMedia");
}




