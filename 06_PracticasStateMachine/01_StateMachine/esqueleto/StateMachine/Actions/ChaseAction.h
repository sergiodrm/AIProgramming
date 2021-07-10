#pragma once

#include "Action.h"

class CChaseAction : public CAction
{
public:
    CChaseAction(CStateMachine* _ownerStateMachine);

    virtual void OnUpdate(float _deltaTime) override;
    virtual void DrawDebug() override;

private:
    float m_secureRadius;
    class Character* m_targetCharacter;
    USVec2D m_targetPosition;
};
