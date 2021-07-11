#pragma once

#include "Condition.h"

class CAndCondition : public ICondition
{
public:
    ~CAndCondition();
    virtual bool Check() const;
    virtual void DrawDebug() const;
    void AddCondition(ICondition* _condition);
private:
    std::vector<ICondition*> m_conditions;
};
