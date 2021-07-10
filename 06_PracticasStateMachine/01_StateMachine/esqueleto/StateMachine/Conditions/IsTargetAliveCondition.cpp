#include "stdafx.h"
#include "IsTargetAliveCondition.h"

#include "character.h"

bool CIsTargetAliveCondition::Check() const
{
    if (m_target)
    {
        return m_target->IsAlive();
    }
    return false;
}
