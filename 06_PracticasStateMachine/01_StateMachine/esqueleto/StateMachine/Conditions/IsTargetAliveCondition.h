#pragma once

#include "Condition.h"

class Character;

class CIsTargetAliveCondition : public ICondition
{
public:
    CIsTargetAliveCondition(Character* _target)
        : m_target(_target) {}

    virtual bool Check() const;
private:
    Character* m_target;
};
