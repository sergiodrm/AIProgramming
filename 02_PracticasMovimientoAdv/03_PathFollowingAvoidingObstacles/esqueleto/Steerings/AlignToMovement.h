#pragma once
#include "Steering.h"


class CAlign;

class CAlignToMovement : public CSteering
{
public:
    CAlignToMovement(Character* character);
    ~CAlignToMovement();

    virtual const SSteeringResult& GetSteering(float _target) override;
    virtual void DrawDebug() const override;

protected:
    float m_angularVelocityDesired;

    CAlign* m_alignDelegate;
};
