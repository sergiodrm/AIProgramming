#pragma once

class Character;

class AlignSteering
{
public:
    AlignSteering(Character* owner);

    float GetSteering();

    void DrawDebug();

    //static void NormalizeAngle(float& angleInRadians);
    //static float ToRadians(float angleInDegrees);
    //static float ToDegrees(float angleInRadians);

private:

    Character* m_character;
    float m_desiredVelocity;
    float m_steering;
};
