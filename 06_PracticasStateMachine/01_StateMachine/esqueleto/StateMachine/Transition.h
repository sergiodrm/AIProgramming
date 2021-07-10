#pragma once


class CAction;
class ICondition;
class CState;

class CTransition
{
public:
    CTransition();

    bool CanTrigger() const;
    CState* Trigger();
private:
    ICondition* m_condition;
    CState* m_targetState;
    CAction* m_triggerAction;
};
