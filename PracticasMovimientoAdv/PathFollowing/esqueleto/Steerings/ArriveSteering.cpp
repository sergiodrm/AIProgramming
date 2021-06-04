#include "stdafx.h"
#include "ArriveSteering.h"

#include "character.h"

void CArriveSteering::OnUpdate(float _deltaTime)
{
    if (!m_character)
    {
        return;
    }

    const Params& params = m_character->GetParams();
    m_steering.Linear = USVec2D();
    m_steering.Angular = 0.f;

    // Calculate desired velocity
    m_desiredVelocity = params.targetPosition - m_character->GetLoc();
    const float distanceToTarget = m_desiredVelocity.Length();
    float maxVelocity = distanceToTarget <= params.dest_radius
                            ? 0.f
                            : params.max_velocity;
    // Refactor steering depending on the distance to target
    const float arriveRadius = params.arrive_radius;
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
    m_steering.Linear = acceleration * params.max_acceleration;
}

SSteeringResult CArriveSteering::GetSteering()
{
    return m_steering;
}

void CArriveSteering::DrawDebug()
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
