#include "stdafx.h"
#include "AndCondition.h"

CAndCondition::~CAndCondition()
{
    for (ICondition* it : m_conditions)
    {
        delete it;
    }
}

bool CAndCondition::Check() const
{
    for (ICondition* it : m_conditions)
    {
        if (!it->Check())
        {
            return false;
        }
    }
    return true;
}

void CAndCondition::DrawDebug() const
{
    for (ICondition* it : m_conditions)
    {
        it->DrawDebug();
    }
}

void CAndCondition::AddCondition(ICondition* _condition)
{
    if (_condition)
    {
        m_conditions.push_back(_condition);
    }
}
