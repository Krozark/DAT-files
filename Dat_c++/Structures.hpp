#include <SFML/Graphics.hpp>

enum _Type{_FILE=0,_DIRECTORY,_DIRECTORY_PART};

struct sDATHeader
{
    sf::Uint8 uniqueID[5]; /// Unique ID used to know if this file is a DAT File from this class
    sf::Uint8 version[3]; /// Version of the DAT file format
    sf::Int32 nb_files; /// Number of files in the DAT file
};

struct sFileEntry
{
    sf::Int8 Type;
    sf::Uint8 name[100]; /// Name of the data file
    sf::Int32 size; /// Size of the data file
    sf::Int32 offset; /// Offset, in the DAT file where the file is
};

#define NOMBRE_OF_FILE  32
struct sDirectoryEntry
{
     sf::Int8 Type;
     sf::Uint8 name[100]; /// Name of the data directory
     sf::Int32 FilesEntry[NOMBRE_OF_FILE]; ///Offset, in the DAT file wher are the files
     sf::Int32 Next;///Offset in the DAT file where are the next file if more than 32 file are in the directory (sDirectoryPart)
     sf::Int8  Nb;///Nomber of file in this directory if Nb > NOMBRE_OF_FILE, go see in Next
};

struct sDirectoryPart
{
    sf::Int8 Type;
    sf::Int32 FilesEntry[NOMBRE_OF_FILE]; ///Offset, in the DAT file wher are the files
    sf::Int32 Next;///Offset in the DAT file where are the next file if more than 32 file are in the directory (sDirectoryPart)
    sf::Int8  Nb;///Nomber of file in this directory if Nb > NOMBRE_OF_FILE, go see in Next
};
