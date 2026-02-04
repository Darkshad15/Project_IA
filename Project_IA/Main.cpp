#include <SFML/Graphics.hpp>
#include "GameState.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1960, 1080 }), "STREAT FIGHTER !");
    Menu* menu = new Menu();

    if (!menu->loadFont("../Assets/Fonts/arialbd.ttf")) {
        return -1;  // Quitter si erreur
    }
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        menu->draw(window); 
        window.display();
    }
}