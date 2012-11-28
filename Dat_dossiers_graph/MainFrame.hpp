#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "Animated.hpp"

#include "QSFMLCanvas.hpp"

#include "SFMLView.hpp"

#include <QtGui>

#include "DAT.hpp"

//supportés
//ogg,wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64
//bmp, dds, jpg, png, tga, psd

//jpeg

//non supportés
//mp3 mp4 mpg, m4a, m3u wma
//avi flv wmv
//tiff tif
//bin cmd, bat exe
//c,cpp,h,hpp doc, js, log, php, html, py, sci, sce
// jar jav java
//zip, gz, tar, iso, rar, tgz 7z dat

class  MAINFRAME : public QSFMLCanvas
{
    Q_OBJECT
    public:

    ~MAINFRAME()
    {
        DatFile.Close();
    };

    MAINFRAME(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 60):
    QSFMLCanvas(Parent,Position,Size,FrameTime), SizeMini(Size), CurrentDirectory(File_Graph("/",sizeof(DATHeader),_DIRECTORY))
    {

    };

    void SetFile(const char* FileName);
    void _next();

    public slots:
        void ReciveOffset(long int);
        void OpenFile(QString);
        void Close();
        void ShowContextMenu(QPoint);
        void ShowItem();
        signals:
        void EmitNextFile(File_Graph);
        void EmiteNbFiles(int);
        void EmitNbDir(int);
        void EmitClose();

    protected:
        void resizeEvent ( QResizeEvent * event );
        void mousePressEvent ( QMouseEvent * event );       //permet de gere les evenement souris lors du click
        void mouseReleaseEvent (QMouseEvent *event);        //permet de gerer les evenemetn souris lors du relachement
        void mouseMoveEvent ( QMouseEvent * event );        //permet de gere les evenement souris lors de son deplacement
        void keyPressEvent (QKeyEvent * event);             //permet de gere les evenement clavier (raccourcis)
        void keyReleaseEvent ( QKeyEvent * event );         //permet de gere les evenement clavier (stop des raccourcis

    private:
        void OnInit();
        void OnUpdate();

        sf::Image Folder_Base_I,
                  Folder_Image_I,
                  Folder_Song_I,
                  File_Base_I,
                  File_Image_I,
                  File_Song_I,
                  Selector_I;

        sf::Sprite Folder_Base_S,
                   Folder_Image_S,
                   Folder_Song_S,
                   File_Base_S,
                   File_Image_S,
                   File_Song_S;
        Animated Selector_S;
        Anim Selector_A;


        std::vector<File_Graph> Files;
        std::vector<Icons> Files_Icons;
        int selection;

        inline int GetCaseSelection(float x,float y);

        DAT DatFile;
        std::string DatFileName;
        void MajFiles();
        File_Graph CurrentDirectory;

        const QSize SizeMini;
        sf::View CurrentView;

        QMenu *menu;
        QAction* _Voir;
        SFMLVIEW* Viewer;

        bool OPEN;
};
#endif

