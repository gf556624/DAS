#ifndef CGraphicsScene_h
#define CGraphicsScene_h

#include "QtWidgets/QGraphicsScene"


class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CGraphicsScene(QObject *parent = 0);
    ~CGraphicsScene();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:

private slots:
};


#endif
