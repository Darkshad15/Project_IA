#include "GameState.h"
#include <iostream>






void Menu::setupMenu(const std::string& title, sf::Color titleColor, const std::vector<std::string>& options)
{
    // Background (commun)
    if (!backgroundTexture.loadFromFile("Assets/Stage Layers/back.png"))
    {
        std::cerr << "Erreur: Impossible de charger le background!" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Clear
    buttonShapes.clear();
    menuOptions.clear();

    // Titre
    titleText.emplace(font);
    titleText->setString(title);
    titleText->setCharacterSize(80);
    titleText->setFillColor(titleColor);
    titleText->setPosition(sf::Vector2f(960.0f - titleText->getLocalBounds().size.x / 2.0f, 200.0f));

    // Options
    for (size_t i = 0; i < options.size(); i++)
    {
        std::optional<sf::Text> text;
        text.emplace(font);
        text->setString(options[i]);
        text->setCharacterSize(50);
        text->setFillColor(i == 0 ? selectedColor : normalColor);

        float yPos = 450.0f + i * 100.0f;
        text->setPosition(sf::Vector2f(960.0f - text->getLocalBounds().size.x / 2.0f, yPos));

        // Bouton rectangle
        sf::RectangleShape button(sf::Vector2f(400.0f, 80.0f));
        button.setPosition(sf::Vector2f(760.0f, yPos - 10.0f));
        button.setFillColor(sf::Color(50, 50, 50, 150));
        button.setOutlineThickness(3.0f);
        button.setOutlineColor(i == 0 ? selectedColor : normalColor);

        buttonShapes.push_back(button);
        menuOptions.push_back(std::move(text));
    }
}

void Menu::setupMainMenu()
{
    setupMenu("STREAT FIGHTER !", sf::Color::Cyan, { "START GAME", "QUIT" });
}

void Menu::setupEndScreen()
{
    setupMenu("GAME OVER", sf::Color::Red, { "NEW GAME", "QUIT" });
}