#pragma once

#include "Group.h"

class CSelector : public CGroup
{
public:
    CSelector(CBehaviorTree* _owner);
    virtual void DrawDebug() const override;
protected:
    virtual void OnEnter() override;
    virtual EStatus OnUpdate(float _deltaTime) override;
    virtual void OnExit() override;

    int m_currentChild;
};
