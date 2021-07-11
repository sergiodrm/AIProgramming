#pragma once

#include "Condition.h"

class COrCondition : public ICondition
{
public:
public:
    ~COrCondition();
    virtual bool Check() const;
    virtual void DrawDebug() const;
    void AddCondition(ICondition* _condition);
private:
    std::vector<ICondition*> m_conditions;
};
