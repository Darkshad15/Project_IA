#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include "NPCContext.h"


class Player 
{
public:
    enum class State
    {
        IDLE,
        WALK,
        HURT,
        PUNCH
    };

private:

    std::map<State, sf::Texture> textures;
    sf::Sprite* sprite;

    State currentState;
    int currentFrame;
    float animationTimer;
    float animationSpeed;

    int frameCount;
    int frameWidth;
    int frameHeight;

    sf::Vector2f velocity;
    bool facingRight;

    Vector2 position;

    sf::RectangleShape hitboxShape;
    bool showHitbox; 

public:


    Player();
    ~Player();

	void Init();
	void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
	void HandleInput();
    void ChangeState(State newState);
    void UpdateAnimation(float deltaTime);

    
    void DrawHitbox(sf::RenderWindow & window);
    void ToggleHitbox();
    sf::FloatRect GetHitbox() const;
    

    sf::Sprite& GetSprite() { return *sprite; }

    Vector2 Getposition() const { return position; }



};

