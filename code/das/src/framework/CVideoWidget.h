/*
* File    : CVideoWidget.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : 
*
* Note    :
*/

#ifndef CVideoWidget_h
#define CVideoWidget_h


#include "CCustomWidgetBase.h"
#include "CGraphicsView.h"

#include "type.h"

class QLabel;


class CVideoWidget : public CCustomWidgetBase
{
    Q_OBJECT

public:
    explicit CVideoWidget(QWidget* parent = 0);
    ~CVideoWidget();

public:
    void setID(int iID) { m_iID = iID; };
    int getID() { return m_iID; };
    void setTitle(const QString& strTitle);
    QString getTitle() { return m_strTitle; };

    void setItemAttr(ItemAttribute_t itemAttr);
    ItemAttribute_t getItemAttr();

    void setView(CGraphicsView* pView) { m_pView = pView; };

    void updateFrame();
    
private:
    QLabel* m_labelTitle;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseDoubleClickEvent(QMouseEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

    virtual ITEMTYPE type();

private:
    int m_iID;
    QString m_strTitle;
    ItemAttribute_t m_tItemAttr;
    bool m_bFullscreenFlag;
    bool m_bFlag;   // test video 

private:
    int m_iLastX;
    int m_iLastY;
    int m_iLastWidth;
    int m_iLastHeight;
    CGraphicsView* m_pView;             // ��ȡView���ڴ�С 
};


#endif // CVideoWidget_h

