#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Background
{
public:
    Background();

    void Init();

    bool loadFromFile(const std::string& filepath);

    void update();
    void draw(sf::RenderWindow& window);

    void setPosition(float x);
    void move(float offsetX);
    void setScrollSpeed(float speed);

    void setWindowSize(float width, float height);

private:
    sf::Texture background;
    sf::Sprite sprite;

    float scrollSpeed;
    float offsetX; // Ajout du membre offsetX pour corriger l'erreur
    float offsetY;
    bool isLoaded;

    float windowWidth;
    float windowHeight;

    void updateScale();
};
