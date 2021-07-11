#include "stdafx.h"
#include "WaitBehavior.h"

CWaitBehavior::CWaitBehavior(CBehaviorTree* _owner)
    : CBehavior(_owner), m_time(5.f), m_timeUntilSuccess(0.f)
{}

void CWaitBehavior::OnEnter()
{
    m_timeUntilSuccess = m_time;
}

EStatus CWaitBehavior::OnUpdate(float _deltaTime)
{
    m_timeUntilSuccess -= _deltaTime;
    if (m_timeUntilSuccess <= 0.f)
    {
        return EStatus::Success;
    }
    return EStatus::Running;
}
