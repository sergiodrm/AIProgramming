#pragma once
#include "Steering.h"


class CAlign : public CSteering
{
public:
    CAlign(Character* character);

    virtual const SSteeringResult& GetSteering(float _target) override;
    virtual void DrawDebug() const override;

protected:
    float m_angularVelocityDesired;
};
