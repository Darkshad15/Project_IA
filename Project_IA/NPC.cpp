#include "NPC.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

Npc::Npc() :
    context(),
    sprite(nullptr),
    textures(),
    frameWidth(0),
    frameHeight(0),
    currentFrame(0),
    frameCount(4),
    animationTimer(0.f),
    animationSpeed(0.15f),
    facingRight(true),
    visionCircle(),
    visionCone(),
    //showVisionDebug(true),  // Activé par défaut
    visionRange(200.0f),
    visionAngle(90.0f)
{
    std::cout << "NPC cree" << std::endl;
    context.npc = nullptr;
    context.position = { 0.f, 0.f };
    context.velocity = { 0.f, 0.f };
    context.currentWaypointIndex = 0;
    context.waitTimer = 0.f;
    context.isWaiting = false;
    context.sprite = nullptr;
    context.currentSpriteState = SpriteState::IDLE;
    context.player = nullptr;
    context.lostPlayerTimer = 0.f;

    context.facingRight = !facingRight;
    context.visionRange = visionRange;
    context.visionAngle = visionAngle;
}

Npc::~Npc()
{
    delete sprite;  // Libérer la mémoire
    sprite = nullptr;
}


void Npc::Init()
{
    std::cout << "=== DEBUT Init() NPC ===" << std::endl;

    // Charger toutes les textures
    bool idle = textures[SpriteState::IDLE].loadFromFile("../Assets/Spritesheets/Enemy_Punk/idle.png");
    bool walk = textures[SpriteState::WALK].loadFromFile("../Assets/Spritesheets/Enemy_Punk/walk.png");
    bool punch = textures[SpriteState::PUNCH].loadFromFile("../Assets/Spritesheets/Enemy_Punk/punch.png");
    bool hurt = textures[SpriteState::HURT].loadFromFile("../Assets/Spritesheets/Enemy_Punk/hurt.png");

    std::cout << "Textures chargees - idle: " << idle << ", walk: " << walk
        << ", punch: " << punch << ", hurt: " << hurt << std::endl;

    if (!idle || !walk || !punch || !hurt)
    {
        std::cerr << "ERREUR: Impossible de charger une ou plusieurs textures!" << std::endl;
        return;
    }

    // État initial
    context.currentSpriteState = SpriteState::IDLE;
    currentFrame = 0;
    frameCount = 4;

    //Calculer les dimensions des frames DYNAMIQUEMENT (comme le Player)
    sf::Vector2u textureSize = textures[context.currentSpriteState].getSize();
    frameWidth = textureSize.x / frameCount;
    frameHeight = textureSize.y;

    std::cout << "Taille texture complete: " << textureSize.x << "x" << textureSize.y << std::endl;
    std::cout << "Frame dimensions: " << frameWidth << "x" << frameHeight << std::endl;

    // Créer le sprite avec la texture (COMME LE PLAYER)
    sprite = new sf::Sprite(textures[context.currentSpriteState]);
    sprite->setTexture(textures[context.currentSpriteState]);
    sprite->setTextureRect(sf::IntRect({ 0, 0 }, { static_cast<int>(frameWidth), static_cast<int>(frameHeight) }));
    sprite->setPosition({ 200.f, 200.f });
    sprite->setOrigin({ frameWidth / 2.0f, frameHeight / 2.0f });
    sprite->setScale({ 1.f, 1.f });

    std::cout << "Sprite position: " << sprite->getPosition().x << ", " << sprite->getPosition().y << std::endl;
    std::cout << "Sprite texture rect: " << sprite->getTextureRect().size.x << "x"
        << sprite->getTextureRect().size.y << std::endl;

    // Mettre à jour le contexte
    context.npc = this;
    context.position = { 100.0f, 100.0f };
    context.velocity = { 0.0f, 0.0f };
    context.currentWaypointIndex = 0;
    context.waitTimer = 0.0f;
    context.isWaiting = false;
    context.sprite = sprite;

    // Définir les waypoints
    std::vector<Vector2> patrolPoints = {
        {context.position.x, context.position.y + 100.0f},
        {context.position.x + 100.0f,context.position.y + 100.0f},
    };

    // Créer les états
    PatrolState* patrolState = fsm.CreateState<PatrolState>();
    ChaseState* chaseState = fsm.CreateState<ChaseState>();

    // Configurer le PatrolState
    patrolState->SetWaypoints(patrolPoints, 100.0f, 2.0f);

    // Ajouter les transitions
    patrolState->AddTransition(Conditions::IsSeeingPlayer, chaseState);

    chaseState->AddTransition(Conditions::HasLostPlayer, patrolState);


    fsm.Init(patrolState, context);

    std::cout << "=== FIN Init() NPC ===" << std::endl;
}

void Npc::UpdateAnimation(float deltaTime)
{
    animationTimer += deltaTime;
    if (animationTimer >= animationSpeed)
    {
        animationTimer = 0.f;
        currentFrame++;
        if (currentFrame >= frameCount)
        {
            currentFrame = 0;
        }

        sprite->setTextureRect(sf::IntRect(
            { currentFrame * static_cast<int>(frameWidth), 0 },
            { static_cast<int>(frameWidth), static_cast<int>(frameHeight) }
        ));
    }
}

