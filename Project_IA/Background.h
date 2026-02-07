#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Background
{
public:
    Background();

    bool loadFromFile(const std::string& filepath);

    void update();
    void draw(sf::RenderWindow& window);

    void setPosition(float x);

    void move(float offsetX);

    void setScrollSpeed(float speed);

private:
    sf::Texture background;
    sf::Sprite sprite;

    float scrollSpeed;
    float offsetX; // Ajout du membre offsetX pour corriger l'erreur
    float offsetY;
    bool isLoaded;
};
