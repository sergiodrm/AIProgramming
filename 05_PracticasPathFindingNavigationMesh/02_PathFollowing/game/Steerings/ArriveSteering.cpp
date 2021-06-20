#include "stdafx.h"
#include "ArriveSteering.h"

#include "character.h"


const SSteeringResult& CArriveSteering::GetSteering(const USVec2D& _target)
{
    if (!m_character)
    {
        return m_steering;
    }

    const SParams& params = m_character->GetParams();
    m_steering = SSteeringResult();

    // Calculate desired velocity
    m_desiredVelocity = _target - m_character->GetLoc();
    const float distanceToTarget = m_desiredVelocity.Length();
    float maxVelocity = distanceToTarget <= params.LinearDestRadius
                            ? 0.f
                            : params.MaxLinearVelocity;
    // Refactor steering depending on the distance to target
    const float arriveRadius = params.LinearArriveRadius;
    if (distanceToTarget < arriveRadius)
    {
        const float factor = distanceToTarget / arriveRadius;
        maxVelocity *= factor;
    }

    // Normalize it to maxVelocity
    m_desiredVelocity.NormSafe();
    m_desiredVelocity *= maxVelocity;

    // Calculate acceleration and normalize it
    USVec3D acceleration = m_desiredVelocity - m_character->GetLinearVelocity();
    acceleration.NormSafe();

    // Steering!!
    m_steering.Linear = acceleration * params.MaxLinearAcceleration;
    return m_steering;
}

void CArriveSteering::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    //// Desired velocity
    //USVec2D position = m_character->GetLoc();
    //USVec2D delta = position + m_desiredVelocity;
    //gfxDevice.SetPenColor(1.f, 0.f, 1.f, 1.f);
    //MOAIDraw::DrawLine(position.mX, position.mY, delta.mX, delta.mY);

    //// Acceleration
    //delta = position + m_steering.Linear;
    //gfxDevice.SetPenColor(0.f, 1.f, 1.f, 1.f);
    //MOAIDraw::DrawLine(position.mX, position.mY, delta.mX, delta.mY);
}
