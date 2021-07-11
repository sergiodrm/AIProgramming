#pragma once

#include "Behavior.h"

class ICondition;

class CGroup : public CBehavior
{
public:
    CGroup()
        : CBehavior(), m_condition(nullptr) {}

    void SetCondition(ICondition* _condition) { m_condition = _condition; }
    void AddBehavior(CBehavior* _behavior) { m_behaviors.push_back(_behavior); }
protected:
    std::vector<CBehavior*> m_behaviors;
    ICondition* m_condition;
};
