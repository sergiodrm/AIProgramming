#include "stdafx.h"
#include "AttackAction.h"

#include "character.h"
#include "StateMachine/StateMachine.h"

CAttackAction::CAttackAction(CStateMachine* _owner)
    : CAction(_owner), m_damageAmount(30.f), m_timeBetweenAttacks(2.f), m_timeUntilNextAttack(0.f)
{}

void CAttackAction::OnStart()
{
    m_ownerStateMachine->GetOwner()->GetTarget()->SetUnderAttack(true);
}

void CAttackAction::OnEnd()
{
    m_ownerStateMachine->GetOwner()->GetTarget()->SetUnderAttack(false);
}

void CAttackAction::OnUpdate(float _deltaTime)
{
    CAction::OnUpdate(_deltaTime);

    if (m_ownerStateMachine)
    {
        if (m_timeUntilNextAttack <= 0.f)
        {
            Character* enemy = m_ownerStateMachine->GetOwner()->GetTarget();
            if (enemy)
            {
                enemy->TakeDamage(m_damageAmount);
                m_timeUntilNextAttack += m_timeBetweenAttacks;
            }
        }
        else
        {
            m_timeUntilNextAttack -= _deltaTime;
        }
    }
}
