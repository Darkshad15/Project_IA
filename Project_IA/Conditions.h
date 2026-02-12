#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "NPCContext.h"
#include "Player.h"

namespace NpcAi
{
    class Conditions
    {
    public:
        static bool IsSeeingPlayer(NpcContext context)
        {
            if (context.player == nullptr)
            {
                return false;
            }

            //  Paramètres de vision
            const float visionRange = context.visionRange;
            const float visionAngle = context.visionAngle;  

            // 1. Calculer la distance
            float dx = context.player->Getposition().x - context.position.x;
            float dy = context.player->Getposition().y - context.position.y;
            float distanceSquared = dx * dx + dy * dy;
            float visionRangeSquared = visionRange * visionRange;

            // Trop loin ?
            if (distanceSquared > visionRangeSquared)
            {
                return false;
            }

            
            float angleToPlayer = std::atan2(dy, dx) * 180.0f / 3.14159f;

           
            float npcDirection = context.facingRight ? 0.0f : 180.0f;

            float angleDiff = angleToPlayer - npcDirection;

  
            while (angleDiff > 180.0f) angleDiff -= 360.0f;
            while (angleDiff < -180.0f) angleDiff += 360.0f;


            return std::abs(angleDiff) <= (visionAngle / 2.0f);

           /* bool inCone = std::abs(angleDiff) <= (visionAngle / 2.0f);

            if (inCone)
            {
                std::cout << "PLAYER DETECTED! Distance: " << std::sqrt(distanceSquared)
                    << " | Angle diff: " << angleDiff << std::endl;
            }*/
        }

        static bool HasLostPlayer(NpcContext context)
        {
            const float lostPlayerThreshold = 2.0f;
            return context.lostPlayerTimer >= lostPlayerThreshold;
        }
    };
}

