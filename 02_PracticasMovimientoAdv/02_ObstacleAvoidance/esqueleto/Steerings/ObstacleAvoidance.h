#pragma once
#include "Steering.h"

class CObstacleAvoidance : public CSteering
{
public:
    CObstacleAvoidance(Character* _character)
        : CSteering(_character) {}

    const SSteeringResult& GetSteering(const USVec2D& _target) override;
    void DrawDebug() const override;

private:
    USVec2D m_projLocation;
};
