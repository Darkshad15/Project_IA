#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "GameState.h"
#include "Background.h"


int main()
{

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "STREAT FIGHTER !");
    window.setFramerateLimit(60);


    Menu menu;
    Background menubackground;
    Background gamebackground;


    Player* player = new Player();

    menu.Init();
    menubackground.Init("../Assets/Background/MainMenu.png");
    gamebackground.Init("../Assets/Background/GameMap.png");

    player->Init();

    sf::Vector2u windowSize = window.getSize();
    menu.setWindowSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    menubackground.setWindowSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    gamebackground.setWindowSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));



    menu.setState(GameState::MAIN_MENU);

    bool isRunning = true;

    while (window.isOpen() && isRunning)
    {
        float deltaTime = clock.restart().asSeconds();

        // Gestion des événements
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                isRunning = false;
            }

            if (event->is<sf::Event::Resized>())
            {
                const auto& resizeEvent = event->getIf<sf::Event::Resized>();

                // Mettre à jour la vue pour éviter la déformation
                sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f) , sf::Vector2f(static_cast<float>(resizeEvent->size.x), static_cast<float>(resizeEvent->size.y)));
                window.setView(sf::View(visibleArea));

                // Informer les objets de la nouvelle taille
                menu.setWindowSize(static_cast<float>(resizeEvent->size.x), static_cast<float>(resizeEvent->size.y));
                menubackground.setWindowSize(static_cast<float>(resizeEvent->size.x), static_cast<float>(resizeEvent->size.y));
            }

            // Gestion souris
            if (event->is<sf::Event::MouseMoved>())
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                menu.handleMouseMove(sf::Vector2f(static_cast<float>(mousePos.x),
                    static_cast<float>(mousePos.y)));
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    menu.handleMouseClick(sf::Vector2f(static_cast<float>(mousePos.x),
                        static_cast<float>(mousePos.y)));

                    // Traiter la sélection
                    if (menu.getState() == GameState::MAIN_MENU)
                    {
                        if (menu.getSelectedOption() == 0) // START GAME
                        {
                            menu.setState(GameState::PLAYING);
                        }
                        else if (menu.getSelectedOption() == 1) // QUIT
                        {
                            window.close();
                            isRunning = false;
                        }
                    }
                    else if (menu.getState() == GameState::PAUSED)
                    {
                        if (menu.getSelectedOption() == 0) // RESUME
                        {
                            menu.setState(GameState::PLAYING);
                        }
                        else if (menu.getSelectedOption() == 2) // QUIT
                        {
                            menu.setState(GameState::MAIN_MENU);
                        }
                    }
                    else if (menu.getState() == GameState::GAME_OVER)
                    {
                        if (menu.getSelectedOption() == 0) // NEW GAME
                        {
                            menu.setState(GameState::PLAYING);
                        }
                        else if (menu.getSelectedOption() == 1) // QUIT
                        {
                            window.close();
                            isRunning = false;
                        }
                    }
                }
            }

            // Gestion clavier
            if (event->is<sf::Event::KeyPressed>())
            {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();

                // Navigation dans les menus
                menu.handleInput(keyEvent->code);

                // Entrée pour valider
                if (keyEvent->code == sf::Keyboard::Key::Enter)
                {
                    if (menu.getState() == GameState::MAIN_MENU)
                    {
                        if (menu.getSelectedOption() == 0) // START GAME
                        {
                            menu.setState(GameState::PLAYING);
                        }
                        else if (menu.getSelectedOption() == 1) // QUIT
                        {
                            window.close();
                            isRunning = false;
                        }
                    }
                    else if (menu.getState() == GameState::PAUSED)
                    {
                        if (menu.getSelectedOption() == 0) // RESUME
                        {
                            menu.setState(GameState::PLAYING);
                        }
                        else if (menu.getSelectedOption() == 2) // QUIT
                        {
                            menu.setState(GameState::MAIN_MENU);
                        }
                    }
                    else if (menu.getState() == GameState::GAME_OVER)
                    {
                        if (menu.getSelectedOption() == 0) // NEW GAME
                        {
                            menu.setState(GameState::PLAYING);
                        }
                        else if (menu.getSelectedOption() == 1) // QUIT
                        {
                            window.close();
                            isRunning = false;
                        }
                    }
                }

                // Échap pour pause
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    if (menu.getState() == GameState::PLAYING)
                    {
                        menu.setState(GameState::PAUSED);
                    }
                    else if (menu.getState() == GameState::PAUSED)
                    {
                        menu.setState(GameState::PLAYING);
                    }
                }
            }
        }

        // Update selon l'état du jeu
        if (menu.getState() == GameState::PLAYING)
        {
            player->Update(deltaTime);
            menubackground.update();
        }
        else
        {
            // Update du background même dans les menus pour l'animation
            gamebackground.update();
        }

        // Rendu
        window.clear();

        if (menu.getState() == GameState::PLAYING)
        {
            // Mode jeu : afficher background et joueur
            gamebackground.draw(window);
            player->Draw(window);
        }
        else
        {
            // Mode menu : afficher background et menu
            menubackground.draw(window);
            menu.draw(window);
        }

        window.display();
    }

        delete player;
        return 0;
}