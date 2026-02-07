#include "Background.h"
#include <iostream>
#include "GameState.h"

Background::Background() : scrollSpeed(1.0f), offsetX(0.0f)
{
    //
}




bool Background::loadFromFile(const std::string& filepath)
{
    if (background.loadFromFile("../Assets/Previews/Stage/preview_stage.png"))

    {
        std::cerr << "ERREUR: Impossible de charger le Background" << std::endl;
        return false;
    }
    sprite.emplace(background);

    return true;
}



void Background::update()
{
    if (sprite.has_value())
    {
        // Défilement vers le bas
        offsetX += scrollSpeed;

        // Reset quand on dépasse la hauteur de l'écran
        if (offsetX >= 1080.0f)
        {
            offsetX = 0.0f;
        }

        sprite->setPosition({ 0.0f, offsetX });
    }
}





void Background::draw(sf::RenderWindow& window)
{
    if (sprite.has_value())
    {
        // Dessiner le sprite principal
        window.draw(sprite.value());

        // Dessiner une copie au-dessus pour un défilement continu
        sf::Vector2f currentPos = sprite->getPosition();
        sprite->setPosition({ 0.0f, currentPos.y - 1080.0f });
        window.draw(sprite.value());

        // Remettre la position originale
        sprite->setPosition(currentPos);
    }
}

void Background::setPosition(float x)
{
    if (sprite.has_value())
    {
        sprite->setPosition({ x, 0.0f });
    }
}

void Background::move(float offsetX)
{
    if (sprite.has_value())
    {
        sprite->move({ offsetX, 0.0f });
    }
}

void Background::setScrollSpeed(float speed)
{
    scrollSpeed = speed;
}