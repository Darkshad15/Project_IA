#include "NPC.h"

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
    facingRight(false)
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
        {100.0f, 100.0f},
        {400.0f, 100.0f},
        {400.0f, 400.0f},
        {100.0f, 400.0f}
    };

    // Créer les états
    PatrolState* patrolState = fsm.CreateState<PatrolState>();
    ChaseState* chaseState = fsm.CreateState<ChaseState>();

    // Configurer le PatrolState
    patrolState->SetWaypoints(patrolPoints, 100.0f, 2.0f);

    // Ajouter les transitions
    patrolState->AddTransition(Conditions::IsSeeingPlayer, chaseState);
    chaseState->AddTransition([](const NpcContext _context)
        {
            return !Conditions::IsSeeingPlayer(_context);
        }, patrolState);

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
    fsm.Update(context);

    if (sprite != nullptr)
    {
        // Synchroniser la position
        sprite->setPosition({ context.position.x, context.position.y });

        // Mettre à jour la direction si le NPC se déplace
        if (context.velocity.x > 0.01f)
        {
            facingRight = false;
        }
        else if (context.velocity.x < -0.01f)
        {
            facingRight = true;
        }

        // Appliquer le flip
        sprite->setScale({ facingRight ? 1.f : -1.f, 1.f });
    }

    UpdateAnimation(deltaTime);
}


void Npc::Draw(sf::RenderWindow& window)
{
    window.draw(*sprite);  
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