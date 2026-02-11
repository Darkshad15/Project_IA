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
	Player* player = nullptr;

	Vector2 position = { 0.f, 0.f };
	Vector2 velocity = { 0.f, 0.f };

	int currentWaypointIndex = 0;
	float waitTimer = 0.f;
	bool isWaiting = false;

	sf::Sprite* sprite = nullptr;
	SpriteState currentSpriteState = SpriteState::IDLE;

	// Données de patrouille
	std::vector<Vector2> patrolWaypoints;
	float patrolSpeed = 1.0f;
	float patrolWaitTime = 0.01f;
	float deltaTime = 0.f;


	float lostPlayerTimer = 0.f;  // Timer depuis la dernière fois qu'on a vu le joueur
	Vector2 lastKnownPlayerPosition = { 0.f, 0.f };  // Dernière position connue du joueur
};



