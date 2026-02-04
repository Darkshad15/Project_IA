#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "STREAT FIGHTER !");
 
    window.setFramerateLimit(60);

    Player* player = new Player();
    player->Init();

    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
 
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        player->Update(deltaTime);


        window.clear();

        player->Draw(window);



        window.display();
    }

    delete player;
    return 0;
}