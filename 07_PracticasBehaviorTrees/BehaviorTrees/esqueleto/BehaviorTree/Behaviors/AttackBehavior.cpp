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

void CAttackBehavior::DrawDebug() const
{
    MOAIDraw::DrawEllipseFill(100.f, 100.f, 10.f, 10.f, 20);
}

void CAttackBehavior::SetDamage(float _damage)
{
    m_damage = _damage;
}

void CAttackBehavior::OnEnter()
{
    if (m_target)
    {
        m_target->SetUnderAttack(true);
    }
}

EStatus CAttackBehavior::OnUpdate(float _deltaTime)
{
    m_target->TakeDamage(m_damage);
    return EStatus::Success;
}

void CAttackBehavior::OnExit()
{
    if (m_target)
    {
        m_target->SetUnderAttack(false);
    }
}
