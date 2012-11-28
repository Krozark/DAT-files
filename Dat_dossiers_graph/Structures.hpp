#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <SFML/Graphics.hpp>
#include "string.h"
#include <iostream>

#define _FILE   '0'
#define _DIRECTORY  '1'
#define _DIRECTORY_PART '2'

struct DATHeader
{
    sf::Uint8 uniqueID[5]; /// Unique ID used to know if this file is a DAT File from this class
    sf::Uint8 version[3]; /// Version of the DAT file format

    DATHeader()
    {
        memcpy(uniqueID,"DGDAT",sizeof(sf::Uint8)*5);
        memcpy(version,"002",sizeof(sf::Uint8)*3);
    };
};

struct FileEntry
{
    sf::Uint8 Type;
    sf::Uint8 name[100]; /// Name of the data file
    sf::Int32 size; /// Size of the data file
    sf::Int32 offset; /// Offset, in the DAT file where the file is

    FileEntry()
    {
        Type=_FILE;
        for(unsigned int i=0;i<100;++i)
            name[i]='\0';
        size=0;
        offset=0;

    };


     void setName(char* nom)
     {
        int size=strlen(nom);
        memcpy (name,nom,size);
        name[size]='\0';
     }
};

#define NOMBRE_OF_FILE  32
struct DirectoryEntry
{
     sf::Uint8 Type;
     sf::Int32 FilesEntry[NOMBRE_OF_FILE]; ///Offset, in the DAT file wher are the files
     sf::Int32 Next;///Offset in the DAT file where are the next file if more than 32 file are in the directory (DirectoryPart)
     sf::Int32  Nb;///Nomber of file in this directory if Nb > NOMBRE_OF_FILE, go see in Next
     sf::Uint8 name[100]; /// Name of the data directory

     DirectoryEntry()
     {
         Type=_DIRECTORY;
         for(int i=0;i<NOMBRE_OF_FILE;++i)
            FilesEntry[i]=0;
        for(unsigned int i=0;i<100;++i)
            name[i]='\0';
        Next=0;
        Nb=0;
     };

     void setName(char* nom)
     {
        int size=strlen(nom);
        memcpy (name,nom,size);
        name[size]='\0';
     }
};

struct DirectoryPart
{
    sf::Uint8 Type;
    sf::Int32 FilesEntry[NOMBRE_OF_FILE]; ///Offset, in the DAT file wher are the files
    sf::Int32 Next;///Offset in the DAT file where are the next file if more than 32 file are in the directory (DirectoryPart)
    sf::Int32  Nb;///Nomber of file in this directory if Nb > NOMBRE_OF_FILE, go see in Next

    DirectoryPart()
    {
        Type=_DIRECTORY_PART;
        for(int i=0;i<NOMBRE_OF_FILE;++i)
            FilesEntry[i]=0;
        Next=0;
        Nb=0;
    };
};

//char* type=(char*)(&f);


class FilesPath
{
    public:
    std::string source,destination;

    FilesPath()
    {
        destination="/";
    }

    FilesPath(std::string s,std::string d="")
    {
        source=s;
        if(d=="")
            destination=s;
        else
            destination=d;
    }
};



class TriageFile
{
     public:
    std::vector <std::string> Path;
    std::string Name;
    std::vector <std::string> FuturPath;

    TriageFile()
    {

    };
    TriageFile(std::vector <std::string> p,std::string n)
    {
        Path=p;
        Name=n;
        FuturPath=p;
    };

     TriageFile(std::vector <std::string> p,std::string n,std::vector <std::string> f)
    {
        Path=p;
        Name=n;
        if(f.size()>0)
            FuturPath=f;
        else
            FuturPath=p;
    };


     friend bool operator<(const TriageFile &f1,const TriageFile &f2)    //comparaison pour le trie
    {
        unsigned int i1=f1.FuturPath.size()+1,i2=f2.FuturPath.size()+1;

        std::vector <std::string> path1,path2;

        path1=f1.FuturPath;
        path1.push_back(f1.Name);

        path2=f2.FuturPath;
        path2.push_back(f2.Name);

        for(unsigned int i=0;i<i1 && i< i2;++i)
        {
            if(path1[i]==path2[i])
                continue;
            return path1[i].compare(path2[i])<0;
        }
        return false;
    };
};

class ReadedFile
{
     public:
    std::string Path;
    char* Buffer;
    sf::Int32 size;

    ReadedFile()
    {
        Path="";
        Buffer=NULL;
    };
    ReadedFile(std::string p)
    {
        Path=p;
    }
};



class File_Graph
{
    public:

    char Name[100];
    sf::Uint8 Type;
    long int Offset; //offset de l'entette dans le fichier

    File_Graph(char* N,long int O,char T=_FILE)
    {
        strcpy(Name,N);
        Offset=O;
        Type=T;
    };
     friend bool operator<(const File_Graph &f1,const File_Graph &f2)    //comparaison pour le trie
     {
         if(f1.Type == f2.Type)
            return strcmp(f1.Name,f2.Name)<0;
        if(f1.Type==_FILE)
            return false; //f2 dossier
        return true; //f1 dossier
     }
};

#define SIZE_ICON   96
#define ESPACE_ICON 32
#define ESPACE_TEXTE 8

#define ESPACE_Y ( SIZE_ICON + ESPACE_ICON )
#define ESPACE_X ( SIZE_ICON + ESPACE_ICON )


class Icons
{
    public:
    sf::Sprite Sprite;

    Icons()
    {
        Name.SetSize(15);
        Name.SetColor(sf::Color::Green);
    };

    void Resize()
    {
        Sprite.Resize(SIZE_ICON,SIZE_ICON);
    };

    void Draw(sf::RenderTarget* App)
    {
        App->Draw(Sprite);
        App->Draw(Name);
    };

    void SetName(std::string n)
    {
        Name.SetText(n);
    };

    void SetPosition(float x,float y)
    {
      Sprite.SetPosition(x,y);
      Name.SetCenter((Name.GetRect().GetWidth()-SIZE_ICON)/2.0f,-SIZE_ICON);
      Name.SetPosition(x,y);
    };

    ~Icons()
    {
    }

    private:
        sf::String Name;

};

#endif
