#include "stdafx.h"
#include "AlignSteering.h"


#include "character.h"
#include "params.h"

AlignSteering::AlignSteering(Character* owner)
    : m_character(owner), m_desiredVelocity(0.f), m_steering(0.f) {}

float AlignSteering::GetSteering()
{
    if (m_character)
    {
        // Params vars to radians
        const Params& params = m_character->GetParams();
        float maxVelocity = ToRadians(params.maxAngularVelocity);
        float maxAcceleration = ToRadians(params.maxAngularAcceleration);
        float target = ToRadians(params.targetRotation);
        float arriveRadius = ToRadians(params.angularArriveRadius);
        float destRadius = ToRadians(params.angularDestRadius);

        // Character vars
        float currentRotation = ToRadians(m_character->GetRot());
        float currentAngularVelocity = ToRadians(m_character->GetAngularVelocity());

        // Normalize angles
        NormalizeAngle(maxVelocity);
        NormalizeAngle(maxAcceleration);
        NormalizeAngle(target);
        NormalizeAngle(arriveRadius);
        NormalizeAngle(destRadius);
        NormalizeAngle(currentRotation);
        NormalizeAngle(currentAngularVelocity);

        const float deltaRotation = target - currentRotation;
        m_desiredVelocity = deltaRotation > 0 ? 1.f : -1.f;
        if (abs(deltaRotation) < destRadius)
        {
            maxVelocity = 0.f;
        }
        else if (abs(deltaRotation) < arriveRadius)
        {
            const float factor = abs(deltaRotation) / arriveRadius;
            maxVelocity *= factor;
        }
        m_desiredVelocity *= maxVelocity;

        m_steering = m_desiredVelocity - currentAngularVelocity > 0.f ? 1.f : -1.f;
        m_steering *= maxAcceleration;

        return ToDegrees(m_steering);
    }
    return 0.f;
}

void AlignSteering::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    gfxDevice.SetPenColor(USColorVec(1.f, 0.2f, 1.f, 1.f));
    USVec2D position = m_character->GetLoc();
    float target = ToRadians(m_character->GetParams().targetRotation);
    NormalizeAngle(target);

    USVec2D delta = position + USVec2D(cosf(m_desiredVelocity), sinf(m_desiredVelocity)) * 50.f;

    MOAIDraw::DrawLine(position.mX, position.mY, delta.mX, delta.mY);
}

void AlignSteering::NormalizeAngle(float& angleInRadians)
{
    if (angleInRadians < -static_cast<float>(PI) || angleInRadians > static_cast<float>(PI))
    {
        const int rounds = static_cast<int>(fmodf(angleInRadians, PI));
        angleInRadians += 2 * static_cast<float>(PI) * -1.f * static_cast<float>(rounds);
    }
}

float AlignSteering::ToRadians(float angleInDegrees)
{
    return angleInDegrees * static_cast<float>(PI) / 180.f;
}

float AlignSteering::ToDegrees(float angleInRadians)
{
    return angleInRadians * 180.f / static_cast<float>(PI);
}
