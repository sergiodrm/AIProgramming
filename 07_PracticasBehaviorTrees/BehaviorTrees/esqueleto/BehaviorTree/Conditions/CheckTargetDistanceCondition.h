#pragma once

#include "Condition.h"

class Character;

class CCheckTargetDistanceCondition : public ICondition
{
public:
    CCheckTargetDistanceCondition(Character* _owner);
    virtual bool Check() const override;
    virtual void DrawDebug() const override;
    void SetTriggerDistance(float _dist);
private:
    Character* m_owner;
    float m_triggerSquaredDistance;
};
