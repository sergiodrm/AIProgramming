#pragma once

class CStateMachine;

class CAction
{
public:
    CAction(CStateMachine* _ownerStateMachine)
        : m_ownerStateMachine(_ownerStateMachine) {}

    virtual void OnStart() {}
    virtual void OnUpdate(float _deltaTime) {}
    virtual void OnEnd() {}
    virtual void DrawDebug() {}
protected:
    CStateMachine* m_ownerStateMachine;
};
