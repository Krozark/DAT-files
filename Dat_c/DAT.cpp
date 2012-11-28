#include "DAT.hpp"
#include "zlib.h"
using namespace std;
using namespace sf;

DAT::DAT (void)
{
//    buffer = NULL;
};

DAT::~DAT (void)
{
    for(unsigned int i=0;i<buffer.size();++i)
        if (buffer[i]!=NULL)
            delete (buffer[i]);
    buffer.clear();
};

bool DAT::Create(string dest)
{
    this->destination=dest;
};

bool DAT::Create (std::vector<std::string> files, std::string dest)
{
    //An file entry in order to push it in the object's std::vector
    this->destination=dest;
    FileEntry entry;
    //An input file stream to read each file included
    //std::ifstream file;
    FILE* file=NULL;
    //An output file stream to write our DAT file
    //std::ofstream datfile;
    FILE* datfile=NULL;
    //The buffer used to read/write the DAT file
    char buffer[1];

    //DATHeader
    //We start by filling it with 0
    memset (&m_header, 0, sizeof(m_header));
    //Then we copy the ID
    memcpy (m_header.uniqueID, "DGDAT", 5);
    //Then the version
    memcpy (m_header.version, "0.2", 3);
    //Then the number of files to include
    m_header.nb_files = files.size();

    //Next, we open each file in order to create the File Entries Table
    for (unsigned int i = 0; i<files.size(); i++)
    {
        file= fopen (files[i].c_str(),"rb");
        if (file)
        {
            //Filling the FileEntry with 0
            memset (&entry, 0, sizeof(FileEntry) );
            //We keep the file name
            memcpy (entry.name, files[i].c_str(), strlen ( files[i].c_str() ) );
            //We calculate its size
            //file.seekg (0, std::ios::end);
            fseek(file,0,SEEK_END);
            entry.size = ftell(file);
            //Since we don't know exactly its final position in the DAT file, let's use 0
            entry.offset = 0;
            //We finished with this file
            fclose(file);

            //Finally, we add this File Entry in our std::vector
            m_entries.push_back(entry);
        }
        else
        {
            //Simple error track
            std::cout<<"File "<<files[i]<<" raise an error."<<std::endl;
            return (false);
        }
    }

    //Now, we know everything about our files, we can update offsets
    long actual_offset = 0;
    actual_offset += sizeof(DATHeader);
    actual_offset += m_header.nb_files * sizeof(FileEntry);
    for (unsigned int i=0;i<m_entries.size();i++)
    {
        m_entries[i].offset = actual_offset;
        actual_offset += m_entries[i].size;
    }

    //And finally, we are writing the DAT file
    datfile=fopen (destination.c_str(),"wb");

    //First, we write the header
    fwrite((char*)&m_header,1, sizeof(DATHeader),datfile );

    //Then, the File Entries Table
    for (unsigned int i=0;i<m_entries.size();i++)
    {
        fwrite((char*)&m_entries[i],1, sizeof(FileEntry),datfile );
    }

    //Finally, we write each file
    for (unsigned int i = 0; i<m_entries.size(); i++)
    {
        file=fopen ((char*)m_entries[i].name,"rb");
        if (file)
        {
            fseek(file,0,SEEK_SET);
            while (fread (buffer, 1,sizeof(char),file))
            {
                fwrite(buffer,1,sizeof(char),datfile);
            }
            fclose(file);
        }
        file=NULL;
    }
    //And it's finished
    fclose(datfile);
    return (true);
};



void DAT::Read (std::string source)
{
    //The input file stream from which we want informations
   FILE* datfile=NULL;
    //A file entry in order to push it in the object's std::vector
    FileEntry entry;

    //Filling the header with 0
    memset (&m_header, 0, sizeof(m_header));
    //We open the DAT file to read it
    datfile=fopen (source.c_str(),"rb");
    if (datfile)
    {
        //Getting to the Header position
        fseek(datfile,0, SEEK_SET);
        //Reading the DAT Header
        fread ((char*)&m_header,1, sizeof(DATHeader),datfile);
        //Next we are reading each file entry
        for (unsigned int i=0;i<m_header.nb_files;i++)
        {
            //Reading a File Entry
            fread ((char*)&entry,1, sizeof(FileEntry),datfile);
            //Pushing it in our std::vector
            m_entries.push_back(entry);
        }
        //Since all seems ok, we keep the DAT file name
        m_datfile = source;
    }
    //Closing the DAT file
    fclose(datfile);
};


char* DAT::GetFile (std::string filename)
{
    //The input file stream from which we want information
    FILE* datfile=NULL;

    char* m_buffer=NULL;

    //Cleaning properly an ancient file loaded
/*    if (buffer != NULL)
    {
        delete (buffer);
        buffer = NULL;
    }*/

    //First, we have to find the file needed
    for (unsigned int i=0; i<m_header.nb_files;i++)
    {
        //If we found it
        if ((char*)(m_entries[i].name) == filename)
        {
            //We are allocating memory to the buffer
            m_buffer = new char[(m_entries[i].size)];
            //Simple error catch
            if (m_buffer==NULL)
                return (NULL);
            //Opening the DAT file ot read the file datas needed
            datfile=fopen (m_datfile.c_str(),"rb");
            if (datfile)
            {
                //Going to the right position
                fseek(datfile,m_entries[i].offset, std::ios::beg);
                //Reading
                fread (m_buffer,1, m_entries[i].size,datfile);
                //We can close the DAT file
                fclose(datfile);
                //Returning the buffer
                buffer.push_back(m_buffer);
                return (m_buffer);
            }
        }
    }
    //Finally, there is no such file in our DAT file
    return (NULL);
};

long int DAT::GetFileSize (std::string filename)
{
    //First, we have to find the file needed
    for (unsigned int i=0; i<m_header.nb_files;i++)
    {
        //If we found it
        if ((char*)(m_entries[i].name) == filename)
        {
            //Returning the size of the file found
            return (m_entries[i].size);
        }
    }
    return (0);
};






