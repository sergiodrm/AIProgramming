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
    void OnUpdate();
    void OnExit();
    const StateTransitions& GetTransition() const;

private:
    CAction* m_enterAction;
    CAction* m_exitAction;
    CAction* m_stateAction;
    StateTransitions m_transitions;
};
