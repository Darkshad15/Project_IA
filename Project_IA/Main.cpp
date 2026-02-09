#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "GameState.h"
#include "Background.h"
#include <iostream>
#include "Player.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "STREAT FIGHTER !");
    window.setFramerateLimit(60);

    // Initialiser le menu
    Menu menu;
    if (!menu.loadFont("../Assets/Fonts/arialbd.ttf"))
    {
        std::cerr << "Erreur: Impossible de charger la police!" << std::endl;
        return -1;
    }

    menu.setState(GameState::MAIN_MENU);

    // Initialiser le background
    Background* background = new Background();

    if (!background->loadFromFile("../Assets/Stage_Layers/MainMenu.png"))
    {
        std::cerr << "Attention: Impossible de charger le background de jeu! (../Assets/Stage_Layers/MainMenu.png)" << std::endl;
    }
    else
    {
        std::cout << "Background de jeu charge avec succes!" << std::endl;
    }

    // Initialiser le joueur
    Player* player = nullptr;

    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "STREAT FIGHTER !");
 
    window.setFramerateLimit(60);

    Player* player = new Player();
    player->Init();

    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // ========================
        // Gestion des événements
        // ========================
        float deltaTime = clock.restart().asSeconds();
 
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
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