#include "DAT.hpp"
#include "zlib.h"

#define _DEBUG

using namespace std;
using namespace sf;

long CurrentOffset=0;

DAT::DAT(string s)
{
    DatFileName=s;
    CurrentObjet=0;
    cpt=0;
};

DAT::DAT (void)
{
    DatFileName="dat.dat";
    CurrentObjet=0;
    cpt=0;
};

DAT::DAT(std::string destination,std::vector<std::string> files)
{
    DatFileName=destination;
    CurrentObjet=0;
    cpt=0;
    for(unsigned int i=0;i<files.size();++i)
        FilesToAdd.push_back(FilesPath(files[i]));
};

DAT::~DAT (void)
{
    for(unsigned int i=0;i<ReadedFiles.size();++i)
        if (ReadedFiles[i].Buffer!=NULL)
            delete (ReadedFiles[i].Buffer);
    ReadedFiles.clear();
};

void DAT::Close()
{
    FilesToAdd.clear();
    for(unsigned int i=0;i<ReadedFiles.size();++i)
        if (ReadedFiles[i].Buffer!=NULL)
            delete (ReadedFiles[i].Buffer);
    ReadedFiles.clear();
    DatFileName="";
    CurrentObjet=0;
    cpt=0;
    CurrentOffset=0;
};

void DAT::Close_save()
{
    FilesToAdd.clear();
    ReadedFiles.clear();
    DatFileName="";
    CurrentObjet=0;
    cpt=0;
    CurrentOffset=0;

};

void DAT::add(std::string path,std::string dest)
{
    FilesToAdd.push_back(FilesPath(path,dest));
};

void DAT::add(FilesPath file)
{
     FilesToAdd.push_back(file);
};

void DAT::setFiles(std::vector<FilesPath> files)
{
    FilesToAdd=files;
};

std::string DAT::GetName(std::string source)
{
    unsigned int index=source.find_last_of('/')+1;
    if (index>source.length())
        index=0;
    return source.substr(index,source.length()-index);
};

std::vector<std::string> DAT::GetDirectoryOf(std::string source)
{
    unsigned int index=GetName(source).length();
    std::vector<std::string> res;

    if((source=source.substr(0,source.length()-index)).length()>0)
        index=source.find_first_of('/');
    else
        return res;

    while (index<source.length())
    {
        res.push_back(source.substr(0,index+1));
        source=source.substr(index+1,source.length());
        index=source.find_first_of('/');
    }
    return res;
};

void DAT::SetDestination(string dest)
{
    this->DatFileName=dest;
};

bool DAT::Create (std::string dest,std::vector<std::string> files)
{
    //An file entry in order to push it in the object's std::vector
    this->DatFileName=dest;
    for(unsigned int i=0;i<files.size();++i)
        FilesToAdd.push_back(FilesPath(files[i]));
    return Create(-1);
};


