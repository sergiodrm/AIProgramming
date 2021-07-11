#pragma once

#include "Condition.h"

class Character;

class CIsAliveCondition : public ICondition
{
public:
    CIsAliveCondition(Character* _target)
        : m_target(_target) {}

    virtual bool Check() const;
private:
    Character* m_target;
};
