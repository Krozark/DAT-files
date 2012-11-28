#include "DAT.hpp"
#include "zlib.h"

using namespace std;
using namespace sf;

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

void DAT::add(std::string path,std::string dest)
{
    FilesToAdd.push_back(FilesPath(path,dest));
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
    return Create();
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
            debut:
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
                    goto fin;//on va a l'incrementation du path
                }
            }//si on sort de la boucle normalement, c'est qu'on a pas trouvé le dossier, on regarde dans le dossier pointé
            if (dir.Next)
            {
                fseek(DatFile,dir.Next,SEEK_SET);//on va a l'adresse indiquée
                fread(&dir,sizeof(DirectoryPart),1,DatFile);//on le lit
                goto debut;
            }
            fin:
            ++i;
        }//quand on sort de là, on est dans le bon dossier, il faut maintenat chercher le bon fichier
        FileEntry File;
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
                goto find;//on sort de la boucle
            }
        }//si on sort normalement, c'est qu'on a pas trouvé
        cerr<<"Impossible de trouver "<<filename<<endl;
        fclose(DatFile);
        exit(0);

        find: //si le fichier à été trouvé
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
    img->LoadFromMemory(ReadedFiles[CurrentObjet].Buffer,ReadedFiles[CurrentObjet].size);
    return true;
};

bool DAT::Load(sf::Music* mus,std::string path)
{
    GetFile(path);
    mus->OpenFromMemory(ReadedFiles[CurrentObjet].Buffer,ReadedFiles[CurrentObjet].size);
    return true;
};

long CurrentOffset=0;
bool DAT::Create()
{
    vector <TriageFile> Triage; //tableau de tous les chemin des fichiers
    string currentPath="/"; //emplacement acctuel dans l'arbre
    unsigned int _size=FilesToAdd.size(); //nombre de fichiers à ajouter

   for(unsigned int i=0;i<_size;++i)
        Triage.push_back(TriageFile(GetDirectoryOf(FilesToAdd[i].source),GetName(FilesToAdd[i].source)));

    DATHeader DatFileHeader; // file header

    FILE* DatFile=NULL;//The buffer used to read/write the DAT file

    DatFile=fopen (DatFileName.c_str(),"wb+");//on ouvre le fichier destination
    fwrite((char*)&DatFileHeader,1, sizeof(DATHeader),DatFile );//ecriture de l'entête

    sort(Triage.begin(),Triage.end());//triage du tableau par profondeur

    DirectoryEntry directory_entry;
    directory_entry.setName((char*)"/");

    AddInDirectory(Triage,0,DatFile,"/",directory_entry);
    fclose(DatFile);

    cout<<"Total de fichiers ajoutées:\t"<<cpt<<endl;

    return true;
};


template <class T> void DAT::AddInDirectory(std::vector<TriageFile> Triage,const unsigned int profondeur,FILE* DatFile,const char* name,T me) //ajouter dans un nouveau dossier
{
	for (unsigned int i=0;i< profondeur;++i)
		cout<<'\t';
	cout<<"Ajout du répertoir :"<<name<<endl;
    /// Triage est un vecteur contenant les chemin des fichier à inclure qui sont obligatoirement dans le repertoire qui les traite
    fseek(DatFile,0,SEEK_END);
    const long int SeekCurrentDir=ftell(DatFile);//on enregistre la position du curseur dans le fichier où est mise la structure du dossier
    fwrite((char*)&me,1,sizeof(T),DatFile);//on met le dossier courant dans le fichier

    CurrentOffset=ftell(DatFile); //adresse acctuelle dans la mémoire

    unsigned int _size=Triage.size();

    vector<unsigned int> index;
    bool sous_dossier=false;


    for(unsigned int i=0;i<_size && !sous_dossier;++i) //pour tous les fichiers on cherche ceux à ajouter
        if(Triage[i].Path.size()==profondeur)//si c'est un fichier à ajouter
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
        string name_next=Triage[0].Path[profondeur]; //nom du prochain
        vector<TriageFile> Triage_next; //fichier du prochain
        sous_dossier=(me.Nb>=NOMBRE_OF_FILE);

        int meNb=me.Nb;

        while(Triage.size()>0 && !sous_dossier) //tant qu'on a pas distribué tous les fichiers/dossiers
        {
            bool ok=true;
            for(unsigned int i=0;i<Triage.size() && ok;++i)
            {
                if(Triage[i].Path[profondeur]==name_next) //on prend tous les fichier qui seront dans le prochain dossier
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
            AddInDirectory(Triage_next,profondeur+1,DatFile,name_next.c_str(),NextDir);
            Triage_next.clear();

            if (Triage.size()>0)
                name_next=Triage[0].Path[profondeur];
        }
        me.Nb=meNb;
        ResetMe(DatFile,&me,SeekCurrentDir); //on reecrit les info sur le dossier

        if (sous_dossier)
            DeleguerDossier(Triage,profondeur,DatFile,name,&me,SeekCurrentDir);
    }
    fseek(DatFile,0,SEEK_END);

};


template <class T> inline void DAT::ResetMe(FILE* DatFile,T* me,const long int seek)
{
    fseek(DatFile,seek,SEEK_SET);
    fwrite((char*)me,1,sizeof(T),DatFile);//on met à jour le dossier courant dans le fichier
    fseek(DatFile,0,SEEK_END);
    CurrentOffset=ftell(DatFile);
};

template <class T> inline void DAT::DeleguerDossier(std::vector<TriageFile> Triage,const unsigned int profondeur,FILE* DatFile,const char* name,T* me,long int seek)
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

    cout<<"Ajout du fichier \t"<<path<<endl;

     if (InFile)
    {
        char buffer[1];

        FileEntry file_entry;
        file_entry.setName((char*)(*Triage)[id].Name.c_str());

        fseek(InFile,0,SEEK_END);            //on calcul sa taille
        file_entry.size = ftell(InFile);

        fseek(DatFile,0,SEEK_END);
        CurrentOffset=ftell(DatFile);

        file_entry.offset =CurrentOffset+sizeof(FileEntry); //on dis à l'entete où va se trouver le fichier

        me->FilesEntry[index]=CurrentOffset;//on dis au dossier où trouver le fichier

        fwrite((char*)&file_entry,1, sizeof(FileEntry),DatFile); //on ecrit l'entete

        fseek(InFile,0,SEEK_SET); // on revien au debut du fichier du fichier à lire

        while (fread (buffer, 1,sizeof(char),InFile)) //on ecrit le fichier à la suite
            fwrite(buffer,1,sizeof(char),DatFile);

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
