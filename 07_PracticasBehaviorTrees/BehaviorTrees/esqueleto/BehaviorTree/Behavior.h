#pragma once

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
    CBehavior();

    EStatus Tick(float _deltaTime);

protected:
    virtual EStatus OnUpdate(float _deltaTime);
    virtual void OnEnter();
    virtual void OnExit();

    EStatus m_status;
};
