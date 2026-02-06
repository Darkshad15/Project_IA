#include "Background.h"
#include <iostream>

static constexpr float WINDOW_WIDTH = 1920.f;
static constexpr float WINDOW_HEIGHT = 1080.f;

Background::Background()
    : sprite(background),
    scrollSpeed(1.0f),
    offsetY(0.0f),
    isLoaded(false)
{
}

bool Background::loadFromFile(const std::string& filepath)
{
    if (!background.loadFromFile(filepath))
    {
        std::cerr << "ERREUR: Impossible de charger le Background: "
            << filepath << std::endl;
        return false;
    }

    sprite.setTexture(background);

    // 🔥 SCALE pour remplir l'écran
    sf::Vector2u texSize = background.getSize();
    float scaleX = WINDOW_WIDTH / texSize.x;
    float scaleY = WINDOW_HEIGHT / texSize.y;
    sprite.setScale({ scaleX, scaleY });

    sprite.setPosition({ 0.f, 0.f });

    isLoaded = true;
    return true;
}

void Background::update()
{
    if (!isLoaded)
        return;

    offsetY += scrollSpeed;

    float height = sprite.getGlobalBounds().size.y;

    if (offsetY >= height)
        offsetY = 0.f;

    sprite.setPosition({ 0.f, offsetY });
}

void Background::draw(sf::RenderWindow& window)
{
    if (!isLoaded)
        return;

    // Sprite principal
    window.draw(sprite);

    // Deuxième sprite pour défilement infini
    sf::Vector2f pos = sprite.getPosition();
    sprite.setPosition({ 0.f, pos.y - sprite.getGlobalBounds().size.y });
    window.draw(sprite);

    // Remise en place
    sprite.setPosition(pos);
}

void Background::setScrollSpeed(float speed)
{
    scrollSpeed = speed;
}
