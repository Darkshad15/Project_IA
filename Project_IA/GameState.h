#pragma once
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>
#include <vector>
#include <optional>

enum class GameState
{
    MAIN_MENU, //MENU
    PLAYING, // IN GAME
    PAUSED,
    DIDACTICIEL,
    GAME_OVER, //MENU
    VICTORY //MENU
};

class Menu
{
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool backgroundLoaded;

public:
    void Init()
    {
        if (backgroundTexture.loadFromFile("../Assets/Stage_Layers/MainMenu.png"))
        {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setScale({ 2.f, 2.f });
            backgroundSprite.setPosition({ 0.f, 0.f });
            backgroundLoaded = true;
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (backgroundLoaded)
            window.draw(backgroundSprite);
        // draw texte et boutons...
    }
};
