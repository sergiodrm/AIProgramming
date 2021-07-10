#include "stdafx.h"
#include "ChaseAction.h"
#include "character.h"
#include "StateMachine/StateMachine.h"

CChaseAction::CChaseAction(CStateMachine* _stateMachine)
    : CAction(_stateMachine), m_targetCharacter(nullptr)
{
    if (m_ownerStateMachine)
    {
        m_targetCharacter = m_ownerStateMachine->GetOwner()->GetTarget();
    }
}

void CChaseAction::OnUpdate(float _deltaTime)
{
    CAction::OnUpdate(_deltaTime);
    if (m_targetCharacter)
    {
        const USVec2D enemyWorldPosition = m_targetCharacter->GetLoc();

        // Move character
        m_ownerStateMachine->GetOwner()->MoveTo(enemyWorldPosition);
    }
}

void CChaseAction::OnEnd()
{
    if (m_ownerStateMachine->GetOwner())
    {
        m_ownerStateMachine->GetOwner()->CancelMovement();
    }
}

void CChaseAction::DrawDebug()
{
    CAction::DrawDebug();

    MOAIGfxDevice& gfx = MOAIGfxDevice::Get();

    gfx.SetPenColor(0.8f, 0.2f, 0.3f, 1.f);
    MOAIDraw::DrawLine(m_ownerStateMachine->GetOwner()->GetLoc().mX, m_ownerStateMachine->GetOwner()->GetLoc().mY,
                       m_targetCharacter->GetLoc().mX, m_targetCharacter->GetLoc().mY);
}
