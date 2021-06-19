#pragma once
#include "Steering.h"


class Character;

class CArriveSteering : public CSteering
{
public:
    CArriveSteering(Character* character) : CSteering(character) {}

    void OnUpdate(float _deltaTime) override;
    virtual SSteeringResult GetSteering() override;

    virtual void DrawDebug() override;
private:
    USVec2D m_desiredVelocity;
};
