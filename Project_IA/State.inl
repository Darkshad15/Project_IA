#pragma once

namespace FSM
{
    template<typename ContextType>
    void State<ContextType>::AddTransition(Condition _condition, _State* _newState)
    {
        transitions.push_back({ _condition, _newState });
    }

    template<typename ContextType>
    typename State<ContextType>::_State* State<ContextType>::TryGetNext(ContextType _context)
    {
        for (auto& [condition, nextState] : transitions)
        {
            if (condition(_context))
            {
                return nextState;
            }
        }
        return nullptr;
    }
}