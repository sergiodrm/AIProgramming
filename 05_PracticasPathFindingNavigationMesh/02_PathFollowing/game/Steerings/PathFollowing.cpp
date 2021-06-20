#include "stdafx.h"
#include "PathFollowing.h"

#include "ArriveSteering.h"
#include "character.h"
#include "pathfinding/Path.h"
#include "pathfinding/pathfinder.h"

CPathFollowing::CPathFollowing(Character* character) : CSteering(character)
{
    m_arriveSteering = new CArriveSteering(character);
}

CPathFollowing::~CPathFollowing() {}

const SSteeringResult& CPathFollowing::GetSteering(const USVec2D& _target)
{
    if (!m_character)
    {
        printf("Invalid character\n");
        return m_steering;
    }
    if (m_character->GetPathfinder()->IsPathCompleted())
    {
        // Get time projection
        const USVec2D position = m_character->GetLoc();
        const USVec2D velocity = m_character->GetLinearVelocity();
        m_projection = position + velocity * m_character->GetParams().TimeAhead;

        // Get the closest point in the path.
        const CPath::SPointInSegment pointSegment = m_character->GetPathfinder()->GetPath()->GetClosestPoint(
            m_projection);
        m_closest = pointSegment.SegmentPoint;
        const float lookAhead = m_character->GetParams().LookAhead;
        m_target = m_character->GetPathfinder()->GetPath()->GetPointAhead(pointSegment, lookAhead);
        m_steering = m_arriveSteering->GetSteering(m_target);
    }
    return m_steering;
}

void CPathFollowing::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(0.7f, 0.1f, 0.7f, 1.f);
    MOAIDraw::DrawEllipseFill(m_projection.mX, m_projection.mY, 5.f, 5.f, 10);
    MOAIDraw::DrawLine(USVec2D(m_character->GetLoc()), m_projection);

    gfxDevice.SetPenColor(0.7f, 0.7f, 0.1f, 1.f);
    MOAIDraw::DrawEllipseFill(m_closest.mX, m_closest.mY, 5.f, 5.f, 10);
    MOAIDraw::DrawLine(m_projection, m_closest);

    gfxDevice.SetPenColor(0.f, 0.7f, 0.7f, 1.f);
    MOAIDraw::DrawEllipseFill(m_target.mX, m_target.mY, 5.f, 5.f, 10);
    MOAIDraw::DrawLine(USVec2D(m_character->GetLoc()), m_target);
}
