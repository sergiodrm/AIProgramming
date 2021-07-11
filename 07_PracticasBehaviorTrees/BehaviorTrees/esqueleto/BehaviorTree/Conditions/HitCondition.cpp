#include "stdafx.h"
#include "HitCondition.h"

#include "character.h"

bool CHitCondition::Check() const
{
    return m_character->IsUnderAttack();
}
