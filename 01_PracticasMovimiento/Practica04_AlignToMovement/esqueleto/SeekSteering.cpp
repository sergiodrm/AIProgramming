#include "stdafx.h"
#include "SeekSteering.h"
#include "character.h"

SeekSteering::SeekSteering(Character* _character)
  : m_character(_character), m_target()
{}

USVec3D SeekSteering::GetSteering()
{
  if (m_character)
  {
    m_desiredVelocity = m_target - m_character->GetLoc();
    USVec3D acceleration = m_desiredVelocity - m_character->GetLinearVelocity();
    acceleration /= acceleration.Length();
    m_acceleration = acceleration * m_character->GetParams().max_acceleration;
    return m_acceleration;
  }
  return USVec3D();
}

void SeekSteering::DrawDebug() const
{
  MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

  // Desired velocity
  USVec2D position = m_character->GetLoc();
  USVec2D delta = position + m_desiredVelocity;
  gfxDevice.SetPenColor(0.9f, 0.2f, 0.2f, 1.f);
  MOAIDraw::DrawLine(position.mX, position.mY, delta.mX, delta.mY);

  // Acceleration
  delta = position + m_acceleration;
  gfxDevice.SetPenColor(0.2f, 0.7f, 0.2f, 1.f);
  MOAIDraw::DrawLine(position.mX, position.mY, delta.mX, delta.mY);
}

void SeekSteering::SetTarget(const USVec3D& _target)
{
  m_target = _target;
}
