//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include "DAT.hpp"


#include "lister.hpp"

using namespace std;
using namespace sf;

int main(int argc, char **argv)
{
    //QApplication App(argc, argv);
    if(argc > 2)
    {

        if(argc < 3)
        {
            cerr<<"Il manque le nom du fichier destination"<<endl<<"Forme 'dossier_source' 'destination'"<<endl;
            return false;
        }

        if(argv[1]=="./")
        {
            cerr<<"Le paramètre doit etre différent de './'"<<endl;
            return false;
        }

        cout<<"Création du fichier DAT en cour"<<endl;
        DAT write_test((string)argv[2]+".dat",LISTEUR::lister_rec(argv[1],NULL));



       if (!write_test.Create())
            return false;
        return true;
    }
     cerr<<"Il manque le dossier source en paramètre et le nom du fichier destination"<<endl<<"Forme 'dossier_source' 'destination'"<<endl;


    DAT write_test("ressources.dat",LISTEUR::lister_rec("ressources",NULL));
    write_test.Create();

    DAT read_test("ressources.dat");


    //Wouh ! Let's go test it in SFML now !

    //SFML variables
    RenderWindow App(sf::VideoMode(800, 600, 32), "SFML Demo");
    Event Event;
    Image i1;
    Sprite s1;
    Music m;
    Music m2;


    read_test.Load(&m,"./ressources/audio/CombatI.ogg");
    read_test.Load(&m2,"ressources/audio/Waterfall01.ogg");
    read_test.Load(&i1,"ressources/cb.bmp");

    //SFML instructions
    s1.SetImage(i1);
    //We try the LoadFromFile function to ensure images are the same

    //SFML instructions
    m.SetLoop(true);
    m2.SetLoop(true);
    m2.Play();
    m.Play();
    bool Running = true;

    //Main loop
    while (Running)
    {
        //Manage Events
        while (App.PoolEvent(Event))
        {
            // Window closed
            if (Event.Type == sf::Event::Closed)
            {
                Running = false;
            }

            //Key pressed
            if (Event.Type == sf::Event::KeyPressed)
            {
                switch (Event.Key.Code)
                {
                    case sf::Key::Escape :
                        Running = false;
                        break;
                    //Pause/Unpause music
                    case sf::Key::Space :
                        if (m.GetStatus() == sf::Music::Paused)
                            m.Play();
                        else
                            m.Pause();
                        break;
                    case sf::Key::A:
                         if (m2.GetStatus() == sf::Music::Paused)
                            m2.Play();
                        else
                            m2.Pause();
                        break;
                    default :
                        break;
                }
            }
        }
        //Drawing sprites using Image from memory and image from a file
        App.Draw(s1);
        App.Display();
    }


    return(EXIT_SUCCESS);
   // return App.exec();

};
