#include "CCurveGraphicsItem.h"

#include "QtGui/QPainter"
#include "QtWidgets/QStyleOptionGraphicsItem"
#include "QtWidgets/QGraphicsSceneHoverEvent"
#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QGraphicsScene"
#include "QtGui/QKeyEvent"


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
{
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setFlags(ItemIsFocusable);
    //setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);


    // ��ʼ��С
    m_itemRectF = QRectF(2, 2, 400, 300);
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
QPointF& CCurveGraphicsItem::mapToAxis(QPointF& point) const
{
    // ������ģ�0,0����
    qreal x = m_itemRectF.bottomLeft().x() + m_iOffset;
    qreal y = m_itemRectF.bottomLeft().y() - m_iOffset;

    if (m_realX != 0)
    {
        qreal px = point.x() / m_realX * m_realXLength;
        point.setX(px + x);
    }
    if (m_realY != 0)
    {
        qreal py = point.y() / m_realY * m_realYLength;
        point.setY(y - py);
    }

    return point;
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

    // x������
    painter->drawLine(m_itemRectF.bottomLeft() + QPointF(m_iOffset, -m_iOffset), m_itemRectF.bottomRight() + QPointF(-m_iOffset, -m_iOffset));
    // ÿ���̶�ֵ
    m_realX = (m_dbXAxisMax - m_dbXAxisMin) / m_iXTicksCount;
    //m_realX = floor(m_realX*10 + 0.5) / 10;
    // ÿ���̶ȳ���
    m_realXLength = ((m_itemRectF.bottomRight().x() - m_iOffset) - (m_itemRectF.bottomLeft().x() + m_iOffset)) / m_iXTicksCount;

    for (int i = 0; i < m_iXTicksCount; ++i)
    {
        painter->drawLine(m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1), m_itemRectF.bottomLeft().y() - m_iOffset, m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1), m_itemRectF.bottomLeft().y() - m_iOffset - 5);
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1) - 5, m_itemRectF.bottomLeft().y() - m_iOffset + 10, QString::number(m_realX*(i + 1)));
    }
    painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 5, m_itemRectF.bottomLeft().y() - m_iOffset + 10, QString::number(m_dbXAxisMin));


    // y ������
    painter->drawLine(m_itemRectF.bottomLeft() + QPointF(m_iOffset, -m_iOffset), m_itemRectF.topLeft() + QPointF(m_iOffset, m_iOffset));
    // ÿ���̶�ֵ
    m_realY = (m_dbYAxisMax - m_dbYAxisMin) / m_iYTicksCount;
    // ÿ���̶ȳ���
    m_realYLength = ((m_itemRectF.bottomLeft().y() - m_iOffset) - (m_itemRectF.topLeft().y() + m_iOffset)) / m_iYTicksCount;
    for (int i = 0; i < m_iYTicksCount; ++i)
    {
        painter->drawLine(m_itemRectF.bottomLeft().x() + m_iOffset, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1), m_itemRectF.bottomLeft().x() + m_iOffset + 5, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1));
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 20, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1) + 5, QString::number(m_realY*(i + 1)));
    }


    // ��������ͼ
    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
    QPointF p1(0, 0); //mapToAxis(p1);
    QPointF p2(0.2, 0.2); //mapToAxis(p2);
    painter->drawLine(mapToAxis(p1), mapToAxis(p2));
    painter->drawLine(mapToAxis(QPointF(0.2, 0.2)), mapToAxis(QPointF(0.3, 0.1)));
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
    QGraphicsItem::hoverEnterEvent(event);
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
