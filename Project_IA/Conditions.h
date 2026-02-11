#pragma once
#include <SFML/Graphics.hpp>
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

            Vector2 playerPos = context.player->Getposition();  // Utiliser le getter

            float dx = playerPos.x - context.position.x;
            float dy = playerPos.y - context.position.y;
            float distanceSquared = dx * dx + dy * dy;

            const float visionRangeSquared = 100.0f * 100.0f;


            return distanceSquared <= visionRangeSquared;
		}

        static bool HasLostPlayer(NpcContext context)
        {
            const float lostPlayerThreshold = 2.0f;  // 2 secondes sans voir le joueur

            return context.lostPlayerTimer >= lostPlayerThreshold;
        }

	};
}

