#ifndef BREADCRUMB_Hpp
#define BREADCRUMB_Hpp

#include "Structures.hpp"
#include "QSFMLCanvas.hpp"
#include <QtGui>

class Button;
class BREADCRUMB : public QSFMLCanvas
{
    Q_OBJECT

    public:
        BREADCRUMB(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 60): QSFMLCanvas(Parent,Position,Size,FrameTime)
        {
            Layout=new QHBoxLayout;
            setLayout(Layout);
        };

    public slots:
        void ReciveIndex(int);
        void ReciveFile(File_Graph);
        void Clear();
        signals:
        void EmitOffest(long int Offset);

    private:
        inline void push_back(File_Graph);
        QHBoxLayout* Layout;
        std::vector<Button*> BreadCrumbButton;
};

class Button : public QPushButton
{
    Q_OBJECT
    public:
        Button(char* name,long int offset,int index) : QPushButton(name) ,
        Offset(offset),
        Index(index)
        {
            QObject::connect(this,SIGNAL(clicked()),this, SLOT(HaveToEmit()));
        };

        const long int Offset;

    public slots:
            void HaveToEmit(){emit EmitIndex(Index);};
            signals:
            void EmitIndex(int);

    private:
        const int Index;
};

#endif // BREADCRUMB_Hpp
