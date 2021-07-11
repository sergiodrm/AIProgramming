#include "stdafx.h"
#include "Sequence.h"

#include "Conditions/Condition.h"

CSequence::CSequence()
    : CGroup(), m_currentChild(0) {}

void CSequence::OnEnter()
{
    m_currentChild = 0;
}

EStatus CSequence::OnUpdate(float _deltaTime)
{
    while (true)
    {
        if (m_condition)
        {
            if (!m_condition->Check())
            {
                return EStatus::Fail;
            }
        }
        const EStatus childStatus = m_behaviors.at(m_currentChild)->Tick(_deltaTime);

        if (childStatus != EStatus::Success)
        {
            return childStatus;
        }

        ++m_currentChild;

        if (m_currentChild == static_cast<int>(m_behaviors.size()))
        {
            return EStatus::Success;
        }
    }
    return EStatus::Invalid;
}
