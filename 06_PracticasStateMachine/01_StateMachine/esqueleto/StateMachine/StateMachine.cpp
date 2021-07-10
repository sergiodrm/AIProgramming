#include "stdafx.h"
#include "StateMachine.h"

#include "State.h"
#include "Transition.h"

CStateMachine::CStateMachine()
    : m_currentState(nullptr)
{}

void CStateMachine::Load() {}

void CStateMachine::Start()
{
    if (!m_states.empty())
    {
        m_currentState = m_states.at(0);
        m_currentState->OnEnter();
    }
}

void CStateMachine::Update()
{
    // Update current state
    m_currentState->OnUpdate();

    // Check transitions
    for (CTransition* it : m_currentState->GetTransition())
    {
        if (it->CanTrigger())
        {
            m_currentState->OnExit();
            m_currentState = it->Trigger();
            m_currentState->OnEnter();
            return;
        }
    }
}
