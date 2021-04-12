#pragma once

class Character;

class AlignSteering
{
public:
    AlignSteering(Character* owner);

    float GetSteering();

    void DrawDebug();

private:

    Character* m_character;
    float m_desiredVelocity;
    float m_steering;
};
