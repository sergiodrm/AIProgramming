#pragma once

#include "Action.h"

class CAttackAction : public CAction
{
public:
    CAttackAction(CStateMachine* _owner);

    virtual void OnStart() override;
    virtual void OnUpdate(float _deltaTime) override;
    virtual void OnEnd() override;
    void SetDamage(float _damage) { m_damageAmount = _damage; }
    void SetTimeBetweenAttacks(float _time) { m_timeBetweenAttacks = _time; }
private:
    float m_damageAmount;
    float m_timeBetweenAttacks;
    float m_timeUntilNextAttack;
};
