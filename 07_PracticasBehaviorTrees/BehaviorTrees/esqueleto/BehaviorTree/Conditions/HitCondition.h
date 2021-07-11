#pragma once

#include "Condition.h"

class Character;

class CHitCondition : public ICondition
{
public:
    CHitCondition(Character* _character)
        : m_character(_character) {}

    virtual bool Check() const override;
private:
    Character* m_character;
};
