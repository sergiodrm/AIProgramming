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

bool CTransition::CanTrigger() const
{
    return m_condition->Check();
}
