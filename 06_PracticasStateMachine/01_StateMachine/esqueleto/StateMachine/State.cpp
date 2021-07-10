#include "stdafx.h"
#include "State.h"

#include "Actions/Action.h"

CState::CState()
    : m_enterAction(nullptr), m_exitAction(nullptr), m_stateAction(nullptr)
{}

void CState::OnEnter()
{
    if (m_enterAction)
    {
        m_enterAction->OnStart();
    }
    if (m_stateAction)
    {
        m_stateAction->OnStart();
    }
}

void CState::OnUpdate()
{
    if (m_stateAction)
    {
        m_stateAction->OnUpdate();
    }
}

void CState::OnExit()
{
    if (m_stateAction)
    {
        m_stateAction->OnEnd();
    }
    if (m_exitAction)
    {
        m_exitAction->OnStart();
    }
}

const StateTransitions& CState::GetTransition() const { return m_transitions; }
