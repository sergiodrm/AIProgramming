#include "stdafx.h"
#include "AttackBehavior.h"

#include "character.h"
#include "BehaviorTree/BehaviorTree.h"

CAttackBehavior::CAttackBehavior(CBehaviorTree* _owner)
    : CBehavior(_owner), m_damage(30.f), m_target(nullptr)
{
    if (m_owner)
    {
        m_target = m_owner->GetOwner()->GetTarget();
    }
}

void CAttackBehavior::SetDamage(float _damage)
{
    m_damage = _damage;
}

EStatus CAttackBehavior::OnUpdate(float _deltaTime)
{
    m_target->TakeDamage(m_damage);
    return EStatus::Success;
}
