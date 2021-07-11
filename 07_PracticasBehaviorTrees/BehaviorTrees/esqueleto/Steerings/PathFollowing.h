#pragma once

#include "Steering.h"

class CArriveSteering;

class CPathFollowing : public CSteering
{
public:
    CPathFollowing(Character* character);
    virtual ~CPathFollowing();

    virtual void OnDestroy() override;
    const SSteeringResult& GetSteering(const USVec2D& _target) override;
    virtual void DrawDebug() const override;

private:
    USVec2D m_target;
    USVec2D m_closest;
    USVec2D m_projection;
    CArriveSteering* m_arriveSteering;
};
