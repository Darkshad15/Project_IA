#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct Vector2
{
	float x, y;
};

class Npc;

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

	// Données de patrouille
	std::vector<Vector2> patrolWaypoints;
	float patrolSpeed = 1.0f;
	float patrolWaitTime = 0.01f;
};

