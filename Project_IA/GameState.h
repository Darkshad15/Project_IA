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
    sf::Font font;
    GameState currentState;

    // Textes
    sf::Text titleText;
    std::vector<sf::Text> menuOptions;
    int selectedOption;

    // Rectangles pour les boutons
    std::vector<sf::RectangleShape> buttonShapes;

    // Couleurs
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color backgroundColor;

    bool fontLoaded;

    float windowWidth;
    float windowHeight;


    void setupMainMenu();
    void setupEndScreen();
    void setupPauseMenu();
    void setupMenu(const std::string& title, sf::Color titleColor, const std::vector<std::string>& options);

public:
    Menu();

    void Init();

    void setWindowSize(float width, float height);

    void setState(GameState state);
    GameState getState() const 
    { 
        return currentState; 
    }

    void handleInput(sf::Keyboard::Key key);
    void handleMouseMove(const sf::Vector2f& mousePos);
    void handleMouseClick(const sf::Vector2f& mousePos);

    void draw(sf::RenderWindow& window);

    int getSelectedOption() const 
    { 
        return selectedOption; 
    }
};
