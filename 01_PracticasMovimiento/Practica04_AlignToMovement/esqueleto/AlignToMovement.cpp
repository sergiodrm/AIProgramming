#include "stdafx.h"
#include "AlignToMovement.h"


#include "character.h"
#include "params.h"

CAlignToMovement::CAlignToMovement(Character* character)
    : CSteering(character) {}

SSteeringResult CAlignToMovement::GetSteering()
{
    if (m_character)
    {
        // Calculate target rotation
        const USVec2D nextLinearVelocity = m_character->GetLinearVelocity();
        float target = Math::ToDegrees(atan2f(nextLinearVelocity.mY, nextLinearVelocity.mX));

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
        Math::NormalizeDegAngle(target);
        Math::NormalizeDegAngle(arriveRadius);
        Math::NormalizeDegAngle(destRadius);
        Math::NormalizeDegAngle(currentRotation);
        Math::NormalizeDegAngle(currentAngularVelocity);

        const float deltaRotation = target - currentRotation;
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


        return m_steering + CallDelegate();
    }
    return SSteeringResult();
}

void CAlignToMovement::DrawDebug() {}
