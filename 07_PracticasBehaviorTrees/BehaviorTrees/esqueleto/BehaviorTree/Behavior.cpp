#include "stdafx.h"
#include "Behavior.h"

CBehavior::CBehavior()
    : m_status(EStatus::Invalid) {}

EStatus CBehavior::Tick(float _deltaTime)
{
    if (m_status == EStatus::Invalid)
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

EStatus CBehavior::OnUpdate(float _deltaTime) { return EStatus::Success; }
void CBehavior::OnEnter() {}
void CBehavior::OnExit() {}
