#include <SFML/Graphics.hpp>
#include "string.h"

#define _FILE   '0'
#define _DIRECTORY  '1'
#define _DIRECTORY_PART '2'

struct DATHeader
{
    sf::Uint8 uniqueID[5]; /// Unique ID used to know if this file is a DAT File from this class
    sf::Uint8 version[3]; /// Version of the DAT file format
    sf::Int32 nb_files; /// Number of files in the DAT file

    DATHeader()
    {
        uniqueID={0,0,0,0,0};
        version={0,0,0};
        nb_files=-1;
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
        size=-1;
        offset=-1;
    };
};

#define NOMBRE_OF_FILE  32
struct DirectoryEntry
{
     sf::Int8 Type;
     sf::Uint8 name[100]; /// Name of the data directory
     sf::Int32 FilesEntry[NOMBRE_OF_FILE]; ///Offset, in the DAT file wher are the files
     sf::Int32 Next;///Offset in the DAT file where are the next file if more than 32 file are in the directory (DirectoryPart)
     sf::Int8  Nb;///Nomber of file in this directory if Nb > NOMBRE_OF_FILE, go see in Next

     DirectoryEntry()
     {
         Type=_DIRECTORY;
         name[0]='\0';
         for(int i=0;i<NOMBRE_OF_FILE;++i)
            FilesEntry[i]=-1;
        Next=-1;
        Nb=0;
     };
};

struct DirectoryPart
{
    sf::Int8 Type;
    sf::Int32 FilesEntry[NOMBRE_OF_FILE]; ///Offset, in the DAT file wher are the files
    sf::Int32 Next;///Offset in the DAT file where are the next file if more than 32 file are in the directory (DirectoryPart)
    sf::Int8  Nb;///Nomber of file in this directory if Nb > NOMBRE_OF_FILE, go see in Next

    DirectoryPart()
    {
        Type=_DIRECTORY_PART;
        for(int i=0;i<NOMBRE_OF_FILE;++i)
            FilesEntry[i]=-1;
        Next=-1;
        Nb=0;
    };
};

//char* type=(char*)(&f);
