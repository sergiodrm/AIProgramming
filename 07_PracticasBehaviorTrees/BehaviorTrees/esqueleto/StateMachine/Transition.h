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

    void DrawDebug() const;

    void SetCondition(ICondition* _condition);
    void SetTargetState(CState* _state);
    void SetTriggerAction(CAction* _action);
private:
    ICondition* m_condition;
    CState* m_targetState;
    CAction* m_triggerAction;
};
