#include "das.h"
#include "constant.h"
#include "image.h"

#include "QtWidgets/QMenu"
#include "QtWidgets/QMenuBar"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QStatusBar"
#include "QtCore/QTimer"


// ��������ؼ��ֳ��� 
const char* cstDictDas = "DAS";

const char* cstFile = "File";
const char* cstEdit = "Edit";
const char* cstView = "View";
const char* cstTool = "Tool";
const char* cstHelp = "Help";

const char* cstDictOpenPath = "Open source data path";
const char* cstDictExit = "Exit system";
const char* cstDictPlay = "Play";
const char* cstDictFind = "Find";
const char* cstDictFullScreen = "Full screen";

const char* cstDictLanuage = "Lanuage";
//const char* cstDictZhCn = "Simplified chinese";
//const char* cstDictEn = "English";
const char* cstDictAbout = "About";

const char* cstOperatorToolBar = "Operator tool bar";
const char* cstModuleToolBar = "Module tool bar";

const char* cstDictTimelineModule = "Timeline";
const char* cstDictVideoModule = "Video";
const char* cstDictCurveModule = "Curve";
const char* cstDictTableModule = "Table";


DAS::DAS(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setWindowTitle(trFormString(cstDictDas));
    //setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(IMG_LOGO));
    resize(800, 600);

    setLayout();

    m_pGraphicsView = new CGraphicsView(this);
    this->setCentralWidget(m_pGraphicsView);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnUpdate()));
    m_pTimer->start(40);

    m_pWidget = new QWidget;
    m_pWidget->setGeometry(100, 100, 500, 400);
    m_pWidget->show();

    m_bFlag = false;
}


DAS::~DAS()
{
    if (m_pGraphicsView != NULL)
    {
        delete m_pGraphicsView;
        m_pGraphicsView = NULL;
    }
}


