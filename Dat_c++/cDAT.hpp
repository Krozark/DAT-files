#include "Structures.hpp"
#include <iostream>
#include <fstream>
#include "string.h"

class cDAT
{
private :
    std::string m_datfile; /// name of the DAT file
    sDATHeader m_header; /// file header
    std::vector<sFileEntry> m_entries; /// vector of files entries
    char* m_buffer; /// Buffer pointing on a file in memory
public :
    cDAT (void);
    ~cDAT (void);
    bool Create (std::vector<std::string> files, std::string destination);
    void Read (std::string source);
    char* GetFile (std::string filename);
    long int GetFileSize (std::string filename);
};
