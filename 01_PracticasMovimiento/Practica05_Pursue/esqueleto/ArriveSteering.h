#pragma once
#include "Steering.h"


class Character;

class CArriveSteering : public CSteering
{
public:

    CArriveSteering(Character* character) : CSteering(character) {}

    virtual SSteeringResult GetSteering() override;

    virtual void DrawDebug() override;
};
