#pragma once

#include "Action.h"

class CChaseAction : public CAction
{
public:
    CChaseAction(CStateMachine* _ownerStateMachine);

    virtual void OnUpdate(float _deltaTime) override;
    virtual void OnEnd() override;
    virtual void DrawDebug() override;

private:
    class Character* m_targetCharacter;
};
