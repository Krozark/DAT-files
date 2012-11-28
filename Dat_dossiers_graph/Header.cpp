#include "Header.hpp"
#include "lister.hpp"
#include "DAT.hpp"

#include <iostream>
#include <stdio.h>
using namespace std;

HEADER::HEADER (QMainWindow * Parent) : QObject(Parent)
{
    _New = new QAction("&Nouveau",Parent);
    _Quit= new QAction("&Quitter",Parent);
    _Open= new QAction("&Ouvrir",Parent);
    _AddF=new QAction("Ajouter Fichier",Parent);
    _AddD=new QAction("Ajouter Dossier",Parent);
    _Creat=new QAction("Generer le DAT",Parent);

    QMenu* M1=Parent->menuBar()->addMenu("&Fichier");
    M1->addAction(_New);
    M1->addAction(_Open);
    M1->addSeparator();
    M1->addAction(_Quit);

    QMenu* M2=Parent->menuBar()->addMenu("&Edition");
    M2->addAction(_AddF);
    M2->addAction(_AddD);
    M2->addAction(_Creat);


    QObject::connect(_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(_Open, SIGNAL(triggered()),this,SLOT(FileToOpen()));

    QObject::connect(_AddD,SIGNAL(triggered()),this,SLOT(DirectoryToAdd()));
    QObject::connect(_AddF,SIGNAL(triggered()),this,SLOT(FileToAdd()));
    QObject::connect(_Creat,SIGNAL(triggered()),this,SLOT(Creat()));
};

void HEADER::FileToOpen()
{
    QString File= QFileDialog::getOpenFileName(0,tr("Selectionez le fichier a ouvrir"),"./",tr("Fichier DAT (*.dat)"));
    emit FileToOpen(File);
};

void HEADER::DirectoryToAdd()
{
    QString DirBase=QFileDialog::getExistingDirectory(0,tr("Entrez le nom du dossier a ajouter"),("/home/"+string(getenv("USER"))).c_str(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::ReadOnly);

    std::vector<std::string> Files=LISTEUR::lister_rec(DirBase.toStdString().c_str(),NULL);

    string Dest=QInputDialog::getText(0,tr("Entrez le nom de la destination"),tr("Destination:"),QLineEdit::Normal,DirBase).toStdString();

    if (Dest.size()>0 && Dest[0]!='/')
        Dest="/"+Dest;

    for (unsigned int i=0,_size=Files.size(),_size_s=DirBase.toStdString().size();i<_size;++i)
    {
        string Source=Files[i];
        string Dest=Source.replace(Source.begin(),Source.begin()+_size_s,Dest);
        FilesToAdd.push_back(FilesPath(Files[i],Dest));
    }

};

void HEADER::FileToAdd()
{
     QStringList Files=QFileDialog::getOpenFileNames(0,tr("Selectionez le ou les fichier(s) a ajouter"),("/home/"+string(getenv("USER"))).c_str());
     string Dest=QInputDialog::getText(0,tr("Entrez le nom du repertoir de destination"),tr("Destination:"),QLineEdit::Normal).toStdString();

     unsigned int _size=Dest.size();

     if (_size>0 && Dest[0]!='/')
        Dest="/"+Dest;

    if(_size>0 && Dest[_size]!='/')
        Dest+=Dest;

     for(int i=0,_size=Files.size();i<_size;++i)
        FilesToAdd.push_back(FilesPath(Files[i].toStdString(),Dest));
};

void HEADER::Creat()
{
    string Dest=QFileDialog::getSaveFileName(0,tr("Enregistrer sous"),("/home/"+string(getenv("USER"))+"/Nouveau_Fichier.dat").c_str(),tr("Fichier Dat(*.dat)")).toStdString();

    if(Dest.find(".dat")>Dest.size())
        Dest+=".dat";

    DAT Dat(Dest);
    Dat.setFiles(FilesToAdd);
    Dat.Create(1);
    Dat.Close();
    FilesToAdd.clear();

    emit FileToOpen(Dest.c_str());
};
