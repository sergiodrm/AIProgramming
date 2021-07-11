#include "stdafx.h"
#include "Selector.h"

#include "Conditions/Condition.h"

CSelector::CSelector()
    : CGroup(), m_currentChild(0) {}

void CSelector::OnEnter()
{
    m_currentChild = 0;
}

EStatus CSelector::OnUpdate(float _deltaTime)
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

        if (childStatus != EStatus::Fail)
        {
            return childStatus;
        }

        ++m_currentChild;

        if (m_currentChild == static_cast<int>(m_behaviors.size()))
        {
            return EStatus::Fail;
        }
    }
    return EStatus::Invalid;
}
