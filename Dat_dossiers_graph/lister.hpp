#ifndef LISTEUR_HPP
#define LISTEUR_HPP

#include <iostream>
#include <string>
#include <vector>



class LISTEUR
{
    /*****************************
    cette class permet de lister le contenu d'un dossier
    */

public:
        LISTEUR(); //constructeur
static std::vector <std::string> lister(const char* dir,...); //permet de spécifier le dossier à lister, et les exeption eventuelles(dernier element à NULL)
static std::vector <std::string> lister_rec(const char* dir,...);

static void ls_rec(std::string nameDir);
};

//void ls_rec(std::string nameDir,std::string path);


#endif
