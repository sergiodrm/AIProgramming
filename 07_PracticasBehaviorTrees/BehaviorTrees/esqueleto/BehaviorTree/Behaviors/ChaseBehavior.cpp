#include "stdafx.h"
#include "ChaseBehavior.h"

#include "character.h"
#include "BehaviorTree/BehaviorTree.h"

CChaseBehavior::CChaseBehavior(CBehaviorTree* _owner)
    : CBehavior(_owner), m_dist(100.f), m_target(nullptr)
{
    if (_owner)
    {
        m_target = _owner->GetOwner()->GetTarget();
    }
}

EStatus CChaseBehavior::OnUpdate(float _deltaTime)
{
    m_owner->GetOwner()->MoveTo(m_target->GetLoc());
    if (CheckDistance())
    {
        return EStatus::Success;
    }
    return EStatus::Running;
}

void CChaseBehavior::OnExit()
{
    m_owner->GetOwner()->CancelMovement();
}

bool CChaseBehavior::CheckDistance() const
{
    const USVec2D delta = m_target->GetLoc() - m_owner->GetOwner()->GetLoc();
    return delta.LengthSquared() <= m_dist * m_dist;
}
