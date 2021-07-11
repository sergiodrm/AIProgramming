#include "stdafx.h"
#include "Selector.h"

#include "Conditions/Condition.h"

CSelector::CSelector(CBehaviorTree* _owner)
    : CGroup(_owner), m_currentChild(0) {}

void CSelector::DrawDebug() const
{
    if (m_condition)
    {
        m_condition->DrawDebug();
    }
    if (m_currentChild < static_cast<int>(m_behaviors.size()))
    {
        m_behaviors.at(m_currentChild)->DrawDebug();
    }
}

void CSelector::OnEnter()
{
    m_currentChild = 0;
}

EStatus CSelector::OnUpdate(float _deltaTime)
{
    if (m_behaviors.size() > 0)
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
    }
    return EStatus::Invalid;
}

void CSelector::OnExit()
{}
