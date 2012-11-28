#include "BreadCrumb.hpp"

void BREADCRUMB::push_back(File_Graph f)
{
    unsigned int index=BreadCrumbButton.size();
    BreadCrumbButton.push_back(new Button(f.Name,f.Offset,index));

    QObject::connect(BreadCrumbButton[index],SIGNAL(EmitIndex(int)),this,SLOT(ReciveIndex(int)));
    Layout->addWidget(BreadCrumbButton[index]);
};

void BREADCRUMB::ReciveIndex(int Index)
{
    emit EmitOffest(BreadCrumbButton[Index]->Offset);
    ++Index;
    for (unsigned int i=Index,_size=BreadCrumbButton.size();i<_size;++i)
    {
        Layout->removeWidget(BreadCrumbButton[i]);
        delete BreadCrumbButton[i];
    }
    BreadCrumbButton.erase(BreadCrumbButton.begin()+Index,BreadCrumbButton.end());
    repaint(contentsRect());
};


void BREADCRUMB::ReciveFile(File_Graph f)
{
    push_back(f);
};

void BREADCRUMB::Clear()
{
    for (unsigned int i=0,_size=BreadCrumbButton.size();i<_size;++i)
    {
        Layout->removeWidget(BreadCrumbButton[i]);
        delete BreadCrumbButton[i];
    }
    BreadCrumbButton.clear();
};
