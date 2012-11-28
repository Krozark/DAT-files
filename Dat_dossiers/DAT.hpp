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
    bool Create();

    void SetDestination(std::string dest);
    void Read(std::string src){DatFileName=src;};

    char* GetFile (std::string filename);
    long int GetFileSize (std::string filename);

    bool Load(sf::Image* img,std::string path);
    bool Load(sf::Music* mus,std::string path);

    void add(std::string path,std::string dest="/");
    ///TODO
   /* bool mkdir(std::string path);
    bool rm(std::string);
    bool rmdir(std::string);
    bool move(std::string,std::string);
    bool cd(std::string);
    std::string ls(std::string="");
    std::string pwd();
    bool cp(std::string,std::string);*/

private :
    std::string DatFileName; /// name of the DAT file
    std::vector<FilesPath> FilesToAdd;
    std::vector<ReadedFile> ReadedFiles;

    template <class T> void AddInDirectory(std::vector<TriageFile> Triage,const unsigned int profondeur,FILE* DatFile,const char* name,T me); //ajouter dans un nouveau dossier
    template <class T> inline void ResetMe(FILE* DatFile,T* me,const long int seek);
    template <class T> inline void DeleguerDossier(std::vector<TriageFile> Triage,const unsigned int profondeur,FILE* DatFile,const char* name,T* me,const long int seek);
    template <class T> inline bool EcrireFichier(FILE* DataFile,T* me,unsigned int index,unsigned int id,std::vector<TriageFile> *Triage);

    std::string GetName(std::string source);//avoir le nom du fichier
    std::vector<std::string> GetDirectoryOf(std::string source);//avoir une liste de tout les répertoire du chemin d'un fichier

    unsigned int CurrentObjet;
    unsigned int cpt;
};

#endif
