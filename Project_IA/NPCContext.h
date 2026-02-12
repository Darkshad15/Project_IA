#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct Vector2
{
	float x, y;
};

class Npc;
class Player;

enum class SpriteState
{
	IDLE,
	WALK,
	PUNCH,
	HURT
};

class NpcContext
{
public :
    Npc* npc = nullptr;
    Vector2 position = { 0.f, 0.f };
    Vector2 velocity = { 0.f, 0.f };
    int currentWaypointIndex = 0;
    float waitTimer = 0.f;
    bool isWaiting = false;
    sf::Sprite* sprite = nullptr;
    SpriteState currentSpriteState = SpriteState::IDLE;
    Player* player = nullptr;
    float lostPlayerTimer = 0.f;
    Vector2 lastKnownPlayerPosition = { 0.f, 0.f };
    float deltaTime = 0.f;

    bool facingRight = false;
    float visionRange = 0.f;   
    float visionAngle = 0.f;        

};



