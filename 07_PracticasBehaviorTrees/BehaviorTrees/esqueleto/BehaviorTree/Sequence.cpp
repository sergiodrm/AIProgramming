#include "stdafx.h"
#include "Sequence.h"

#include "Conditions/Condition.h"

CSequence::CSequence(CBehaviorTree* _owner)
    : CGroup(_owner), m_currentChild(0) {}

void CSequence::OnEnter()
{
    m_currentChild = 0;
}

EStatus CSequence::OnUpdate(float _deltaTime)
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
    }
    return EStatus::Invalid;
}

void CSequence::OnExit()
{
    m_currentChild = 0;
}