void DAS::OnUpdate()
{
    if (m_bFlag)
    {
        QPalette pal;
        pal.setBrush(QPalette::Window, QBrush(QPixmap(IMG_ENGLISH).scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        m_pWidget->setPalette(pal);
        m_bFlag = false;
    }
    else
    {
        QPalette pal;
        pal.setBrush(QPalette::Window, QBrush(QPixmap(IMG_CHINESE).scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        m_pWidget->setPalette(pal);
        m_bFlag = true;
    }

    m_pWidget->update();
}


void DAS::setLayout()
{
    // �����ļ��˵��������ļ��˵��¼� 
    // ������Դ·�� 
    m_pActOpen = new QAction(trMenuString(cstDictOpenPath), this);
    m_pActOpen->setIcon(QIcon(IMG_OPEN));
    m_pActOpen->setShortcut(QKeySequence("Ctrl+O"));
    //m_pActOpen->setStatusTip("");
    connect(m_pActOpen, SIGNAL(triggered()), this, SLOT(OnOpen()));

    // �˳�ϵͳ 
    m_pActExit = new QAction(trMenuString(cstDictExit), this);
    m_pActExit->setIcon(QIcon(IMG_EXIT));
    m_pActExit->setShortcut(QKeySequence("Alt+F4"));
    connect(m_pActExit, SIGNAL(triggered()), this, SLOT(exit()));

    m_pMenuFile = new QMenu(trMenuString(cstFile), this);
    m_pMenuFile->addAction(m_pActOpen);
    m_pMenuFile->addAction(m_pActExit);

    // �����༭�˵��������༭�˵��¼� 
    m_pActPlay = new QAction(trMenuString(cstDictPlay), this);    // ���š���ͣ��ֹͣ�����࿪�� 
    m_pActPlay->setIcon(QIcon(IMG_PLAY));
    m_pActPlay->setShortcut(QKeySequence("Ctrl+F5"));
    connect(m_pActPlay, SIGNAL(triggered()), this, SLOT(OnPlay()));

    m_pActFind = new QAction(trMenuString(cstDictFind), this);    // ���� 
    m_pActFind->setIcon(QIcon(IMG_FIND));
    m_pActFind->setShortcut(QKeySequence("Ctrl+F"));
    connect(m_pActFind, SIGNAL(triggered()), this, SLOT(OnFind()));

    m_pMenuEdit = new QMenu(trMenuString(cstEdit), this);
    m_pMenuEdit->addAction(m_pActFind);

    // ������ͼ�˵���������ͼ�¼� 
    m_pActFullScreen = new QAction(trMenuString(cstDictFullScreen), this);   // ȫ�� 
    m_pActFullScreen->setIcon(QIcon(IMG_FULLSCREEN));
    m_pActFullScreen->setShortcut(QKeySequence("Alt+Enter"));
    connect(m_pActFullScreen, SIGNAL(triggered()), this, SLOT(OnFullScreen()));

    m_pMenuView = new QMenu(trMenuString(cstView), this);
    m_pMenuView->addAction(m_pActFullScreen);

    // �������߲˵����������߲˵��¼� 
    m_pActZhCn = new QAction(QStringLiteral("��������"), this);   // �������� 
    m_pActZhCn->setIcon(QIcon(IMG_ENGLISH));
    m_pActZhCn->setCheckable(true);
    m_pActZhCn->setChecked(true);
    m_pActEn = new QAction(QStringLiteral("English"), this);       // Ӣ�� 
    m_pActEn->setIcon(QIcon(IMG_CHINESE));
    m_pActEn->setCheckable(true);

    m_pMenuTool = new QMenu(trMenuString(cstTool), this);
    m_pMenuTool->addAction(m_pActPlay);
    m_pMenuLanuage = new QMenu(trMenuString(cstDictLanuage), m_pMenuTool);
    connect(m_pMenuLanuage, SIGNAL(triggered(QAction*)), this, SLOT(OnLanuageChanged(QAction*)));
    m_pMenuLanuage->addAction(m_pActZhCn);
    m_pMenuLanuage->addAction(m_pActEn);
    m_pMenuTool->addAction(m_pMenuLanuage->menuAction());

    // �������ڲ˵������������¼� 
    m_pActAbout = new QAction(trMenuString(cstDictAbout), this);   // ���� 
    m_pActAbout->setIcon(QIcon(IMG_ABOUT));
    connect(m_pActAbout, SIGNAL(triggered()), this, SLOT(OnAbout()));

    m_pMenuHelp = new QMenu(trMenuString(cstHelp), this);
    m_pMenuHelp->addAction(m_pActAbout);

    QMenuBar* pMenuBar = new QMenuBar(this);
    pMenuBar->addMenu(m_pMenuFile);
    pMenuBar->addMenu(m_pMenuEdit);
    pMenuBar->addMenu(m_pMenuView);
    pMenuBar->addMenu(m_pMenuTool);
    pMenuBar->addMenu(m_pMenuHelp);
    this->setMenuBar(pMenuBar);

    // ����������(�����������������������  
    m_pOperatorToolBar = this->addToolBar(trMenuString(cstOperatorToolBar));
    m_pOperatorToolBar->addAction(m_pActOpen);
    m_pOperatorToolBar->addAction(m_pActFind);
    m_pOperatorToolBar->addSeparator();
    m_pOperatorToolBar->addAction(m_pActPlay);
    m_pOperatorToolBar->addSeparator();
    m_pOperatorToolBar->addAction(m_pActFullScreen);

    m_pActTimeline = new QAction(trMenuString(cstDictTimelineModule), this);   // ʱ�������  
    m_pActTimeline->setIcon(QIcon(IMG_TIMELINE));
    connect(m_pActTimeline, SIGNAL(triggered()), this, SLOT(OnTimeline()));

    m_pActVideo = new QAction(trMenuString(cstDictVideoModule), this);   // ��Ƶ���  
    m_pActVideo->setIcon(QIcon(IMG_VIDEO));
    connect(m_pActVideo, SIGNAL(triggered()), this, SLOT(OnVideo()));

    m_pActCurve = new QAction(trMenuString(cstDictCurveModule), this);   // ����ͼ���  
    m_pActCurve->setIcon(QIcon(IMG_CURVE));
    connect(m_pActCurve, SIGNAL(triggered()), this, SLOT(OnCurve()));

    m_pActTable= new QAction(trMenuString(cstDictTableModule), this);   // ������  
    m_pActTable->setIcon(QIcon(IMG_TABLE));
    connect(m_pActTable, SIGNAL(triggered()), this, SLOT(OnTable()));

    m_pModuleToolBar = this->addToolBar(trMenuString(cstModuleToolBar));
    m_pModuleToolBar->addAction(m_pActTimeline);
    m_pModuleToolBar->addAction(m_pActVideo);
    m_pModuleToolBar->addAction(m_pActCurve);
    m_pModuleToolBar->addAction(m_pActTable);

    // ����״̬�� 
    this->setStatusBar(new QStatusBar(this));
}


void DAS::OnOpen()
{

}


void DAS::OnPlay()
{

}


void DAS::OnFind()
{

}


void DAS::OnFullScreen()
{

}


void DAS::OnLanuageChanged(QAction* pAction)
{
    if (pAction == m_pActZhCn)              // �������� 
    {
        m_pActEn->setChecked(false);
        m_pActZhCn->setChecked(true);
        if (m_translator.load(":/dict/das_zh_cn.qm"))
        {
            QApplication::installTranslator(&m_translator);
        }
    }
    else if (pAction == m_pActEn)           // Ӣ�� 
    {
        m_pActEn->setChecked(true);
        m_pActZhCn->setChecked(false);
        if (m_translator.load(":/dict/das_en.qm"))
        {
            QApplication::installTranslator(&m_translator);
        }
    }
}


void DAS::OnAbout()
{

}


void DAS::retranslate()
{
    m_pActOpen->setText(trMenuString(cstDictOpenPath));
    m_pActExit->setText(trMenuString(cstDictExit));
    m_pMenuFile->setTitle(trMenuString(cstFile));
    m_pActPlay->setText(trMenuString(cstDictPlay));    // ���š���ͣ��ֹͣ�����࿪�� 

    m_pActFind->setText(trMenuString(cstDictFind));

    m_pMenuEdit->setTitle(trMenuString(cstEdit));
    m_pActFullScreen->setText(trMenuString(cstDictFullScreen));
    m_pMenuView->setTitle(trMenuString(cstView));
    //m_pActZhCn->setText(trMenuString(cstDictZhCn));
    //m_pActEn->setText(trMenuString(cstDictEn));

    m_pMenuTool->setTitle(trMenuString(cstTool));
    m_pMenuLanuage->setTitle(trMenuString(cstDictLanuage));

    m_pActAbout->setText(trMenuString(cstDictAbout));
    m_pMenuHelp->setTitle(trMenuString(cstHelp));

    m_pOperatorToolBar->setWindowTitle(trMenuString(cstOperatorToolBar));
    m_pActTimeline->setText(trMenuString(cstDictTimelineModule));
    m_pActVideo->setText(trMenuString(cstDictVideoModule));
    m_pActCurve->setText(trMenuString(cstDictCurveModule));
    m_pActTable->setText(trMenuString(cstDictTableModule));
    m_pModuleToolBar->setWindowTitle(trMenuString(cstModuleToolBar));
}


void DAS::changeEvent(QEvent *ev)
{
    int iType = ev->type();
    switch (iType)
    {
    case QEvent::LanguageChange:
        retranslate();
        break;
    default:
        break;
    }

    QMainWindow::changeEvent(ev);
}

