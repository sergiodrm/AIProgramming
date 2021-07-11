#pragma once

#include "BehaviorTree/Behavior.h"

class Character;

class CAttackBehavior : public CBehavior
{
public:
    CAttackBehavior(CBehaviorTree* _owner);
    void SetDamage(float _damage);
protected:
    virtual EStatus OnUpdate(float _deltaTime) override;

    float m_damage;
    Character* m_target;
};
