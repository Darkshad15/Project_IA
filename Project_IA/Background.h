#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Background
{
public:
    Background();

    bool loadFromFile(const std::string& filepath);

    void update();
    void draw(sf::RenderWindow& window);

    void setScrollSpeed(float speed);
    void setPosition(float x);
    void move(float offsetX);

private:
    sf::Texture background;
    std::optional<sf::Sprite> sprite;

    float scrollSpeed;
    float offsetX;
};