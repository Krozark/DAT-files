#include "TotalFrame.hpp"


void TOTALFRAME::OnInit()
{
    setWindowTitle("Editeur de Fichiers DAT");
    Layout=new QGridLayout();

    Header=new HEADER(this);
    BreadCrumb=new BREADCRUMB(0,QPoint(0,0),QSize(size().width()-30,size().height()/10.0 -30));
    MainFrame=new MAINFRAME(0,QPoint(0,0),QSize(size().width()-38,size().height()/20.0*17-25));
    Footer=new FOOTER(0,QPoint(0,0),QSize(size().width()-30,size().height()/20.0 -30));


    QObject::connect(BreadCrumb,SIGNAL(EmitOffest(long int)),MainFrame,SLOT(ReciveOffset(long int)));
    QObject::connect(MainFrame,SIGNAL(EmitNextFile(File_Graph)),BreadCrumb,SLOT(ReciveFile(File_Graph)));

    QObject::connect(MainFrame,SIGNAL(EmiteNbFiles(int)),Footer,SLOT(ReciveNbFiles(int)));
    QObject::connect(MainFrame,SIGNAL(EmitNbDir(int)),Footer,SLOT(ReciveNbDir(int)));

    QObject::connect(Header,SIGNAL(FileToOpen(QString)),MainFrame,SLOT(OpenFile(QString)));
    QObject::connect(MainFrame,SIGNAL(EmitClose()),BreadCrumb,SLOT(Clear()));

    ScrollArea=new QScrollArea();
    ScrollArea->setWidget(MainFrame);

    int i=0;
    Layout->addWidget(BreadCrumb,i,0,2,1);i+=2;
    Layout->addWidget(ScrollArea,i,0,17,1);i+=17;
    Layout->addWidget(Footer,i,0,1,1);++i;

    QWidget* conteneur=new QWidget();
    conteneur->setLayout(Layout);
    setCentralWidget(conteneur);

    QPalette palette(this->palette());
    palette.setBrush(QPalette::Window,QColor(0,0,0,255));
    this->setPalette(palette);

};

void TOTALFRAME::OnUpdate()
{

};

