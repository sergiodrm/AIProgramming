#pragma once

#include "BehaviorTree/Behavior.h"

class Character;

class CAttackBehavior : public CBehavior
{
public:
    CAttackBehavior(CBehaviorTree* _owner);
    virtual void DrawDebug() const override;
    void SetDamage(float _damage);
protected:
    virtual void OnEnter() override;
    virtual EStatus OnUpdate(float _deltaTime) override;
    virtual void OnExit() override;

    float m_damage;
    Character* m_target;
};
