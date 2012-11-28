#ifndef TOTALFRAME_HPP
#define TOTALFRAME_HPP

#include "QSFMLCanvas.hpp"

#include <QtGui>

#include "MainFrame.hpp"
#include "Footer.hpp"
#include "include/BreadCrumb.hpp"
#include "Header.hpp"

class TOTALFRAME : public QMainWindow
{
    Q_OBJECT
  public:
    TOTALFRAME(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 60): QMainWindow(Parent)
    {
        resize(Size);
        OnInit();
    };
private:

    void OnInit();
    void OnUpdate();
    QScrollArea* ScrollArea;
    QGridLayout* Layout;
    MAINFRAME* MainFrame;
    BREADCRUMB* BreadCrumb;
    FOOTER* Footer;
    HEADER* Header;

};

#endif
