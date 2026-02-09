#pragma once

namespace FSM
{
    template <typename ContextType>
    template <typename StateType>
    StateType* StateMachine<ContextType>::CreateState()
    {
        StateType* state = new StateType();
        states.push_back(state);
        return state;
    }

    template <typename ContextType>
    void StateMachine<ContextType>::Init(State* _initState, ContextType _context)
    {
        currentState = _initState;
        if (currentState != nullptr)
        {
            currentState->Enter(_context);
        }
    }

    template <typename ContextType>
    void StateMachine<ContextType>::Update(ContextType _context)
    {
        if (currentState == nullptr)
            return;

        currentState->Execute(_context);

        State* nextState = currentState->TryGetNext(_context);
        if (nextState != nullptr && nextState != currentState)
        {
            currentState->Exit(_context);
            currentState = nextState;
            currentState->Enter(_context);
        }
    }
}