char* DAT::GetFile (std::string filename)
{
    //on regarde en premier les fichiers déjà lus
    const unsigned int _size=ReadedFiles.size();
    for(unsigned int i=0;i<_size;++i)
        if (ReadedFiles[i].Path==filename)
        {
            CurrentObjet=i;
            return ReadedFiles[i].Buffer;
        }

    FILE* DatFile=fopen(DatFileName.c_str(),"rb");
    if (DatFile)
    {
        std::vector<std::string> Path=GetDirectoryOf(filename);
        string Name=GetName(filename);
        const unsigned int _sizePath=Path.size();

        fseek(DatFile,sizeof(DATHeader),SEEK_SET);

        DirectoryEntry dir;
        fread(&dir,sizeof(DirectoryEntry),1,DatFile);

        char* type=(char*)(&dir);
        DirectoryEntry dir_read;
        unsigned int i=0;// car le "/" ne compte pas
        while(i<_sizePath)
        {
            debut_dossier:
            for(unsigned int j=0;j<dir.Nb;++j)
            {
                fseek(DatFile,dir.FilesEntry[j],SEEK_SET);//on va a l'adresse du supposé dossier pointé
                fread(&dir_read,sizeof(DirectoryEntry),1,DatFile);//on le lit

                type=(char*)(&dir_read);
                if(type[0] == _FILE) //si c'est un fichier, on passe
                    continue;
                if ((char*)dir_read.name == Path[i]) //si c'est le dossier nommé on sort de la boucle
                {
                    dir=dir_read;
                    goto fin_dossier;//on va a l'incrementation du path
                }

            }//si on sort de la boucle normalement, c'est qu'on a pas trouvé le dossier, on regarde dans le dossier pointé
            if (dir.Next!=0)
            {
                fseek(DatFile,dir.Next,SEEK_SET);//on va a l'adresse indiquée
                fread(&dir,sizeof(DirectoryPart),1,DatFile);//on le lit
                goto debut_dossier;
            }
            fin_dossier:
            ++i;
        }//quand on sort de là, on est dans le bon dossier, il faut maintenat chercher le bon fichier
        FileEntry File;
        debut_fichier:
         for(unsigned int j=0;j<dir.Nb;++j)
        {
            fseek(DatFile,dir.FilesEntry[j],SEEK_SET);//on va a l'adresse du supposé dossier pointé
            fread(&File,sizeof(FileEntry),1,DatFile);//on le lit

            type=(char*)(&File);
            if(type[0]!= _FILE) //si c'est un fichier, on passe
                continue;

            if ((char*)File.name == Name) //si c'est le dossier nommé on sort de la boucle
            {
                dir=dir_read;
                goto fin_fichier;//on sort de la boucle
            }
        }//si on sort normalement, c'est qu'on a pas trouvé
        if (dir.Next!=0)
        {
            fseek(DatFile,dir.Next,SEEK_SET);//on va a l'adresse indiquée
            fread(&dir,sizeof(DirectoryPart),1,DatFile);//on le lit
            goto debut_fichier;
        }
        cerr<<"Impossible de trouver "<<filename<<endl;
        fclose(DatFile);
        exit(0);

        fin_fichier: //si le fichier à été trouvé
        ReadedFile newFile(filename);
        newFile.Buffer=new char[File.size];
        newFile.size=File.size;
        if (newFile.Buffer==NULL)
        {
            fclose(DatFile);
            cerr<<"Impossible de trouver "<<filename<<endl;
            exit(0);
        }
        fseek(DatFile,File.offset, SEEK_SET);//Going to the right position
        fread (newFile.Buffer,1, File.size,DatFile);//Reading
        fclose(DatFile);

        ReadedFiles.push_back(newFile);
        CurrentObjet=_size;
        return (newFile.Buffer);
    }
    cerr<<"Impossible d'ouvrir le fichier "<<DatFileName<<endl;
    exit(0);
};

long int DAT::GetFileSize (std::string filename)
{
    if (ReadedFiles[CurrentObjet].Path==filename) //dans le cas ou le dernier aubjet chargé est été le même (pour eviter toute une recherche)
        return ReadedFiles[CurrentObjet].size;

    const unsigned int _size=ReadedFiles.size();
    for(unsigned int i=0;i<_size;++i)
        if (ReadedFiles[i].Path==filename)
        {
            CurrentObjet=i;
            return ReadedFiles[i].size;
        }
    cerr<<"Le fichier "<<filename<<"n'a pas été chargé"<<endl;
    exit(0);
};

bool DAT::Load(sf::Image* img,std::string path)
{
    GetFile(path);
    try
    {
        img->LoadFromMemory(ReadedFiles[CurrentObjet].Buffer,ReadedFiles[CurrentObjet].size);
    }
    catch(...)
    {
        return false;
    }
    return true;
};

bool DAT::Load(sf::Music* mus,std::string path)
{
    GetFile(path);
    try
    {
        mus->OpenFromMemory(ReadedFiles[CurrentObjet].Buffer,ReadedFiles[CurrentObjet].size);
    }
    catch(...)
    {
        return false;
    }
    return true;
};

