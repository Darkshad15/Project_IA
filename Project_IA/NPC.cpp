#include "NPC.h"

Npc::Npc() :
    context(),
    sprite(sprite),
    textures(),
    frameWidth(96),      // Largeur d'une frame
    frameHeight(63),     // Hauteur d'une frame
    currentFrame(0),
    frameCount(4),       // Nombre de frames par animation
    animationTimer(0.f),
    animationSpeed(0.15f)
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

void Npc::Init()
{
    // Charger les textures
    if (!LoadTextures())
    {
        std::cerr << "Erreur lors du chargement des textures du NPC" << std::endl;
        return;
    }

    // Initialiser le sprite avec la texture IDLE
    sprite.setTexture(textures[SpriteState::IDLE]);

    // IMPORTANT: Définir le rectangle de texture pour afficher UNE SEULE frame
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameWidth, frameHeight }));

    // Position et origine
    sprite.setPosition({ 400.0f, 400.0f });
    sprite.setOrigin({ frameWidth / 2.0f , frameHeight / 2.0f });

    std::cout << "Sprite position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
    std::cout << "Texture rect: " << sprite.getTextureRect().size.x << "x" << sprite.getTextureRect().size.y << std::endl;

    // Mettre à jour le contexte
    context.npc = this;
    context.position = { 100.0f, 100.0f };
    context.velocity = { 0.0f, 0.0f };
    context.currentWaypointIndex = 0;
    context.waitTimer = 0.0f;
    context.isWaiting = false;
    context.sprite = &sprite;
    context.currentSpriteState = SpriteState::IDLE;

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

        // Mettre à jour le rectangle de texture
        sprite.setTextureRect(sf::IntRect(
            { currentFrame * frameWidth,  // X position dans la spritesheet
            0 },                          // Y position (0 si une seule ligne)
            { frameWidth,                 // Largeur
            frameHeight }                 // Hauteur
        ));
    }
}

void Npc::Update(float deltaTime)
{
    fsm.Update(context);

    // Synchroniser la position du sprite avec le contexte
    sprite.setPosition({ context.position.x, context.position.y });

    // Animer le sprite
    UpdateAnimation(deltaTime);
}

void Npc::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Npc::SetSpriteState(SpriteState state)
{
    if (context.currentSpriteState != state)
    {
        context.currentSpriteState = state;
        sprite.setTexture(textures[state]);

        // Réinitialiser l'animation
        currentFrame = 0;
        animationTimer = 0.f;
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameWidth, frameHeight }));
    }
}