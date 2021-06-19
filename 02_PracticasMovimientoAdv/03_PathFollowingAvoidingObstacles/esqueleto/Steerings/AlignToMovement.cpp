#include "stdafx.h"
#include "AlignToMovement.h"


#include "Align.h"
#include "character.h"
#include "params.h"

CAlignToMovement::CAlignToMovement(Character* character)
    : CSteering(character)
{
    m_alignDelegate = new CAlign(character);
}

CAlignToMovement::~CAlignToMovement()
{
    delete m_alignDelegate;
}


const SSteeringResult& CAlignToMovement::GetSteering(float)
{
    if (!m_character)
    {
        return m_steering;
    }
    // Calculate target rotation
    const USVec2D nextLinearVelocity = m_character->GetLinearVelocity();
    const float target = Math::ToDegrees(atan2f(nextLinearVelocity.mY, nextLinearVelocity.mX));

    m_steering = m_alignDelegate->GetSteering(target);
    return m_steering;
}

void CAlignToMovement::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(0.5f, 0.8f, 0.1f, 1.f);
}
