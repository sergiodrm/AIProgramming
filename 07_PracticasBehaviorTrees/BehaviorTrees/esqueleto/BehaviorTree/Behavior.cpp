#include "stdafx.h"
#include "Behavior.h"

CBehavior::CBehavior(CBehaviorTree* _owner)
    : m_status(EStatus::Invalid), m_owner(_owner) {}

EStatus CBehavior::Tick(float _deltaTime)
{
    if (m_status != EStatus::Running)
    {
        OnEnter();
    }

    m_status = OnUpdate(_deltaTime);

    if (m_status != EStatus::Running)
    {
        OnExit();
    }
    return m_status;
}

void CBehavior::DrawDebug() const
{}

EStatus CBehavior::OnUpdate(float _deltaTime) { return EStatus::Success; }
void CBehavior::OnEnter() {}
void CBehavior::OnExit() {}
