#include "Background.h"
#include <iostream>

Background::Background()
    : sprite(background),
      scrollSpeed(1.0f),
      offsetX(0.0f),
      offsetY(0.0f), // Initialisation explicite de offsetY
      isLoaded(false)
{
}

bool Background::loadFromFile(const std::string& filepath)
{
    if (!background.loadFromFile(filepath))
    {
        std::cerr << "ERREUR: Impossible de charger le Background" << std::endl;
        return false;
    }
    sprite.setTexture(background);
    isLoaded = true;
    return true;
}

void Background::update()
{
    if (isLoaded)
    {
        // Défilement vers le bas
        offsetX += scrollSpeed;

        // Reset quand on dépasse la hauteur de l'écran
        if (offsetX >= 1080.0f)
        {
            offsetX = 0.0f;
        }

        sprite.setPosition({ 0.0f, offsetX });
    }
}

void Background::draw(sf::RenderWindow& window)
{
    if (isLoaded)
    {
        // Dessiner le sprite principal
        window.draw(sprite);

        // Dessiner une copie au-dessus pour un défilement continu
        sf::Vector2f currentPos = sprite.getPosition();
        sprite.setPosition({ 0.0f, currentPos.y - 1080.0f });
        window.draw(sprite);

        // Remettre la position originale
        sprite.setPosition(currentPos);
    }
}

void Background::setPosition(float x)
{
    if (isLoaded)
    {
        sprite.setPosition({ x, 0.0f });
    }
}

void Background::move(float offsetX)
{
    if (Background::isLoaded)
    {
        sprite.move({ offsetX, 0.0f });
    }
}

void Background::setScrollSpeed(float speed)
{
    scrollSpeed = speed;
}
