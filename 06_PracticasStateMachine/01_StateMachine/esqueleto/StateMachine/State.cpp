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

void CState::OnUpdate(float _deltaTime)
{
    if (m_stateAction)
    {
        m_stateAction->OnUpdate(_deltaTime);
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

void CState::DrawDebug()
{
    if (m_stateAction)
    {
        m_stateAction->DrawDebug();
    }
}

const StateTransitions& CState::GetTransition() const { return m_transitions; }

void CState::AddEnterAction(CAction* _action)
{
    OverrideAction(m_enterAction, _action);
}

void CState::AddStateAction(CAction* _action)
{
    OverrideAction(m_stateAction, _action);
}

void CState::AddExitAction(CAction* _action)
{
    OverrideAction(m_exitAction, _action);
}

void CState::OverrideAction(CAction*& currentAction_, CAction* _newAction)
{
    if (_newAction)
    {
        delete currentAction_;
        currentAction_ = _newAction;
    }
}
