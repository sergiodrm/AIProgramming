#include "stdafx.h"
#include "StateMachine.h"

#include "State.h"
#include "Transition.h"
#include "Actions/ChaseAction.h"

CStateMachine::CStateMachine(Character* _owner)
    : m_currentState(nullptr), m_owner(_owner)
{}

CStateMachine::~CStateMachine()
{
    for (CState* it : m_states)
    {
        delete it;
    }
}

void CStateMachine::Load()
{
    CState* chaseState = new CState();
    chaseState->AddStateAction(new CChaseAction(this));
    m_states.push_back(chaseState);
}

void CStateMachine::Start()
{
    if (!m_states.empty())
    {
        m_currentState = m_states.at(0);
        m_currentState->OnEnter();
    }
}

void CStateMachine::Update(float _deltaTime)
{
    if (m_currentState)
    {
        // Update current state
        m_currentState->OnUpdate(_deltaTime);

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
}

void CStateMachine::DrawDebug()
{
    m_currentState->DrawDebug();
}
