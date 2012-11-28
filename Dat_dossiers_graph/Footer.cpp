#include "Footer.hpp"
#include <sstream>

void FOOTER::ReciveNbFiles(int nb)
{
    NbFiles=nb;
    MAJ();
};

void FOOTER::ReciveNbDir(int nb)
{
    NbDir=nb;
    MAJ();
};


void FOOTER::OnInit()
{
    NbFiles=NbDir=0;
    Description.SetPosition(0,0);
    Description.SetColor(sf::Color::Green);
    Description.SetSize(15);
    MAJ();

};

void FOOTER::OnUpdate()
{
    Clear();
    Draw(Description);
    Display();
};

void FOOTER::MAJ()
{
    std::stringstream out;
    out<<NbFiles<<((NbFiles>1)? " Fichiers":" Fichier")<<",    "<<NbDir<<((NbDir>1)?" Repertoires":" Repertoire")<<",    Total: "<<NbFiles+NbDir;
    Description.SetText(out.str());
};

void FOOTER::resizeEvent ( QResizeEvent * event )
{
    //resize(QSize(event->size().width(),event->size().heigth);
    CurrentView.SetFromRect(sf::FloatRect(0,0,event->size().width(),event->size().height()));
    SetView(CurrentView);
};

