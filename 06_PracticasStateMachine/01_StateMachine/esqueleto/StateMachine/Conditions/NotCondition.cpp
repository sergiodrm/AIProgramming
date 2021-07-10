#include "stdafx.h"
#include "NotCondition.h"

CNotCondition::~CNotCondition()
{
    delete m_condition;
}

bool CNotCondition::Check() const
{
    if (m_condition)
    {
        return !m_condition->Check();
    }
    return false;
}

void CNotCondition::DrawDebug() const
{
    if (m_condition)
    {
        m_condition->DrawDebug();
    }
}

void CNotCondition::SetCondition(ICondition* _condition)
{
    if (_condition)
    {
        m_condition = _condition;
    }
}
