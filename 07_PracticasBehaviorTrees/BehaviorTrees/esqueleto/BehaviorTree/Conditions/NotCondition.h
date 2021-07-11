#pragma once

#include "Condition.h"

class CNotCondition : public ICondition
{
public:
    CNotCondition(ICondition* _condition = nullptr)
        : m_condition(_condition) {}

    ~CNotCondition();
    virtual bool Check() const;
    virtual void DrawDebug() const;
    void SetCondition(ICondition* _condition);
private:
    ICondition* m_condition;
};
