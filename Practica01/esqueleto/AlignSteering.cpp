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
        const Params& params = m_character->GetParams();
        const float rotation = m_character->GetRot();
        float target = params.targetRotation * static_cast<float>(PI) / 180.f;

        // Normalize target angle
        int rounds = static_cast<int>(fmodf(target, PI));
        target += 2 * PI * -1.f * rounds;


        // Desired angular velocity
        m_desiredVelocity = target - rotation;
        float maxAngularVelocity = params.maxAngularVelocity;

        if (m_desiredVelocity < params.angularDestRadius)
            maxAngularVelocity = 0.f;

        if (m_desiredVelocity < params.angularArriveRadius)
        {
            const float factor = m_desiredVelocity / params.angularArriveRadius;
            maxAngularVelocity *= factor;
        }

        m_steering = m_desiredVelocity - m_character->GetAngularVelocity();
        if (m_steering > params.maxAngularAcceleration)
        {
            m_steering = params.maxAngularAcceleration;
        }

        return m_steering;
    }
    return 0.f;
}

void AlignSteering::DrawDebug()
{ }
