#ifndef FOOTER_HPP
#define FOOTER_HPP
#include "QSFMLCanvas.hpp"
#include <QtGui>

class FOOTER : public QSFMLCanvas
{
    Q_OBJECT
    public:
    FOOTER(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 60): QSFMLCanvas(Parent,Position,Size,FrameTime) ,SizeMini(Size)
    {
    };

    public slots:
        void ReciveNbFiles(int);
        void ReciveNbDir(int);

    protected:
        void resizeEvent ( QResizeEvent * event );

    private:
        void OnInit();
        void OnUpdate();
        void MAJ();

        sf::String Description;
        int NbFiles,NbDir;

        const QSize SizeMini;
        sf::View CurrentView;
};


#endif
