#include "stdafx.h"
#include "Align.h"


#include "character.h"
#include "params.h"

CAlign::CAlign(Character* character)
    : CSteering(character) {}


const SSteeringResult& CAlign::GetSteering(float _target)
{
    if (!m_character)
    {
        return SSteeringResult();
    }
    // Params vars to radians
    const Params& params = m_character->GetParams();
    float maxVelocity = params.maxAngularVelocity;
    float maxAcceleration = params.maxAngularAcceleration;
    float arriveRadius = params.angularArriveRadius;
    float destRadius = params.angularDestRadius;

    // Character vars
    float currentRotation = m_character->GetRot();
    float currentAngularVelocity = m_character->GetAngularVelocity();

    // Normalize angles
    Math::NormalizeDegAngle(maxVelocity);
    Math::NormalizeDegAngle(maxAcceleration);
    Math::NormalizeDegAngle(_target);
    Math::NormalizeDegAngle(arriveRadius);
    Math::NormalizeDegAngle(destRadius);
    Math::NormalizeDegAngle(currentRotation);
    Math::NormalizeDegAngle(currentAngularVelocity);

    const float deltaRotation = _target - currentRotation;
    m_angularVelocityDesired = deltaRotation > 0 ? 1.f : -1.f;
    if (abs(deltaRotation) < destRadius)
    {
        maxVelocity = 0.f;
    }
    else if (abs(deltaRotation) < arriveRadius)
    {
        const float factor = abs(deltaRotation) / arriveRadius;
        maxVelocity *= factor;
    }
    m_angularVelocityDesired *= maxVelocity;

    m_steering.Angular = m_angularVelocityDesired - currentAngularVelocity > 0.f ? 1.f : -1.f;
    m_steering.Angular *= maxAcceleration;
    return m_steering;
}

void CAlign::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(0.5f, 0.8f, 0.1f, 1.f);
}
