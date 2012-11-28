#include "SFMLView.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


 void SFMLVIEW::resizeEvent ( QResizeEvent * event )
 {
    CurrentView.SetFromRect(sf::FloatRect(0,0,event->size().width(),event->size().height()));
    SetView(CurrentView);
 };

void SFMLVIEW::OnInit()
{
    setWindowTitle(MyFile.Name);

    DAT Dat(DatFileName);

    if (Dat.Load(&Img,MyFile))
    {
        Spr.SetImage(Img);
        Type=_IMAGE;
        resize(Spr.GetSize().x,Spr.GetSize().y);
        Spr.SetPosition(0,0);
        Dat.Close();

    }
    else if(Dat.Load(&Mus,MyFile))
    {
        Mus.Play();
        Type=_MUSIC;
        Dat.Close_save();
    }
    else
    {
        Type=_AUTRE;
        std::cerr<<"error"<<std::endl;
    }
};

void SFMLVIEW::OnUpdate()
{
    if (Type==_IMAGE)
        Draw(Spr);
    else if(Type == _MUSIC)
    {
        //dessiner les boutons...
    }
};
