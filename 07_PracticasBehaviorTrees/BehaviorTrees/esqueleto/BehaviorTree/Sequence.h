#pragma once

#include "Group.h"

class CSequence : public CGroup
{
public:
    CSequence();
protected:
    virtual void OnEnter() override;
    virtual EStatus OnUpdate(float _deltaTime) override;

    int m_currentChild;

    std::vector<CBehavior*> m_behaviors;
};
