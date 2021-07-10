#include "stdafx.h"
#include "OrCondition.h"

COrCondition::~COrCondition()
{
    for (ICondition* it : m_conditions)
    {
        delete it;
    }
}

bool COrCondition::Check() const
{
    for (ICondition* it : m_conditions)
    {
        if (it->Check())
        {
            return true;
        }
    }
    return false;
}

void COrCondition::DrawDebug() const
{
    for (ICondition* it : m_conditions)
    {
        it->DrawDebug();
    }
}

void COrCondition::AddCondition(ICondition* _condition)
{
    if (_condition)
    {
        m_conditions.push_back(_condition);
    }
}
