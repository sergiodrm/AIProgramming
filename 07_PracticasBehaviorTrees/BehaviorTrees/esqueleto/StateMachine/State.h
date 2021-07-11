#pragma once
#include <vector>

class CAction;
class CTransition;

typedef std::vector<CTransition*> StateTransitions;

class CState
{
public:
    CState();

    void OnEnter();
    void OnUpdate(float _deltaTime);
    void OnExit();
    void DrawDebug();
    const StateTransitions& GetTransition() const;

    void AddEnterAction(CAction* _action);
    void AddStateAction(CAction* _action);
    void AddExitAction(CAction* _action);
    void AddTransition(CTransition* _transition);

protected:
    static void OverrideAction(CAction*& currentAction_, CAction* _newAction);

private:
    CAction* m_enterAction;
    CAction* m_exitAction;
    CAction* m_stateAction;
    StateTransitions m_transitions;
};
