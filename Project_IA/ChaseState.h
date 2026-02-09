#pragma once
#include <iostream>
#include <ostream>

#include "NPCContext.h"
#include "State.h"

namespace NpcAi{
	
	class ChaseState : public FSM::State<NpcContext>
	{
		void Enter(NpcContext _context) override
		{
			std::cout << "Enter Patrol State" << std::endl;
		}

		void Exit(NpcContext _context) override
		{
			std::cout << "Exit Patrol State" << std::endl;
		}
	};
}
