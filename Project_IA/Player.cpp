#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>


Player::Player() :
	sprite(nullptr),
	currentState(State::IDLE),
	currentFrame(0),
	animationTimer(0.f),
	animationSpeed(0.15f),
	frameWidth(64),
	frameHeight(64),
	frameCount(4),
	velocity(0.f, 0.f),
	facingRight(true)
{
	std::cout << "Player cree" << std::endl;
}


Player::~Player()
{
	std::cout << "Player detruit" << std::endl;
}

void Player::Init()
{
	std::cout << "=== Début Init() ===" << std::endl;

	// Charger toutes les textures
	bool idle = textures[State::IDLE].loadFromFile("../Assets/Spritesheets/Enemy_Punk/idle.png");
	bool walk = textures[State::WALK].loadFromFile("../Assets/Spritesheets/Enemy_Punk/walk.png");
	bool punch = textures[State::PUNCH].loadFromFile("../Assets/Spritesheets/Enemy_Punk/punch.png");
	bool hurt = textures[State::HURT].loadFromFile("../Assets/Spritesheets/Enemy_Punk/hurt.png");

	std::cout << "idle: " << idle << ", walk: " << walk
		<< ", punch: " << punch << ", hurt: " << hurt << std::endl;

	// État initial
	currentState = State::IDLE;
	currentFrame = 0;
	frameCount = 4;

	// Calculer dimensions
	sf::Vector2u textureSize = textures[currentState].getSize();
	frameWidth = textureSize.x / frameCount;
	frameHeight = textureSize.y;

	std::cout << "Taille texture complète: " << textureSize.x << "x" << textureSize.y << std::endl;
	std::cout << "Frame dimensions: " << frameWidth << "x" << frameHeight << std::endl;


	sprite = new sf::Sprite(textures[currentState]);
	sprite->setTexture(textures[currentState]);
	//sprite->setTextureRect(sf::IntRect({ 0, 0 }, { frameWidth, frameHeight }));
	sprite->setPosition({ 100.f, 100.f });
	sprite->setScale({ 2.f, 2.f });


	std::cout << "Sprite configuré sans TextureRect" << std::endl;
	std::cout << "=== Fin Init() ===" << std::endl;

	std::cout << "=== Fin Init() ===" << std::endl;
}

void Player::Update(float deltaTime)
{
	HandleInput();
	UpdateAnimation(deltaTime);
	sprite->move(velocity * deltaTime);

	velocity.x = 0.f;
}

void Player::HandleInput()
{
	float speed = 0.1f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		velocity.x = -speed;
		facingRight = true;

		if (currentState != State::WALK && currentState != State::PUNCH)
			ChangeState(State::WALK);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		velocity.x = speed;
		facingRight = false;

		if (currentState != State::WALK && currentState != State::PUNCH)
			ChangeState(State::WALK);
	}
	else
	{
		if (currentState == State::WALK)
			ChangeState(State::IDLE);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		if (currentState != State::PUNCH)
			ChangeState(State::PUNCH);
	}
}


void Player::ChangeState(State state)
{
	if (currentState == state)
		return;

	currentState = state;
	currentFrame = 0;
	animationTimer = 0.f;

	sprite->setTexture(textures[currentState]);

	switch (currentState)
	{
	case Player::State::IDLE:
		frameCount = 4;
		animationSpeed = 0.2f;
		break;
	case Player::State::WALK:
		frameCount = 4;
		animationSpeed = 0.1f;
		break;
	case Player::State::HURT:
		frameCount = 4;
		animationSpeed = 0.1f;
		break;
	case Player::State::PUNCH:
		frameCount = 3;
		animationSpeed = 0.05f;
		break;
	}
}

void Player::UpdateAnimation(float deltaTime)
{
	animationTimer += deltaTime;

	if (animationTimer >= animationSpeed)
	{
		animationTimer = 0.f;
		currentFrame++;

		if (currentFrame >= frameCount)
		{
			currentFrame = 0;

			if (currentState == State::PUNCH || currentState == State::HURT)
				ChangeState(State::IDLE);
		}
	}


	int left = currentFrame * frameWidth;

	sprite->setTextureRect(sf::IntRect({ left , 0 }, { frameWidth , frameHeight }));

	if (!facingRight)
	{
		sprite->setTextureRect(sf::IntRect({ left + frameWidth, 0 }, { -frameWidth, frameHeight }));
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	// Test 2: Votre sprite normal
	window.draw(*sprite);

}