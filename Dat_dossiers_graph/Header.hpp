#ifndef HEADER_HPP
#define HEADER_HPP

#include <QtGui>
#include "Structures.hpp"

class HEADER : public QObject
{
    Q_OBJECT
    public:
        HEADER (QMainWindow * Parent);

    public slots:
        void FileToOpen();
        void DirectoryToAdd();
        void FileToAdd();
        void Creat();
        signals:
        void FileToOpen(QString);

    private:
        QAction *_New,*_Open,*_Quit,*_AddF,*_AddD,*_Creat;
        std::vector <FilesPath> FilesToAdd;

};

#endif

