#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "GameState.h"
#include "Background.h"

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

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // ========================
        // Gestion des événements
        // ========================
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Gestion clavier
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                GameState currentState = menu.getState();

                if (currentState != GameState::PLAYING)
                {
                    if (keyPressed->code == sf::Keyboard::Key::Enter ||
                        keyPressed->code == sf::Keyboard::Key::Space)
                    {
                        int selected = menu.getSelectedOption();

                        if (currentState == GameState::MAIN_MENU)
                        {
                            if (selected == 0)
                            {
                                menu.setState(GameState::PLAYING);

                                if (player == nullptr)
                                {
                                    player = new Player();
                                    player->Init();
                                }
                            }
                            else if (selected == 1)
                            {
                                window.close();
                            }
                        }
                        else if (currentState == GameState::PAUSED)
                        {
                            if (selected == 0)
                                menu.setState(GameState::PLAYING);
                            else if (selected == 2)
                                window.close();
                        }
                        else if (currentState == GameState::GAME_OVER)
                        {
                            if (selected == 0)
                            {
                                menu.setState(GameState::PLAYING);

                                if (player != nullptr)
                                    delete player;

                                player = new Player();
                                player->Init();
                            }
                            else if (selected == 1)
                            {
                                window.close();
                            }
                        }
                    }
                }
                else
                {
                    if (keyPressed->code == sf::Keyboard::Key::Escape)
                    {
                        menu.setState(GameState::PAUSED);
                    }
                }
            }

            // Gestion souris
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                sf::Vector2f mousePos(
                    static_cast<float>(mouseMoved->position.x),
                    static_cast<float>(mouseMoved->position.y)
                );
                menu.handleMouseMove(mousePos);
            }

            if (const auto* mouseClicked = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseClicked->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos(
                        static_cast<float>(mouseClicked->position.x),
                        static_cast<float>(mouseClicked->position.y)
                    );

                    menu.handleMouseClick(mousePos);

                    GameState currentState = menu.getState();
                    int selected = menu.getSelectedOption();

                    if (currentState == GameState::MAIN_MENU)
                    {
                        if (selected == 0)
                        {
                            menu.setState(GameState::PLAYING);

                            if (player == nullptr)
                            {
                                player = new Player();
                                player->Init();
                            }
                        }
                        else if (selected == 1)
                        {
                            window.close();
                        }
                    }
                }
            }
        }

        // ========================
        // Update
        // ========================
        if (menu.getState() == GameState::PLAYING && player != nullptr)
        {
            background->update();
            player->Update(deltaTime);
        }

        // ========================
        // Draw
        // ========================
        window.clear();

        if (menu.getState() == GameState::PLAYING)
        {
            background->draw(window);

            if (player != nullptr)
                player->Draw(window);
        }

        menu.draw(window);

        window.display();
    }

    // Nettoyage
    if (player != nullptr)
        delete player;

    delete background;

    return 0;
}
