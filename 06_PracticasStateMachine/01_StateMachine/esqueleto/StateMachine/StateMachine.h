#pragma once
#include <vector>

class CState;
typedef std::vector<CState*> StateVector;

class CStateMachine
{
public:
    CStateMachine();

    void Load();
    void Start();
    void Update();
private:
    StateVector m_states;
    CState* m_currentState;
};
