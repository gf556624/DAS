#include "CGraphicsScene.h"
#include "constant.h"

#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QMenu"

const char* cstRemove = "Remove";

CGraphicsScene::CGraphicsScene(QObject *parent)
: QGraphicsScene(parent)
{

}


CGraphicsScene::~CGraphicsScene()
{
}


void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }


    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


void CGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}


void CGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem* item = this->focusItem();
    if (item)
    {
        // �Ҽ��Ƴ�item
        QMenu menu;
        QAction *removeAction = menu.addAction(trFormString(cstRemove));
        connect(removeAction, &QAction::triggered, [=](){this->removeItem(item);});
        QAction *selectedAction = menu.exec(event->screenPos());
    }

    QGraphicsScene::contextMenuEvent(event);
}



