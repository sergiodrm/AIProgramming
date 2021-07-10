#include "stdafx.h"
#include "Transition.h"
#include "Actions/Action.h"
#include "Conditions/Condition.h"

CTransition::CTransition()
    : m_condition(nullptr), m_targetState(nullptr), m_triggerAction(nullptr) {}

CState* CTransition::Trigger()
{
    if (m_triggerAction)
    {
        m_triggerAction->OnStart();
    }
    return m_targetState;
}

void CTransition::DrawDebug() const
{
    if (m_condition)
    {
        m_condition->DrawDebug();
    }
}

void CTransition::SetCondition(ICondition* _condition) { m_condition = _condition; }
void CTransition::SetTargetState(CState* _state) { m_targetState = _state; }
void CTransition::SetTriggerAction(CAction* _action) { m_triggerAction = _action; }

bool CTransition::CanTrigger() const
{
    return m_condition->Check();
}
