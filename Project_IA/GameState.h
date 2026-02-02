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
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    GameState currentState;

    // Textes
    std::optional<sf::Text> titleText;
    std::vector<std::optional<sf::Text>> menuOptions;
    int selectedOption;

    // Rectangles pour les boutons
    std::vector<sf::RectangleShape> buttonShapes;

    // Couleurs
    sf::Color normalColor;
    sf::Color selectedColor;

    void setupMainMenu();
    void setupEndScreen();
    void setupMenu(const std::string& title, sf::Color titleColor, const std::vector<std::string>& options);

public:
    Menu();

    bool loadFont(const std::string& fontPath);

    void setState(GameState state);
    GameState getState() const { return currentState; }

    void setScore(int score, int wave);

    void handleInput(sf::Keyboard::Key key);
    void handleMouseMove(const sf::Vector2f& mousePos);
    void handleMouseClick(const sf::Vector2f& mousePos);

    void draw(sf::RenderWindow& window);
};


/*
class Game
{

};

class Pause
{

};





class Tuto
{

};
*/