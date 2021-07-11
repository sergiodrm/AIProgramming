#pragma once

#include "BehaviorTree/Behavior.h"

class CWaitBehavior : public CBehavior
{
public:
    CWaitBehavior(CBehaviorTree* _owner);
    void SetTime(float _time) { m_time = _time; }
protected:
    virtual void OnEnter() override;
    virtual EStatus OnUpdate(float _deltaTime) override;
private:
    float m_time;
    float m_timeUntilSuccess;
};
