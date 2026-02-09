#include "ChaseState.h"


#include "NPC.h"

void NpcAi::ChaseState::Enter(NpcContext& _context)
{
    std::cout << "Enter Chase State" << std::endl;

    if (_context.npc != nullptr)
    {
        _context.npc->SetSpriteState(SpriteState::WALK);
    }
}

void NpcAi::ChaseState::Execute(NpcContext& _context)
{
    // TODO: Logique de poursuite
}

void NpcAi::ChaseState::Exit(NpcContext& _context)
{
    std::cout << "Exit Chase State" << std::endl;
}