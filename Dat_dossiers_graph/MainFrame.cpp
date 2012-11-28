#include "MainFrame.hpp"



using namespace std;
using namespace sf;

int NB_PAR_LIGNE=0;

 void MAINFRAME::OnInit()
 {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));

    menu= new QMenu( this );
    _Voir=new QAction("Voir",this);
    menu->addAction(_Voir);
    QObject::connect(_Voir,SIGNAL(triggered()),this,SLOT(ShowItem()));

    Viewer=NULL;


    NB_PAR_LIGNE=(size().width()-ESPACE_ICON)/(SIZE_ICON+ESPACE_ICON);

    DatFile.Open("data/icones.dat");

    DatFile.Load(&Folder_Base_I,"icones/DIR.png");
    Folder_Base_S.SetImage(Folder_Base_I);

    DatFile.Load(&Folder_Image_I,"icones/DIR.png");
    Folder_Image_S.SetImage(Folder_Image_I);

    DatFile.Load(&Folder_Song_I,"icones/DIR.png");
    Folder_Song_S.SetImage(Folder_Song_I);

    DatFile.Load(&File_Base_I,"icones/FILE.png");
    File_Base_S.SetImage(File_Base_I);

    DatFile.Load(&File_Image_I,"icones/PNG.png");
    File_Image_S.SetImage(File_Image_I);

    DatFile.Load(&File_Song_I,"icones/OGG.png");
    File_Song_S.SetImage(File_Song_I);

    DatFile.Load(&Selector_I,"icones/anim.png");

    int Image_X=(Selector_I.GetWidth())/16;
    int Image_y=(Selector_I.GetHeight());

    for (int i=0;i<16;++i)
        Selector_A.PushFrame(Frame(&Selector_I,Rect<int>(Image_X*(i-1), 0, Image_X*i, Image_y)));

    Selector_S.SetAnim(&Selector_A);
    Selector_S.SetLoop(true);
    Selector_S.Resize(SIZE_ICON+ESPACE_ICON,SIZE_ICON/3);
    Selector_S.SetPosition(ESPACE_ICON,ESPACE_ICON);
    Selector_S.SetCenter(ESPACE_ICON/2.0-3,-SIZE_ICON*2.0/3.0);
    Selector_S.Play();

    Close();

    //OpenFile("data/icones.dat");

 };

 void MAINFRAME::OnUpdate()
 {
     Clear();
     if (OPEN)
     {
        sf::Event Event;
        while(GetEvent(Event))
        {
            if (Event.Type ==Event::MouseMoved)
            {
                    selection=GetCaseSelection(Event.MouseMove.X,Event.MouseMove.Y);
                    Selector_S.SetPosition(Files_Icons[selection].Sprite.GetPosition());
            }
        }


        Selector_S.anim(GetFrameTime());
        Draw(Selector_S);

        const unsigned int _size=Files_Icons.size();

        for(unsigned int i=0;i<_size;++i)
            Files_Icons[i].Draw(this);
     }
 };

 void MAINFRAME::MajFiles()
 {
    int nb_files=0,nb_dir=0;
    EmitNextFile(CurrentDirectory);

    const unsigned int _size=Files.size();

    Files_Icons.clear();

    float x=ESPACE_ICON,y=ESPACE_ICON;

    for(unsigned int i=0;i<_size;++i)
    {
        Icons icon;
        Files_Icons.push_back(icon);

        string ext=Files[i].Name;

        if (Files[i].Type== _FILE) //si c'est un fichier
        {
            unsigned int point=ext.find_last_of('.');

            if(point<ext.length()) // si il a une extention
            {
                ext=ext.substr(point,ext.length());

                if(!ext.compare(".png"))
                    Files_Icons[i].Sprite=File_Image_S;
                else if(!ext.compare(".ogg"))
                    Files_Icons[i].Sprite=File_Song_S;
                else
                    Files_Icons[i].Sprite=File_Base_S;
                    //MP3 MPG/MPEG GIF jpeg/jpg PDF PNG
                    //txt,odt,
                    //ogg
            }
            else
                Files_Icons[i].Sprite=File_Base_S;
            nb_files++;
        }
        else //si c'est un dossier
        {
            Files_Icons[i].Sprite=Folder_Base_S;
            nb_dir++;
        }


        //on met son nom
        Files_Icons[i].Resize();
        Files_Icons[i].SetName(Files[i].Name);
        //mettre la position
        Files_Icons[i].SetPosition(x,y);

        x+=ESPACE_X;
        if(x+ESPACE_X>size().width())
        {
            x=ESPACE_ICON;
            y+=ESPACE_Y;
        }
    }
    EmiteNbFiles(nb_files);
    EmitNbDir(nb_dir);

    int ligne=Files_Icons.size()/NB_PAR_LIGNE+1;
    float size_ligne=ESPACE_ICON+ligne*ESPACE_Y;

    if (size_ligne>SizeMini.height())
        resize(QSize(SizeMini.width(),size_ligne));
    else
        resize(SizeMini);

    selection=0;
    Selector_S.SetPosition(Files_Icons[selection].Sprite.GetPosition());
 };

