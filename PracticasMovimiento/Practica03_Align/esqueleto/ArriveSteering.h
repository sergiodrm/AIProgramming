#pragma once


class Character;

class ArriveSteering
{
public:

  ArriveSteering(Character* _character);

public:

  USVec3D GetSteering(const USVec3D& _target);

  void DrawDebug();

private:

  USVec3D m_desiredVelocity;
  USVec3D m_steering;
  Character* m_character;
};
