//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include "DAT.hpp"

#include <QtGui>

#include "lister.hpp"

#include "TotalFrame.hpp"

using namespace std;
using namespace sf;

int main(int argc, char **argv)
{
    QApplication App(argc, argv);

    /*std::vector<string> files=LISTEUR::lister_rec("..",NULL);

    DAT write_test("data/t.dat");

    for(unsigned int i=0,_size=files.size();i<_size;++i)
        write_test.add(files[i]);
    write_test.Create(0);*/

    //On crée une vue SFML dans la fenêtre principale
    TOTALFRAME* SFMLView = new TOTALFRAME(0,QPoint(0,0),QSize(800,600));
    SFMLView->show();

    return App.exec();
}