char* DAT::GetFileFromHeader(File_Graph file)
{
    ReadedFile newFile(file.Name);

    FILE* Me=fopen(DatFileName.c_str(),"rb");

    if (Me == NULL)
    {
        std::cout<<"me = NULL"<<std::endl;
        return NULL;
    }


    fseek(Me,file.Offset,SEEK_SET);
    FileEntry read;
    fread(&read,1,sizeof(FileEntry),Me);

    newFile.Buffer=new char[read.size];
    newFile.size=read.size;

    if (newFile.Buffer==NULL)
    {
        cerr<<"Impossible de charger le fichier "<<file.Name<<endl;
        fclose(Me);
        return NULL;
    }

    fseek(Me,read.offset, SEEK_SET);//Going to the right position
    fread(newFile.Buffer,1, read.size,Me);//Reading

    fclose(Me);

    ReadedFiles.push_back(newFile);
    CurrentObjet=ReadedFiles.size()-1;
    return newFile.Buffer;
};


bool DAT::Load(sf::Image* img,File_Graph file)
{
    if(GetFileFromHeader(file)==NULL)
        return false;

    return (img->LoadFromMemory(ReadedFiles[CurrentObjet].Buffer,ReadedFiles[CurrentObjet].size));
};

bool DAT::Load(sf::Music* mus,File_Graph file)
{
    if(GetFileFromHeader(file)==NULL)
        return false;
    return (mus->OpenFromMemory(ReadedFiles[CurrentObjet].Buffer,ReadedFiles[CurrentObjet].size));
};

bool DAT::Create(int profondeur)
{
    vector <TriageFile> Triage; //tableau de tous les chemin des fichiers
    string currentPath="/"; //emplacement acctuel dans l'arbre
    unsigned int _size=FilesToAdd.size(); //nombre de fichiers à ajouter

   for(unsigned int i=0;i<_size;++i)
        Triage.push_back(TriageFile(GetDirectoryOf(FilesToAdd[i].source),GetName(FilesToAdd[i].source),GetDirectoryOf(FilesToAdd[i].destination)));

    DATHeader DatFileHeader; // file header

    FILE* DatFile=NULL;//The buffer used to read/write the DAT file

    DatFile=fopen (DatFileName.c_str(),"wb+");//on ouvre le fichier destination
    fwrite((char*)&DatFileHeader,1, sizeof(DATHeader),DatFile );//ecriture de l'entête

    sort(Triage.begin(),Triage.end());//triage du tableau par profondeur

    DirectoryEntry directory_entry;
    directory_entry.setName((char*)"/");

    AddInDirectory(Triage,profondeur,DatFile,"/",directory_entry);
    fclose(DatFile);

    cout<<"Total de fichiers ajoutées:\t"<<cpt<<endl;

    return true;
};


