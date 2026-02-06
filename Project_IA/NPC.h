#pragma once
#include "StateMachine.h"

#include "Conditions.h"
#include "ChaseState.h"
#include "PatrolState.h"

using namespace NpcAi;


class Npc
{

	FSM::StateMachine<NpcContext> fsm;

	NpcContext context{};

	void Init()
	{
		PatrolState* patrolState = fsm.CreateState<PatrolState>();
		ChaseState* chaseState = fsm.CreateState<ChaseState>();

		patrolState->AddTransition(Conditions::IsSeeingPlayer, chaseState);
		chaseState->AddTransition([](const NpcContext _context)
			{
				return !Conditions::IsSeeingPlayer(_context);
			}, patrolState);

		fsm.Init(patrolState, context);
	}

	void Update()
	{
		fsm.Update(context);
	}
};


