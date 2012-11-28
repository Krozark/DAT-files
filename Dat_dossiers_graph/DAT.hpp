#ifndef DAT_HPP
#define DAT_HPP
#include "Structures.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class DAT
{
public :
    DAT (void);
    DAT(std::string);
    DAT(std::string destination,std::vector<std::string> files);
    ~DAT (void);
    bool Create (std::string destination,std::vector<std::string> files);
    bool Create(int profondeur=0);

    void SetDestination(std::string dest);
    void Open(std::string src){DatFileName=src;};
    void Close();
    void Close_save();

    char* GetFile (std::string filename);
    long int GetFileSize (std::string filename);

    bool Load(sf::Image* img,std::string path);
    bool Load(sf::Music* mus,std::string path);

    bool Load(sf::Image* img,File_Graph file);
    bool Load(sf::Music* mus,File_Graph file);

    void add(std::string path,std::string dest="");
    void add(FilesPath file);
    void setFiles(std::vector<FilesPath> files);

    std::vector<File_Graph> GetFilesIn(long int Offset=-1);

private :
    std::string DatFileName; /// name of the DAT file
    std::vector<FilesPath> FilesToAdd;
    std::vector<ReadedFile> ReadedFiles;

    template <class T> void AddInDirectory(std::vector<TriageFile> Triage,unsigned int profondeur,FILE* DatFile,const char* name,T me); //ajouter dans un nouveau dossier
    template <class T> inline void ResetMe(FILE* DatFile,T* me,const long int seek);
    template <class T> inline void DeleguerDossier(std::vector<TriageFile> Triage,unsigned int profondeur,FILE* DatFile,const char* name,T* me,long int seek);
    template <class T> inline bool EcrireFichier(FILE* DataFile,T* me,unsigned int index,unsigned int id,std::vector<TriageFile> *Triage);
    char* GetFileFromHeader(File_Graph file);

    static std::string GetName(std::string source);//avoir le nom du fichier
    static std::vector<std::string> GetDirectoryOf(std::string source);//avoir une liste de tout les répertoire du chemin d'un fichier

    unsigned int CurrentObjet;
    unsigned int cpt; //compteur de fichiers insérés

};

#endif
