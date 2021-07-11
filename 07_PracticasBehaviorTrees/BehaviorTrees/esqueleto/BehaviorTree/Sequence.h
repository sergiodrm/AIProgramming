#pragma once

#include "Group.h"

class CSequence : public CGroup
{
public:
    CSequence(CBehaviorTree* _owner);

    virtual void DrawDebug() const override;

protected:
    virtual void OnEnter() override;
    virtual EStatus OnUpdate(float _deltaTime) override;
    virtual void OnExit() override;

    int m_currentChild;
};
