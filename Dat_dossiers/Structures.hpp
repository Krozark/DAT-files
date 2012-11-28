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
        /*uniqueID={'D','G','D','A','T'};
        version={'0','0','2'};*/
    };
};

struct FileEntry
{
    sf::Int8 Type;
    sf::Uint8 name[100]; /// Name of the data file
    sf::Int32 size; /// Size of the data file
    sf::Int32 offset; /// Offset, in the DAT file where the file is

    FileEntry()
    {
        Type=_FILE;
        name[0]='\0';
        size=0;
        offset=0;
    };


     void setName(char* nom){
        int size=strlen(nom);
        memcpy (name,nom,size);
        name[size]='\0';
     }
};

#define NOMBRE_OF_FILE  32
struct DirectoryEntry
{
     sf::Int8 Type;
     sf::Int32 FilesEntry[NOMBRE_OF_FILE]; ///Offset, in the DAT file wher are the files
     sf::Int32 Next;///Offset in the DAT file where are the next file if more than 32 file are in the directory (DirectoryPart)
     sf::Int32  Nb;///Nomber of file in this directory if Nb > NOMBRE_OF_FILE, go see in Next
     sf::Uint8 name[100]; /// Name of the data directory

     DirectoryEntry()
     {
         Type=_DIRECTORY;
         name[0]='\0';
         for(int i=0;i<NOMBRE_OF_FILE;++i)
            FilesEntry[i]=0;
        Next=0;
        Nb=0;
     };

     void setName(char* nom){
        int size=strlen(nom);
        memcpy (name,nom,size);
        name[size]='\0';
     }
};

struct DirectoryPart
{
    sf::Int8 Type;
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


struct FilesPath
{
    std::string source,destination;

    FilesPath()
    {
        destination="/";
    }

    FilesPath(std::string s,std::string d="\t")
    {
        source=s;
        if(d=="\t")
            destination=s;
        else
            destination=d;
    }
};



struct TriageFile
{
    std::vector <std::string> Path;
    std::string Name;

    TriageFile()
    {

    };
    TriageFile(std::vector <std::string> p,std::string n)
    {
        Path=p;
        Name=n;
    };

     friend bool operator<(const TriageFile &f1,const TriageFile &f2)    //comparaison pour le trie
    {
        unsigned int i1=f1.Path.size()+1,i2=f2.Path.size()+1;
        std::vector <std::string> path1,path2;
        path1=f1.Path;
        path1.push_back(f1.Name);
        path2=f2.Path;
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

struct ReadedFile
{
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

#endif
