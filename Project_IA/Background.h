#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Background 
{
private:
    sf::Texture* texture;  // pointeur vers texture chargée
    sf::Sprite* sprite;
    bool isLoaded;

public:
    Background() : sprite(nullptr), isLoaded(false) {}

    bool loadFromFile(const std::string& filepath)
    {
        if (!texture->loadFromFile(filepath))
            return false;

        sprite = new sf::Sprite(texture); // OK, pointer
        isLoaded = true;
        return true;
    }

    void draw(sf::RenderWindow& window)
    {
        if (isLoaded && sprite)
            window.draw(*sprite);
    }

    ~Background()
    {
        if (sprite) delete sprite;
    }
};

