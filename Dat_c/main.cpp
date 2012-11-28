#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "DAT.hpp"

using namespace std;

int main()
{
    //Variables for creation
    DAT write_test;
    std::vector<std::string> tab_fichiers_nom;

    //Let's creating a DAT file
    tab_fichiers_nom.push_back("cb.bmp");
    tab_fichiers_nom.push_back("CombatI.ogg");
    if (!write_test.Create(tab_fichiers_nom, "data.dat"))
    {
        return EXIT_FAILURE;
    }
    //Ok, we got a magnific DAT file named "data.dat"

    //Now, let's try to read it
    DAT read_test;
    char* buffer;

    //Getting files informations
    read_test.Read("data.dat");

    //Wouh ! Let's go test it in SFML now !

    //SFML variables
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML Demo");
    sf::Event Event;
    sf::Image i1;
    sf::Sprite s1;
    sf::Music m;
    bool Running = true;

    //Let's read the image file
    buffer = read_test.GetFile("cb.bmp");
    if (buffer==NULL)
        return EXIT_FAILURE;
    //Ok, now we can load the image by using memory instead of a file
    i1.LoadFromMemory(buffer, read_test.GetFileSize("cb.bmp"));

    //SFML instructions
    s1.SetImage(i1);
    //We try the LoadFromFile function to ensure images are the same

    //Now, let's get the music file
    buffer = new char;
    buffer=read_test.GetFile("CombatI.ogg");
    if (buffer==NULL)
        return EXIT_FAILURE;
    //Ok, now we can load the music by using memory instead of a file
    m.OpenFromMemory(buffer, read_test.GetFileSize("CombatI.ogg"));
    //WARNING, deleting the buffer causes app to crash because
    //the sf::Music keep a link on our memory
    //You must make a copy of memory or not load any more files

    //SFML instructions
    m.SetLoop(true);
    m.Play();

    //Main loop
    while (Running)
    {
        //Manage Events
        while (App.GetEvent(Event))
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
                        {
                            m.Play();
                        }
                        else
                        {
                            m.Pause();
                        }
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

    return EXIT_SUCCESS;
};
