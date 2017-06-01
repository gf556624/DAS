#include "CGraphicsScene.h"
#include "constant.h"
#include "CConfigCurveDlg.h"
#include "CCurveGraphicsItem.h"

#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QMenu"
#include "QtWidgets/QGraphicsItem"
#include "QtWidgets/QInputDialog"
#include "QtWidgets/QGraphicsProxyWidget"
#include "CVideoWidget.h"
#include "CTableView.h"
#include "CCurveGraphicsItem.h"

const char* cstRemove = "Remove";
extern const char* cstSelectCurve;
extern const char* cstOk;
extern const char* cstCancel;

CGraphicsScene::CGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_bEnable(false)
    , m_bPlay(false)
{

}


CGraphicsScene::~CGraphicsScene()
{
}


void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }
    QGraphicsItem* item = this->itemAt(mouseEvent->scenePos(), QTransform());
    if (!item)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return ;
    }
    item->setZValue(0.1);
    QList<QGraphicsItem*> lstItem = collidingItems(item);

    for (auto& TmpItem : lstItem)
    {
        TmpItem->setZValue(0);
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
    QGraphicsItem* item = this->itemAt(event->scenePos(), QTransform());

    if (item)
    {
        //item->stackBefore();
        // �Ҽ��Ƴ�item
        QMenu menu;
        QAction *removeAction = menu.addAction(trFormString(cstRemove));
        removeAction->setEnabled(m_bEnable);
        connect(removeAction, &QAction::triggered, [=](){this->removeItem(item);});

        QGraphicsProxyWidget* pWidget = nullptr;
        CCustomWidgetBase* pCustomItem = nullptr;
        ITEMTYPE iType = Item_None;
        if (item->isWidget())
        {
            pWidget = (QGraphicsProxyWidget*)item;
            pCustomItem = (CCustomWidgetBase*)pWidget->widget();
            iType = pCustomItem->type();
        }
       
        // ѡ������
        if (!item->isWidget())
        {
            QAction *curveAction = menu.addAction(trFormString(cstSelectCurve));
            curveAction->setEnabled(!m_bPlay);
            connect(curveAction, &QAction::triggered, [=](){
                CConfigCurveDlg TmpObj;
                TmpObj.setCheckedLines(static_cast<CCurveGraphicsItem*>(item)->getLines());
                if (TmpObj.exec())
                {
                    static_cast<CCurveGraphicsItem*>(item)->setLines(TmpObj.getCheckedLines());
                }
            });
        }

        // ����
        if (iType != Item_TimeAxis || !item->isWidget())         // time axis û��channel
        {
            QAction *propertyAction = menu.addAction(trFormString("Property"));
            propertyAction->setEnabled(!m_bPlay);
            connect(propertyAction, &QAction::triggered, [=](){
                QInputDialog oTmpDlg;
                oTmpDlg.setWindowFlags(oTmpDlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);
                oTmpDlg.setIntMinimum(0);
                oTmpDlg.setInputMode(QInputDialog::IntInput);
                oTmpDlg.setWindowTitle(trFormString("Property"));
                oTmpDlg.setLabelText(trFormString("Channel"));
                oTmpDlg.setOkButtonText(trFormString(cstOk));
                oTmpDlg.setCancelButtonText(trFormString(cstCancel));
                oTmpDlg.setIntValue(getItemChannel(item));
                if (oTmpDlg.exec())
                {
                    int iPreChannel = -1;
                    if (item->isWidget())
                    {
                        QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)item;
                        CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
                        ITEMTYPE iType = pCustomItem->type();
                        if (iType == Item_Video)         // video 
                        {
                            CVideoWidget *pVideoWidget = dynamic_cast<CVideoWidget*>(pCustomItem);
                            iPreChannel = pVideoWidget->getChannel();
                            pVideoWidget->setChannel(oTmpDlg.intValue());
                        }
                        else if (iType == Item_Table)           // table 
                        {
                            CTableView *pTableView = dynamic_cast<CTableView*>(pCustomItem);
                            iPreChannel = pTableView->getChannel();
                            pTableView->setChannel(oTmpDlg.intValue());
                        }
                    }
                    else // curve
                    {
                        iPreChannel = dynamic_cast<CCurveGraphicsItem*>(item)->getChannel();
                        dynamic_cast<CCurveGraphicsItem*>(item)->setChannel(oTmpDlg.intValue());
                    }

                    emit sigChannelChanged(item, iPreChannel);
                }
            });
        }

        QAction *selectedAction = menu.exec(event->screenPos());
    }

    QGraphicsScene::contextMenuEvent(event);
}

// ��ȡ���channel
int CGraphicsScene::getItemChannel(QGraphicsItem* item) const
{
    if (!item)
    {
        return 0;
    }
    int iChannel = 0;
    if (item->isWidget())
    {
        QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)item;
        CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
        ITEMTYPE iType = pCustomItem->type();
        if (iType == Item_Video)         // video 
        {
            CVideoWidget *pVideoWidget = dynamic_cast<CVideoWidget*>(pCustomItem);
            iChannel = pVideoWidget->getChannel();
        }
        else if (iType == Item_Table)           // table 
        {
            CTableView *pTableView = dynamic_cast<CTableView*>(pCustomItem);
            iChannel = pTableView->getChannel();
        }
    }
    else // curve
    {
        iChannel = dynamic_cast<CCurveGraphicsItem*>(item)->getChannel();
    }

    return iChannel;
}




