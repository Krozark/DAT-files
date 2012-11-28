//#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include "lister.hpp"


using namespace std;

LISTEUR::LISTEUR()
{
}


 std::vector <std::string> LISTEUR::lister(const char* dir,...)
{
    va_list p_list;
    char* exp;
    va_start(p_list, dir);

    vector  <std::string> exeptions;

    while((exp=va_arg(p_list, char *))!=NULL)
        exeptions.push_back(exp);

    vector  <std::string> res;

    DIR *curDir;
    bool b_error = false;
    struct dirent *curEntry;
    errno = false;

    if (NULL == (curDir = opendir(dir)))
        //verification des erreurs
        b_error = true;
     else
    {
        errno = false;
        unsigned int _size=exeptions.size();

        while ((curEntry = readdir(curDir))!= NULL)
        {
            if ((strcmp(curEntry->d_name,"."))&& (strcmp(curEntry->d_name,".."))) //si direfent du nom de dossier courant et parent
            {
                bool ok=true;
                for(unsigned int i=0;i<_size && ok;++i) //on regarde els exeption
                    if (!strcmp(curEntry->d_name,exeptions[i].c_str())) //si ca corespon à une d'elle
                        ok=false; //on ajoute pas

                if(ok) //sino, on ajoute
                    res.push_back(curEntry->d_name);
            }
        }

        if (errno)
            b_error = true;
    }

    //return
    if (b_error)
    {
        res.clear();
        return (res);
    }
    else
    {

        return res;
    }
};
 vector  <std::string> exeptions;
 vector  <std::string> res;
 unsigned int _size=0;

std::vector <std::string> LISTEUR::lister_rec(const char* dir,...)
{
    ///on cherche les exeptions
    exeptions.clear();
    res.clear();
    va_list p_list;
    char* exp;
    va_start(p_list, dir);
    while((exp=va_arg(p_list, char *))!=NULL)
        exeptions.push_back(exp);///on les ajout à un tableau

    _size=exeptions.size();

    string src=dir;
    if(src.length()>3 && src[0]=='.' && src[1]=='/')
        src=src.substr(2,src.length());
    ls_rec(src);

    return res;
};


void LISTEUR::ls_rec(string nameDir)
{
    DIR*dir;
    struct dirent* d;
    struct stat st;

    if( (dir=opendir(nameDir.c_str())) != NULL)
    {
        while( (d=readdir(dir)) )
        {
            string svt=nameDir;
            if (nameDir[nameDir.length()-1]=='/')
                svt+=d->d_name;
            else
                svt+=("/"+(string)d->d_name);

            stat(string(svt).c_str(),&st);

            if (!S_ISDIR(st.st_mode)) //si c'est un fichier
            {
                bool ok=true;
                for(unsigned int i=0;i<_size && ok;++i) //on regarde els exeption
                    if (!strcmp(svt.c_str(),exeptions[i].c_str())) //si ca corespon à une d'elle
                        ok=false; //on ajoute pas et on stop la boucle
                if(ok) //sinon, on ajoute
                    res.push_back(svt);
            }
            else if ((strcmp(d->d_name,"."))&& (strcmp(d->d_name,".."))) //si c'est un dossier
            {
                bool ok=true;
                for(unsigned int i=0;i<_size && ok;++i) //on regarde els exeption
                    if (!strcmp(svt.c_str(),exeptions[i].c_str())) //si ca corespon à une d'elle
                        ok=false; //on ajoute pas et on stop la boucle

                if(ok) //sinon, on ajoute
                    ls_rec(svt);
            }
            else
                continue;

        }
        closedir(dir);
    }
};




