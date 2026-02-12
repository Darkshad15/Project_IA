#pragma once
#include <iostream>
#include "NPCContext.h"
#include "State.h"

namespace NpcAi
{
    class ChaseState : public FSM::State<NpcContext&>
    {
    public:
        void Enter(NpcContext& _context) override;
        void Execute(NpcContext& _context) override;
        void Exit(NpcContext& _context) override;
    };
}