template <class T> void DAT::AddInDirectory(std::vector<TriageFile> Triage,unsigned int profondeur,FILE* DatFile,const char* name,T me) //ajouter dans un nouveau dossier
{
    #ifdef DEBUG
    for (unsigned int i=0;i< profondeur;++i)
        cout<<'\t';
    cout<<"Ajout du répertoir :"<<name<<endl;
    #endif
    /// Triage est un vecteur contenant les chemin des fichier à inclure qui sont obligatoirement dans le repertoire qui les traite
    fseek(DatFile,0,SEEK_END);
    const long int SeekCurrentDir=ftell(DatFile);//on enregistre la position du curseur dans le fichier où est mise la structure du dossier
    fwrite((char*)&me,1,sizeof(T),DatFile);//on met le dossier courant dans le fichier

    CurrentOffset=ftell(DatFile); //adresse acctuelle dans la mémoire

    unsigned int _size=Triage.size();

    vector<unsigned int> index;
    bool sous_dossier=false;


    for(unsigned int i=0;i<_size && !sous_dossier;++i) //pour tous les fichiers on cherche ceux à ajouter
        if(Triage[i].FuturPath.size()==profondeur)//si c'est un fichier à ajouter
        {
            if (me.Nb<NOMBRE_OF_FILE)
            {
                me.Nb++;
                index.push_back(i);
            }
            else
                sous_dossier=true;
        }

    ///on a donc tous les fichiers à ajouter, et si nesésaire, dans un dossier de suite
    _size=0;    //difference dans index

    for (unsigned int i=0;i<me.Nb;++i) //pour tous les fichier à ajouter
    {
        //on va chercher le fichier
        int id=index[i]-_size;
        if(EcrireFichier(DatFile,&me,i,id,&Triage)) //on ecrit le fichier
            ++_size;
        else
            exit(0);
    }

    ResetMe(DatFile,&me,SeekCurrentDir); //on reecrit les info sur le dossier

    if (sous_dossier)//si sous_dossier, envoyer le vecteur.
        DeleguerDossier(Triage,profondeur,DatFile,name,&me,SeekCurrentDir);

    else if (Triage.size()>0)//on à écrit tous les fichiers, on passe aux sous dossiers  //sinon découper le vecteur par sousdossier et appeler la fonction pour chaqu'un des dossiers
    {
     //si il y à des fichiers/dossier à mettre
        string name_next=Triage[0].FuturPath[profondeur]; //nom du prochain
        vector<TriageFile> Triage_next; //fichier du prochain
        sous_dossier=(me.Nb>=NOMBRE_OF_FILE);

        int meNb=me.Nb;

        while(Triage.size()>0 && !sous_dossier) //tant qu'on a pas distribué tous les fichiers/dossiers
        {
            bool ok=true;
            for(unsigned int i=0;i<Triage.size() && ok;++i)
            {
                if(Triage[i].FuturPath[profondeur]==name_next) //on prend tous les fichier qui seront dans le prochain dossier
                {
                    Triage_next.push_back(Triage[i]);
                    Triage.erase(Triage.begin()+i);
                    --i;
                }
                else
                    ok=false;
            }

            fseek(DatFile,0,SEEK_END);
            CurrentOffset=ftell(DatFile);

            me.FilesEntry[meNb]=CurrentOffset;
            sous_dossier=((++meNb)>=NOMBRE_OF_FILE);

            DirectoryEntry NextDir;//prochain repertoir
            NextDir.setName((char*)name_next.c_str());
            AddInDirectory(Triage_next,profondeur+1,DatFile,(char*)NextDir.name,NextDir);
            Triage_next.clear();

            if (Triage.size()>0)
                name_next=Triage[0].FuturPath[profondeur];
        }
        me.Nb=meNb;
        ResetMe(DatFile,&me,SeekCurrentDir); //on reecrit les info sur le dossier

        if (sous_dossier)
            DeleguerDossier(Triage,profondeur,DatFile,name,&me,SeekCurrentDir);
    }
    fseek(DatFile,0,SEEK_END);

};


template <class T> inline void DAT::ResetMe(FILE* DatFile,T* me,long int seek)
{
    fseek(DatFile,seek,SEEK_SET);
    fwrite((char*)&(*me),1,sizeof(T),DatFile);//on met à jour le dossier courant dans le fichier
    fseek(DatFile,0,SEEK_END);
    CurrentOffset=ftell(DatFile);
};

template <class T> inline void DAT::DeleguerDossier(std::vector<TriageFile> Triage,unsigned int profondeur,FILE* DatFile,const char* name,T* me,long int seek)
{
    me->Next=ftell(DatFile); //on pointe vers les prochain
    ResetMe(DatFile,me,seek); //on met à jour l'entete du dossier

    DirectoryPart NextDir;
    AddInDirectory(Triage,profondeur,DatFile,name,NextDir);
};

