#include "stdafx.h"
#include "ChaseAction.h"
#include "character.h"
#include "StateMachine/StateMachine.h"

CChaseAction::CChaseAction(CStateMachine* _stateMachine)
    : CAction(_stateMachine), m_secureRadius(100.f), m_targetCharacter(nullptr)
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
        // Get owner and target world position
        const USVec2D ownerWorldPosition = m_ownerStateMachine->GetOwner()->GetLoc();
        const USVec2D enemyWorldPosition = m_targetCharacter->GetLoc();

        // Calculate the distance to target and the direction
        USVec2D direction = enemyWorldPosition - ownerWorldPosition;
        const float distanceToTarget = direction.Length();
        direction.NormSafe();
        // Calculate the position with secure radius in local space
        USVec2D targetPosition = direction * (distanceToTarget - m_secureRadius);

        // Transform local target to world target
        targetPosition += ownerWorldPosition;

        // Move character
        m_ownerStateMachine->GetOwner()->MoveTo(targetPosition);
    }
}

void CChaseAction::DrawDebug()
{
    CAction::DrawDebug();

    MOAIGfxDevice& gfx = MOAIGfxDevice::Get();

    gfx.SetPenColor(0.8f, 0.2f, 0.3f, 1.f);
    MOAIDraw::DrawEllipseOutline(m_targetCharacter->GetLoc().mX, m_targetCharacter->GetLoc().mY, m_secureRadius,
                                 m_secureRadius, 10);
    MOAIDraw::DrawLine(m_ownerStateMachine->GetOwner()->GetLoc().mX, m_ownerStateMachine->GetOwner()->GetLoc().mY,
                       m_targetCharacter->GetLoc().mX, m_targetCharacter->GetLoc().mY);
}