void Npc::Update(float deltaTime)
{
    context.deltaTime = deltaTime;

    fsm.Update(context);

    if (sprite != nullptr)
    {
        sprite->setPosition({ context.position.x, context.position.y });

        // Mettre à jour la direction
        if (context.velocity.x > 0.01f)
        {
            facingRight = false;
            context.facingRight = !facingRight;  //  Synchroniser avec le context
        }
        else if (context.velocity.x < -0.01f)
        {
            facingRight = true;
            context.facingRight = !facingRight;  // Synchroniser avec le context
        }

        sprite->setScale({ facingRight ? 1.f : -1.f, 1.f });
    }

    UpdateAnimation(deltaTime);
}


void Npc::Draw(sf::RenderWindow& window)
{
    DrawVisionDebug(window);

    window.draw(*sprite);  
}


void Npc::DrawVisionDebug(sf::RenderWindow& window)
{
    if (!showVisionDebug)
    {
        return;
    }

    // Calculer la direction du NPC
    float npcDirection = context.facingRight ? 0.0f : 180.0f;
    float halfAngle = context.visionAngle / 2.0f;

    // Créer un cône avec arc arrondi
    const int arcPoints = 30;
    visionCone.setPointCount(arcPoints + 2);

    // Point central (position du NPC)
    visionCone.setPoint(0, sf::Vector2f(0, 0));

    // Points de l'arc
    for (int i = 0; i <= arcPoints; i++)
    {
        float angle = (npcDirection - halfAngle + (context.visionAngle * i / arcPoints)) * 3.14159f / 180.0f;
        visionCone.setPoint(i + 1, sf::Vector2f(
            std::cos(angle) * context.visionRange,
            std::sin(angle) * context.visionRange
        ));
    }

    visionCone.setPosition({ context.position.x, context.position.y });

    // Changer la couleur selon l'état
    if (Conditions::IsSeeingPlayer(context))
    {
        // Rouge si le joueur est détecté
        visionCone.setFillColor(sf::Color(255, 0, 0, 80));
        visionCone.setOutlineColor(sf::Color(255, 0, 0, 200));
    }
    else
    {
        // Vert normal
        visionCone.setFillColor(sf::Color(0, 255, 0, 50));
        visionCone.setOutlineColor(sf::Color(0, 255, 0, 150));
    }
    visionCone.setOutlineThickness(2.0f);

    window.draw(visionCone);

    // Ligne vers le joueur si visible
    if (context.player != nullptr && Conditions::IsSeeingPlayer(context))
    {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(context.position.x, context.position.y), sf::Color::Red),
            sf::Vertex(sf::Vector2f(context.player->Getposition().x, context.player->Getposition().y), sf::Color::Red)
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);

        // Point sur le joueur détecté
        sf::CircleShape playerDot(8.0f);
        playerDot.setOrigin({ 8.0f, 8.0f });
        playerDot.setPosition({ context.player->Getposition().x, context.player->Getposition().y });
        playerDot.setFillColor(sf::Color::Red);
        playerDot.setOutlineColor(sf::Color::White);
        playerDot.setOutlineThickness(2.0f);
        window.draw(playerDot);
    }

    // Afficher la dernière position connue
    if (context.lostPlayerTimer > 0.0f && context.lostPlayerTimer < 2.0f)
    {
        sf::CircleShape lastPosMarker(10.0f);
        lastPosMarker.setOrigin({ 10.0f, 10.0f });
        lastPosMarker.setPosition({ context.lastKnownPlayerPosition.x, context.lastKnownPlayerPosition.y });
        lastPosMarker.setFillColor(sf::Color(255, 165, 0, 100));  // Orange
        lastPosMarker.setOutlineColor(sf::Color(255, 165, 0, 255));
        lastPosMarker.setOutlineThickness(2.0f);
        window.draw(lastPosMarker);

        // Ligne pointillée vers la dernière position
        sf::Vertex dashedLine[] = {
            sf::Vertex(sf::Vector2f(context.position.x, context.position.y), sf::Color(255, 165, 0, 150)),
            sf::Vertex(sf::Vector2f(context.lastKnownPlayerPosition.x, context.lastKnownPlayerPosition.y), sf::Color(255, 165, 0, 150))
        };
        window.draw(dashedLine, 2, sf::PrimitiveType::Lines);
    }

    // Afficher la direction avec une flèche
    float arrowLength = 40.0f;
    float arrowAngle = npcDirection * 3.14159f / 180.0f;
    sf::Vertex arrow[] = {
        sf::Vertex(sf::Vector2f(context.position.x, context.position.y), sf::Color::Yellow),
        sf::Vertex(sf::Vector2f(
            context.position.x + std::cos(arrowAngle) * arrowLength,
            context.position.y + std::sin(arrowAngle) * arrowLength
        ), sf::Color::Yellow)
    };
    window.draw(arrow, 2, sf::PrimitiveType::Lines);
}

void Npc::ToggleVisionDebug()
{
    showVisionDebug = !showVisionDebug;
    std::cout << "Vision debug: " << (showVisionDebug ? "ON" : "OFF") << std::endl;
}


void Npc::SetSpriteState(SpriteState state)
{
    if (context.currentSpriteState != state)
    {
        context.currentSpriteState = state;

        //  Recalculer les dimensions pour la nouvelle texture
        sf::Vector2u textureSize = textures[state].getSize();
        frameWidth = textureSize.x / frameCount;
        frameHeight = textureSize.y;

        sprite->setTexture(textures[state]);

        // Réinitialiser l'animation
        currentFrame = 0;
        animationTimer = 0.f;
        sprite->setTextureRect(sf::IntRect(
            { 0, 0 },
            { static_cast<int>(frameWidth), static_cast<int>(frameHeight) }
        ));
    }
}


void Npc::SetPlayer(Player* player)
{
    context.player = player;
}