template <class T> inline bool DAT::EcrireFichier(FILE* DatFile,T* me,unsigned int index,unsigned int id,std::vector<TriageFile> *Triage)
{
    string path;//chemin complet du fichier
    unsigned int profondeur=(*Triage)[id].Path.size();

       for(unsigned int j=0;j<profondeur;++j)
            path+=(*Triage)[id].Path[j];
        path+=(*Triage)[id].Name; //on a le nom complet

    FILE* InFile=fopen (path.c_str(),"rb"); //on ouvre le fichier


     if (InFile)
    {
        char buffer[1];

        FileEntry file_entry;
        file_entry.setName((char*)(*Triage)[id].Name.c_str());

        fseek(InFile,0,SEEK_END);            //on calcul sa taille
        file_entry.size = ftell(InFile);

        fseek(DatFile,0,SEEK_END);
        CurrentOffset=ftell(DatFile);

        file_entry.offset =(CurrentOffset+sizeof(FileEntry)); //on dis à l'entete où va se trouver le fichier

        me->FilesEntry[index]=CurrentOffset;//on dis au dossier où trouver l'entete du fichier

        fwrite((char*)&file_entry,1, sizeof(FileEntry),DatFile); //on ecrit l'entete

        fseek(InFile,0,SEEK_SET); // on revien au debut du fichier du fichier à lire

        #ifdef DEBUG
         cout<<"Ajout du fichier \t <"<<(*Triage)[id].Name<<">"<<endl;
         #endif

        fread (buffer, 1,sizeof(char),InFile);
        while (!feof(InFile))
        {
            fwrite(buffer,1,sizeof(char),DatFile); //on ecrit le fichier à la suite
            fread (buffer, 1,sizeof(char),InFile);
        }


        CurrentOffset=ftell(DatFile);//on met à jour le curseur
        fclose(InFile); //We finished with this file
        Triage->erase(Triage->begin()+id);    //on supprime le fichier
        ++cpt;

    }
    else
    {
        //Simple error track
        cerr<<"File "<<path<<" raise an error."<<endl;
        return false;
    }
    return true;
};

std::vector<File_Graph> DAT::GetFilesIn(long int Offset) //l'offest spécifié est celui de je sais pas quoi (directoryEntry ou FileEntry )
{
    std::vector<File_Graph> res;

    if(DatFileName.length()==0)
    {
        cerr<<"Aucun nom de fichier .dat à ouvrir"<<endl;
        return res;
    }

    FILE* DatFile=fopen(DatFileName.c_str(),"rb");

    if(DatFile==NULL)
    {
        cerr<<"Impossible d'ouvrir le fichier "<<DatFileName<<endl;
        return res;
    }

    if (Offset==-1)//si le paramètre était pas spécifié, c'est qu'on cherche la racine
        Offset=sizeof(DATHeader);
    //sino, on va à l'adresse indiquée

    fseek(DatFile,Offset,SEEK_SET);

    DirectoryEntry CurrentDirectory;
    fread(&CurrentDirectory,sizeof(DirectoryEntry),1,DatFile);

    FileEntry      ReadFile;

    if (CurrentDirectory.Type==_FILE)
    {
        fseek(DatFile,Offset,SEEK_SET);
        fread(&ReadFile,sizeof(FileEntry),1,DatFile);
        return res;
    }
    else if (CurrentDirectory.Type==_DIRECTORY)
    {

        unsigned int _size=CurrentDirectory.Nb;

        DirectoryEntry ReadDirectory;

        debut:
        for(unsigned int i=0;i<_size;++i)//pour tous les fichiers/dissiers pointés
        {
            fseek(DatFile,CurrentDirectory.FilesEntry[i],SEEK_SET);
            fread(&ReadFile,sizeof(FileEntry),1,DatFile);//au cas ou c'est un fichier(il y a plus de fichiers )

            if(ReadFile.Type == _FILE) //si c'est un fichier, on le lit
            {
                res.push_back(File_Graph((char*)ReadFile.name,CurrentDirectory.FilesEntry[i],_FILE));
            }
            else if (ReadFile.Type == _DIRECTORY)
            {
                fseek(DatFile,CurrentDirectory.FilesEntry[i],SEEK_SET);
                fread(&ReadDirectory,sizeof(DirectoryEntry),1,DatFile); //au cas où c'est un dossier on le relis pour avoir les bonnes infos
                res.push_back(File_Graph((char*)ReadDirectory.name,CurrentDirectory.FilesEntry[i],_DIRECTORY));
            }

        }
        if (CurrentDirectory.Next)
        {
            fseek(DatFile,CurrentDirectory.Next,SEEK_SET);//on va a l'adresse indiquée
            fread(&CurrentDirectory,sizeof(DirectoryPart),1,DatFile);//on le lit
            goto debut;//on recommence l'oppération
        }
    }
    fclose(DatFile);

    sort(res.begin(),res.end());

    return res;
};
