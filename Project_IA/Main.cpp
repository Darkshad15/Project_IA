#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "NPC.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "STREAT FIGHTER !");
 
    window.setFramerateLimit(60);

    Player* player = new Player();
    player->Init();
    

    Npc* npc = new Npc();
    npc->Init();


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

        npc->Update(deltaTime);

        const NpcContext& ctx = npc->GetContext();

        window.clear();

        player->Draw(window);

        sf::CircleShape debugCircle(10);
        debugCircle.setFillColor(sf::Color::Red);
        debugCircle.setPosition({ ctx.position.x - 10, ctx.position.y - 10 });
        window.draw(debugCircle);


 
        npc->Draw(window);
        window.display();
    }

    //delete player;
    return 0;
}