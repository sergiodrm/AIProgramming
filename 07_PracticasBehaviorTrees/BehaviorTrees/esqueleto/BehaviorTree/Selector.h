#pragma once

#include "Group.h"

class CSelector : public CGroup
{
public:
    CSelector();
protected:
    virtual void OnEnter() override;
    virtual EStatus OnUpdate(float _deltaTime) override;

    int m_currentChild;

    std::vector<CBehavior*> m_behaviors;
};
