#include "Background.h"
#include <SFML/Graphics.hpp>
#include <iostream>


Background::Background()
    : sprite(background),
    scrollSpeed(1.0f),
    offsetX(0.0f),
    offsetY(0.0f),
    isLoaded(false),
    windowWidth(800.0f),
    windowHeight(600.0f)
{
}

void Background::Init(const std::string& filepath)
{
    std::cout << "=== Début Background::Init() ===" << std::endl;

    if (!background.loadFromFile(filepath))
    {
        std::cerr << "Erreur: Impossible de charger le Background du Menu" << std::endl;
        isLoaded = false;
        return;
    }

    sprite.setTexture(background);

    // Définir le rect en fonction de la taille réelle de la texture
    sf::Vector2u textureSize = background.getSize();
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { static_cast<int>(textureSize.x), static_cast<int>(textureSize.y) }));

    isLoaded = true;

    updateScale();

    std::cout << "Background chargé avec succés!" << std::endl;
    std::cout << "=== Fin Background::Init" << std::endl;

}

bool Background::loadFromFile(const std::string& filepath)
{
    if (!background.loadFromFile("../Assets/Background/MainMenu.png"))
    {
        std::cerr << "ERREUR: Impossible de charger le Background" << std::endl;
        return false;
    }
    sprite.setTexture(background);
    isLoaded = true;
    return true;
}

void Background::setWindowSize(float width, float height)
{
    windowWidth = width;
    windowHeight = height;
    updateScale();
}

void Background::updateScale()
{
    if (!isLoaded)
        return;

    // Obtenir la taille de la texture
    sf::Vector2u textureSize = background.getSize();

    // Calculer le ratio pour couvrir toute la fenétre
    float scaleX = windowWidth / static_cast<float>(textureSize.x);
    float scaleY = windowHeight / static_cast<float>(textureSize.y);

    // Utiliser le plus grand ratio pour que l'image couvre toute la fenétre
    float scale = std::max(scaleX, scaleY);

    sprite.setScale({ scale, scale });

    // Centrer l'image
    float scaledWidth = textureSize.x * scale;
    float scaledHeight = textureSize.y * scale;
    float centeredOffsetX = (windowWidth - scaledWidth) / 2.0f;
    float centeredOffsetY = (windowHeight - scaledHeight) / 2.0f;

    sprite.setPosition({ centeredOffsetX, centeredOffsetY });

    std::cout << "Background redimensionné - Scale: " << scale << std::endl;
}

void Background::update()
{
    if (isLoaded)
    {
        // Défilement vers la droite
        offsetX += scrollSpeed;

        // Reset quand on dépasse la hauteur de l'écran
        if (offsetX <= 1080.0f)
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