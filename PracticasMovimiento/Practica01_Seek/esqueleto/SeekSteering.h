#pragma once


class Character;

class SeekSteering
{
public:

  explicit SeekSteering(Character* _character);

public:

  USVec3D GetSteering();
  void DrawDebug() const;

  void SetTarget(const USVec3D& _target);

private:

  Character* m_character;
  USVec3D m_target;

  USVec3D m_desiredVelocity;
  USVec3D m_acceleration;
};
