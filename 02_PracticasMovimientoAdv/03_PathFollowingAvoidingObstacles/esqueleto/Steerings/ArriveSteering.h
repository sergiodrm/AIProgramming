#pragma once
#include "Steering.h"


class Character;

class CArriveSteering : public CSteering
{
public:
    CArriveSteering(Character* character) : CSteering(character) {}

    virtual const SSteeringResult& GetSteering(const USVec2D& _target) override;

    virtual void DrawDebug() const override;
private:
    USVec2D m_desiredVelocity;
};
