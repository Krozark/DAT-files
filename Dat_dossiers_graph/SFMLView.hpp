#ifndef SFMLVIEW_HPP
#define SFMLVIEW_HPP

#include "QSFMLCanvasDialog.hpp"
#include <QtGui>

#include "Structures.hpp"
#include "DAT.hpp"

#include <iostream>

class SFMLVIEW : public QSFMLCANVASDIALOG
{
    Q_OBJECT

    public:
    SFMLVIEW (QWidget* Parent,std::string DatName,File_Graph &file): QSFMLCANVASDIALOG(Parent), MyFile(File_Graph((char*)"",0,_FILE))
    {
        MyFile=file;
        DatFileName=DatName;
    };

    protected:
        void resizeEvent ( QResizeEvent * event );

    private:
        void OnInit();
        void OnUpdate();

        sf::View CurrentView;

        File_Graph MyFile;
        std::string DatFileName;

        enum {_MUSIC=0,_IMAGE,_AUTRE}Type;

        sf::Image Img;
        sf::Sprite Spr;
        sf::Music Mus;

};



#endif
