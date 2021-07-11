#pragma once

class CBehaviorTree;

enum class EStatus : int8_t
{
    Invalid,
    Success,
    Fail,
    Running
};

class CBehavior
{
public:
    CBehavior(CBehaviorTree* _owner);

    EStatus Tick(float _deltaTime);
    virtual void DrawDebug() const;

protected:
    virtual EStatus OnUpdate(float _deltaTime);
    virtual void OnEnter();
    virtual void OnExit();

    EStatus m_status;
    CBehaviorTree* m_owner;
};
