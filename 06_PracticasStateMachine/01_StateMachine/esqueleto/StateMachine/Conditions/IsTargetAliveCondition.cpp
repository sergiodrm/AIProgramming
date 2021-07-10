#include "stdafx.h"
#include "IsTargetAliveCondition.h"

#include "character.h"

bool CIsAliveCondition::Check() const
{
    if (m_target)
    {
        return m_target->IsAlive();
    }
    return false;
}
