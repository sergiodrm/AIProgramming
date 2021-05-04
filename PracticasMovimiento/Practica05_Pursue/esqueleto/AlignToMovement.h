#pragma once
#include "Steering.h"


class CAlignToMovement : public CSteering
{
public:
    CAlignToMovement(Character* character);

    virtual SSteeringResult GetSteering() override;
    virtual void DrawDebug() override;

protected:
    float m_angularVelocityDesired;
};
