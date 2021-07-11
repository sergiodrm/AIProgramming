#pragma once

#include "BehaviorTree/Behavior.h"

class Character;

class CChaseBehavior : public CBehavior
{
public:
    CChaseBehavior(CBehaviorTree* _owner);
    void SetDistance(float _dist) { m_dist = _dist; }
protected:
    virtual EStatus OnUpdate(float _deltaTime) override;
    virtual void OnExit() override;
    bool CheckDistance() const;
private:
    Character* m_target;
    float m_dist;
};
