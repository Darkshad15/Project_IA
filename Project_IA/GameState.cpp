#include "GameState.h"
#include <iostream>

Menu::Menu()
    : backgroundSprite(backgroundTexture),  // Initialiser avec la texture
    titleText(font),  // Initialiser avec la font
    currentState(GameState::MAIN_MENU),
    selectedOption(0),
    normalColor(sf::Color::White),
    selectedColor(sf::Color::Blue),
    backgroundColor(sf::Color(0, 0, 0, 150)),
    backgroundLoaded(false),
    fontLoaded(false)
{
}

void Menu::setupMenu(const std::string& title, sf::Color titleColor, const std::vector<std::string>& options)
{
    // Background (commun)
    if (!backgroundLoaded)
    {
        if (backgroundTexture.loadFromFile("../Assets/Stage_Layers/back.png"))
        {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundLoaded = true;
        }
        else
        {
            std::cerr << "Erreur: Impossible de charger le background!" << std::endl;
        }
    }

    // Clear
    buttonShapes.clear();
    menuOptions.clear();

    // Titre
    if (fontLoaded)
    {
        titleText.setString(title);
        titleText.setCharacterSize(80);
        titleText.setFillColor(titleColor);
        titleText.setPosition(sf::Vector2f(960.0f - titleText.getLocalBounds().size.x / 2.0f, 200.0f));
    }

    // Options
    for (size_t i = 0; i < options.size(); i++)
    {
        sf::Text text(font);
        text.setString(options[i]);
        text.setCharacterSize(50);
        text.setFillColor(i == 0 ? selectedColor : normalColor);

        float yPos = 450.0f + i * 100.0f;
        text.setPosition(sf::Vector2f(960.0f - text.getLocalBounds().size.x / 2.0f, yPos));

        // Bouton rectangle
        sf::RectangleShape button(sf::Vector2f(400.0f, 80.0f));
        button.setPosition(sf::Vector2f(760.0f, yPos - 10.0f));
        button.setFillColor(sf::Color(50, 50, 50, 150));
        button.setOutlineThickness(3.0f);
        button.setOutlineColor(i == 0 ? selectedColor : normalColor);

        buttonShapes.push_back(button);
        menuOptions.push_back(text);
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

void Menu::setupPauseMenu()
{
    setupMenu("PAUSED", sf::Color::Yellow, { "RESUME", "HELP", "QUIT" });
}

bool Menu::loadFont(const std::string& fontPath)
{
    if (!font.openFromFile("../Assets/Fonts/arialbd.ttf"))
    {
        std::cerr << "ERREUR: Impossible de charger la police " << fontPath << std::endl;
        return false;
    }
    fontLoaded = true;
    titleText.setFont(font);  // Mettre à jour la font du titleText
    setupMainMenu();
    return true;
}

void Menu::setState(GameState state)
{
    currentState = state;
    selectedOption = 0;

    switch (state)
    {
    case GameState::MAIN_MENU:
        setupMainMenu();
        break;

    case GameState::PLAYING:
        // Pas de menu en jeu
        break;

    case GameState::PAUSED:
        setupPauseMenu();
        break;
    case GameState::GAME_OVER:
        setupEndScreen();
        break;
    }
}

void Menu::handleMouseMove(const sf::Vector2f& mousePos)
{
    if (currentState == GameState::PLAYING)
        return;

    for (size_t i = 0; i < buttonShapes.size(); i++)
    {
        if (buttonShapes[i].getGlobalBounds().contains(mousePos))
        {
            selectedOption = static_cast<int>(i);

            // Mettre à jour les couleurs
            for (size_t j = 0; j < menuOptions.size(); j++)
            {
                if (j < buttonShapes.size())
                {
                    menuOptions[j].setFillColor(j == i ? selectedColor : normalColor);
                    buttonShapes[j].setOutlineColor(j == i ? selectedColor : normalColor);
                }
            }
            break;
        }
    }
}

void Menu::handleInput(sf::Keyboard::Key key)
{
    if (currentState == GameState::PLAYING)
        return;

    int maxOptions = static_cast<int>(buttonShapes.size());

    if (key == sf::Keyboard::Key::Up || key == sf::Keyboard::Key::Z)
    {
        selectedOption--;
        if (selectedOption < 0)
            selectedOption = maxOptions - 1;
    }
    else if (key == sf::Keyboard::Key::Down || key == sf::Keyboard::Key::S)
    {
        selectedOption++;
        if (selectedOption >= maxOptions)
            selectedOption = 0;
    }

    // Mettre à jour les couleurs
    for (size_t i = 0; i < menuOptions.size(); i++)
    {
        if (i < buttonShapes.size())
        {
            menuOptions[i].setFillColor(i == selectedOption ? selectedColor : normalColor);
            buttonShapes[i].setOutlineColor(i == selectedOption ? selectedColor : normalColor);
        }
    }
}

void Menu::handleMouseClick(const sf::Vector2f& mousePos)
{
    if (currentState == GameState::PLAYING)
        return;

    for (size_t i = 0; i < buttonShapes.size(); i++)
    {
        if (buttonShapes[i].getGlobalBounds().contains(mousePos))
        {
            selectedOption = static_cast<int>(i);
            break;
        }
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    if (currentState == GameState::PLAYING)
        return;

    // Background
    if (backgroundLoaded)
    {
        window.draw(backgroundSprite);
    }

    // Fond semi-transparent
    sf::RectangleShape overlay(sf::Vector2f(1920.0f, 1080.0f));
    overlay.setFillColor(backgroundColor);
    window.draw(overlay);

    // Titre
    if (fontLoaded)
    {
        window.draw(titleText);
    }

    // Boutons et textes
    for (size_t i = 0; i < buttonShapes.size(); i++)
    {
        window.draw(buttonShapes[i]);
    }

    for (auto& text : menuOptions)
    {
        window.draw(text);
    }
}
