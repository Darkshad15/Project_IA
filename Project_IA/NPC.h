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
    sf::Sprite* sprite;


    //AJOUTER CECI pour la zone de vision
    sf::CircleShape visionCircle;
    sf::ConvexShape visionCone;
    bool showVisionDebug;


    // Paramètres de vision
    float visionRange;
    float visionAngle;


    // Animation
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int frameCount;
    float animationTimer;
    float animationSpeed;
    bool facingRight;

public:

    Npc();
    ~Npc();

    Npc(const Npc&) = delete;
    Npc& operator=(const Npc&) = delete;

    void Init();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    void SetSpriteState(SpriteState state);
    void UpdateAnimation(float deltaTime);

    sf::Sprite& GetSprite() { return *sprite; }
    const NpcContext& GetContext() const { return context; }


    void SetPlayer(Player* player);

    void DrawVisionDebug(sf::RenderWindow& window);
    void ToggleVisionDebug();




};



