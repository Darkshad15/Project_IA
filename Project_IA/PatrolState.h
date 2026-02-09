#pragma once

#include <iostream>
#include <vector>
#include "NPCContext.h"
#include "State.h"

namespace NpcAi
{
    class PatrolState : public FSM::State<NpcContext&>
    {
    public:
        PatrolState();

        void SetWaypoints(const std::vector<Vector2>& _waypoints, float _speed, float _waitTime = 1.0f);

        void Enter(NpcContext& _context) override;
        void Execute(NpcContext& _context) override;
        void Exit(NpcContext& _context) override;

    private:
        std::vector<Vector2> waypoints;
        float speed;
        float waitTimeAtWaypoints;

        void MoveTowardsWaypoint(NpcContext& _context);
        void UpdateSpriteDirection(NpcContext& _context);
        bool HasReachedWaypoint(const Vector2& position, const Vector2& waypoint);
    };
}