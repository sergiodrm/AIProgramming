#include "stdafx.h"
#include "CheckTargetDistanceCondition.h"

#include "character.h"

CCheckTargetDistanceCondition::CCheckTargetDistanceCondition(Character* _owner)
    : m_owner(_owner), m_triggerSquaredDistance(300.f * 300.f)
{ }

bool CCheckTargetDistanceCondition::Check() const
{
    if (m_owner && m_owner->GetTarget())
    {
        const USVec2D ownerWorldPosition = m_owner->GetLoc();
        const USVec2D targetWorldPosition = m_owner->GetTarget()->GetLoc();

        const USVec2D delta = targetWorldPosition - ownerWorldPosition;
        return delta.LengthSquared() <= m_triggerSquaredDistance;
    }
}

void CCheckTargetDistanceCondition::DrawDebug() const
{
    MOAIGfxDevice::Get().SetPenColor(0.2f, 0.7f, 0.6f, 1.f);
    MOAIDraw::DrawEllipseOutline(m_owner->GetLoc().mX, m_owner->GetLoc().mY, sqrtf(m_triggerSquaredDistance),
                                 sqrtf(m_triggerSquaredDistance), 10);
}

void CCheckTargetDistanceCondition::SetTriggerDistance(float _dist) { m_triggerSquaredDistance = _dist * _dist; }
