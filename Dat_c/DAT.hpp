#include "Structures.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

class DAT
{
public :
    DAT (void);
    ~DAT (void);
    bool Create (std::vector<std::string> files, std::string destination);
    bool Create (std::string dest);

    void Read (std::string source);
    char* GetFile (std::string filename);
    long int GetFileSize (std::string filename);
    ///TODO
    bool mkdir(std::string path);
    bool add(std::string path);
    bool rm(std::string);
    bool rmdir(std::string);
    bool move(std::string,std::string);
    bool cd(std::string);
    std::string ls(std::string="");
    std::string pwd();
    bool cp(std::string,std::string);

private :
    std::string m_datfile; /// name of the DAT file
    DATHeader m_header; /// file header
    std::vector<FileEntry> m_entries; /// vector of files entries
    std::vector<char*> buffer; /// Buffer pointing on a file in memory
    DirectoryEntry CurrentDirectory;
    ///todo
    std::vector<std::string> GetDirectoryOf(std::string File);
    std::string destination;
};
