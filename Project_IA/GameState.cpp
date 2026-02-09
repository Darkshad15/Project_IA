#include "GameState.h"
#include <iostream>

Menu::Menu()

    :titleText(font),
    currentState(GameState::MAIN_MENU),
    selectedOption(0),
    normalColor(sf::Color::White),
    selectedColor(sf::Color::Blue),
    backgroundColor(sf::Color(0, 0, 0, 150)),
    fontLoaded(false),
    windowWidth(800.0f),
    windowHeight(600.0f)

{
}

void Menu::Init()
{
    std::cout << "=== Début Menu::Init() ===" << std::endl;

    if (!font.openFromFile("../Assets/Fonts/arialbd.ttf"))
    {
        std::cerr << "Erreur: Impossible de charger la Police" << std::endl;
        return;

    }
        fontLoaded = true;
        std::cerr << "Police chargée avec succès!" << std::endl;

        setupMainMenu();

        std::cerr << "=== Fin Menu::Init() ===" << std::endl;
}

void Menu::setWindowSize(float width, float height)
{
    windowWidth = width;
    windowHeight = height;

    switch (currentState)
    {
    case GameState::MAIN_MENU:
        setupMainMenu();
        break;
    case GameState::PAUSED:
        setupPauseMenu();
        break;
        case GameState::GAME_OVER:
            setupEndScreen();
            break;
        default:
            break;
    }
}

void Menu::setupMenu(const std::string& title, sf::Color titleColor, const std::vector<std::string>& options)
{
    // Clear
    buttonShapes.clear();
    menuOptions.clear();

    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;


    // Titre
    if (fontLoaded)
    {
        titleText.setString(title);
        titleText.setCharacterSize(80);
        titleText.setFillColor(titleColor);
        titleText.setPosition(sf::Vector2f(960.0f - titleText.getLocalBounds().size.x / 2.0f, 200.0f));

        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f));
        titleText.setPosition(sf::Vector2f(centerX, centerY - 150.0f));

    }

    // Options
    if (fontLoaded)
    { 
        float buttonWidth = 400.0f;
        float buttonHeight = 80.0f;
        float buttonSpacing = 100.0f;

        // Calculer la position de départ pour centrer verticalement tous les boutons
        float totalHeight = options.size() * buttonHeight + (options.size() - 1) * (buttonSpacing - buttonHeight);
        float startY = centerY - totalHeight / 2.0f;


        for (size_t i = 0; i < options.size(); i++)
        {
            sf::Text text(font);
            text.setString(options[i]);
            text.setCharacterSize(50);
            text.setFillColor(i == 0 ? selectedColor : normalColor);

            float yPos = startY + i * buttonSpacing;

            // *** CENTRAGE DU TEXTE ***
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
            text.setPosition(sf::Vector2f(centerX, yPos + buttonHeight / 2.0f));

            // *** BOUTON CENTRÉ ***
            sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
            button.setOrigin(sf::Vector2f(buttonWidth / 2.0f, buttonHeight / 2.0f));
            button.setPosition(sf::Vector2f(centerX, yPos + buttonHeight / 2.0f));
            button.setFillColor(sf::Color(50, 50, 50, 150));
            button.setOutlineThickness(3.0f);
            button.setOutlineColor(i == 0 ? selectedColor : normalColor);

            buttonShapes.push_back(button);
            menuOptions.push_back(text);
        }
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

    case GameState::DIDACTICIEL:
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
 

    // Fond semi-transparent
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
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
