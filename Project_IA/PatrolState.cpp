#include "PatrolState.h"
#include "NPC.h"
#include <cmath>

NpcAi::PatrolState::PatrolState()
    : speed(100.0f),
    waitTimeAtWaypoints(1.0f)
{
}

void NpcAi::PatrolState::SetWaypoints(const std::vector<Vector2>& _waypoints, float _speed, float _waitTime)
{
    waypoints = _waypoints;
    speed = _speed;
    waitTimeAtWaypoints = _waitTime;
}

void NpcAi::PatrolState::Enter(NpcContext& _context)
{
    std::cout << "Enter Patrol State" << std::endl;
    _context.currentWaypointIndex = 0;
    _context.waitTimer = 0.0f;
    _context.isWaiting = false;

    if (_context.npc != nullptr)
    {
        _context.npc->SetSpriteState(SpriteState::WALK);
    }
}

void NpcAi::PatrolState::Execute(NpcContext& _context)
{
    if (waypoints.empty())
    {
        std::cout << "WARNING: No waypoints!" << std::endl;
        return;
    }

    if (_context.isWaiting)
    {
        _context.waitTimer -= 0.016f;

        if (_context.waitTimer <= 0.0f)
        {
            _context.isWaiting = false;
            _context.currentWaypointIndex = (_context.currentWaypointIndex + 1) % waypoints.size();

            if (_context.npc != nullptr)
            {
                _context.npc->SetSpriteState(SpriteState::WALK);
            }
        }
        else
        {
            _context.velocity = { 0.0f, 0.0f };
            if (_context.npc != nullptr)
            {
                _context.npc->SetSpriteState(SpriteState::IDLE);
            }
        }
    }
    else
    {
        MoveTowardsWaypoint(_context);
        UpdateSpriteDirection(_context);

        Vector2 currentWaypoint = waypoints[_context.currentWaypointIndex];

        if (HasReachedWaypoint(_context.position, currentWaypoint))
        {
            _context.isWaiting = true;
            _context.waitTimer = waitTimeAtWaypoints;
            _context.position = currentWaypoint;
        }
    }
}

void NpcAi::PatrolState::Exit(NpcContext& _context)
{
    std::cout << "Exit Patrol State" << std::endl;
    _context.velocity = { 0.0f, 0.0f };

    if (_context.npc != nullptr)
    {
        _context.npc->SetSpriteState(SpriteState::IDLE);
    }
}

void NpcAi::PatrolState::MoveTowardsWaypoint(NpcContext& _context)
{
    Vector2 target = waypoints[_context.currentWaypointIndex];

    float dx = target.x - _context.position.x;
    float dy = target.y - _context.position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0.0f)
    {
        _context.velocity.x = (dx / distance) * speed;
        _context.velocity.y = (dy / distance) * speed;

        _context.position.x += _context.velocity.x * 0.016f;
        _context.position.y += _context.velocity.y * 0.016f;
    }
}

void NpcAi::PatrolState::UpdateSpriteDirection(NpcContext& _context)
{
    if (_context.sprite == nullptr)
        return;

    sf::Sprite* sfSprite = static_cast<sf::Sprite*>(_context.sprite);

    if (_context.velocity.x < -0.1f)
    {
        sfSprite->setScale({ -1.0f, 1.0f });
    }
    else if (_context.velocity.x > 0.1f)
    {
        sfSprite->setScale({ 1.0f, 1.0f });
    }
}

bool NpcAi::PatrolState::HasReachedWaypoint(const Vector2& position, const Vector2& waypoint)
{
    float threshold = 5.0f;
    float dx = waypoint.x - position.x;
    float dy = waypoint.y - position.y;
    float distanceSquared = dx * dx + dy * dy;

    return distanceSquared < (threshold * threshold);
}