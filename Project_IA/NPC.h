#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include "StateMachine.h"

#include "Conditions.h"
#include "ChaseState.h"
#include "PatrolState.h"

using namespace NpcAi;


class Npc
{

private:

	FSM::StateMachine<NpcContext&> fsm;
	NpcContext context{};

    std::map<SpriteState, sf::Texture> textures;
    sf::Sprite sprite;


    // Animation
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int frameCount;
    float animationTimer;
    float animationSpeed;


    bool LoadTextures()
    {
        bool idle = textures[SpriteState::IDLE].loadFromFile("../Assets/Spritesheets/Enemy_Punk/idle.png");
        bool walk = textures[SpriteState::WALK].loadFromFile("../Assets/Spritesheets/Enemy_Punk/walk.png");
        bool punch = textures[SpriteState::PUNCH].loadFromFile("../Assets/Spritesheets/Enemy_Punk/punch.png");
        bool hurt = textures[SpriteState::HURT].loadFromFile("../Assets/Spritesheets/Enemy_Punk/hurt.png");

        if (!idle || !walk || !punch || !hurt)
        {
            std::cerr << "Erreur: Impossible de charger une ou plusieurs textures!" << std::endl;
            return false;
        }

        return true;
    }


public:

    Npc();

    Npc(const Npc&) = delete;
    Npc& operator=(const Npc&) = delete;

    void Init();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    void SetSpriteState(SpriteState state);
    void UpdateAnimation(float deltaTime);

    sf::Sprite& GetSprite() { return sprite; }
    const NpcContext& GetContext() const { return context; }

};



