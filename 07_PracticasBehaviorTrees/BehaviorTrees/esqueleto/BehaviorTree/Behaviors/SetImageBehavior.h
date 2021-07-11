#pragma once

#include "BehaviorTree/Behavior.h"
#include "StateMachine/Actions/SetImageAction.h"

class CSetImageBehavior : public CBehavior
{
public:
    CSetImageBehavior(CBehaviorTree* _owner);

    void SetImage(EImageId _id) { m_imageId = _id; }

protected:
    virtual EStatus OnUpdate(float _deltaTime) override;

private:
    EImageId m_imageId;
};
