/*
* File    : CCustomWidgetBase.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : �Զ���ؼ�����
*
* Note    :
*/


#ifndef CCUSTOM_WIDGET_BASE_H
#define CCUSTOM_WIDGET_BASE_H


#include "QtWidgets/QFrame"
#include "type.h"


class CCustomWidgetBase : public QFrame
{
    Q_OBJECT

public:
    CCustomWidgetBase(QWidget* parent = 0);
    virtual ~CCustomWidgetBase() = 0;

public:
    virtual ITEMTYPE type() = 0;

private:
    void mouseRegion(const QPoint &cursorPoint);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

protected:
    bool m_bIsLeftPressDown;       // �Ƿ��������
    QPoint m_startPos;             // �϶���ʼ��
    QPoint m_endPos;               // �϶�������
    DragDirection m_dragDirection; // ��ק����
};


#endif // CCUSTOM_WIDGET_BASE_H