int MAINFRAME::GetCaseSelection(float x,float y)
{
    int res= (int((y-ESPACE_ICON)/ESPACE_Y)*NB_PAR_LIGNE) + int((x-ESPACE_ICON)/ESPACE_X);
    unsigned int _size=Files_Icons.size()-1;
    return ((res>_size)? _size: ((res>0)? res:0));
};

bool PressedLeft=false,PressedRight=false;

void MAINFRAME::mousePressEvent ( QMouseEvent * event ) //permet de gérer les evenement souris
{
    if (OPEN)
    {
        selection=GetCaseSelection(event->pos().x(),event->pos().y());
        Selector_S.SetPosition(Files_Icons[selection].Sprite.GetPosition());

        if (event->button()==Qt::LeftButton)
            PressedLeft=true;
        if (event->button()==Qt::RightButton)
            PressedRight=true;
    }
};

void MAINFRAME::mouseReleaseEvent (QMouseEvent *event)        //permet de gerer les evenemetn souris lors du relachement
{
    if (OPEN)
    {
        selection=GetCaseSelection(event->pos().x(),event->pos().y());
        Selector_S.SetPosition(Files_Icons[selection].Sprite.GetPosition());
        if (event->button()==Qt::LeftButton  )
        {
            PressedLeft=false;
            _next();
        }

        if (event->button()==Qt::RightButton)
        {
            PressedRight=false;
        }
    }
};

void MAINFRAME::mouseMoveEvent ( QMouseEvent * event )        //permet de gere les evenement souris lors de son deplacement
{

};

void MAINFRAME::keyPressEvent (QKeyEvent * event)             //permet de gere les evenement clavier (raccourcis)
{

};

void MAINFRAME::keyReleaseEvent ( QKeyEvent * event )         //permet de gere les evenement clavier (stop des raccourcis
{

};
int c=-1;

void MAINFRAME::ShowContextMenu(QPoint pos)
{
    c=-1;
    if (OPEN && Files.size()>0 && Files[selection].Type==_FILE)
    {
        c=GetCaseSelection(pos.x(),pos.y());
        menu->exec(mapToGlobal(pos));
    }
};

void MAINFRAME::ReciveOffset(long int offset)
{
    if(offset>0)
    {
        Files=DatFile.GetFilesIn(offset);
        MajFiles();
    }
};

void MAINFRAME::_next()
{
    if(Files.size()>0 && Files[selection].Type==_DIRECTORY)
    {
        CurrentDirectory=Files[selection];
       // Files.clear();
        Files=DatFile.GetFilesIn(Files[selection].Offset);
        MajFiles();

    }
    else if(Files.size()>0 && Files[selection].Type==_FILE)
    {
        //on ouvre le fichier
    }
};

 void MAINFRAME::resizeEvent ( QResizeEvent * event )
 {
    CurrentView.SetFromRect(sf::FloatRect(0,0,event->size().width(),event->size().height()));
    SetView(CurrentView);
 };

void MAINFRAME::OpenFile(QString s)
{
    Close();
    int nb_files=0,nb_dir=0;
    EmiteNbFiles(nb_files);
    EmitNbDir(nb_dir);
    SetFile(s.toStdString().c_str());
};

void MAINFRAME::Close()
{
     CurrentDirectory=File_Graph("/",sizeof(DATHeader),_DIRECTORY);
     DatFileName="";
     DatFile.Close();
     OPEN=false;
     emit EmitClose();
};

void MAINFRAME::ShowItem()
{
    if (c>=0)
    {
        if(Viewer != NULL)
            delete Viewer;
        Viewer=new SFMLVIEW (0,DatFileName,Files[c]);
        Viewer->open();
    }
};

void MAINFRAME::SetFile(const char* FileName)
{
    DatFile.Close();
    DatFileName=FileName;
    DatFile.Open(FileName);
    Files=DatFile.GetFilesIn();
    MajFiles();
    OPEN=true;
};

