#pragma once

#include "Steering.h"

class CPathFollowing : public CSteering
{
public:
    CPathFollowing(Character* character) : CSteering(character) {}
    virtual ~CPathFollowing();

    const SSteeringResult& GetSteering(const USVec2D& _target) override;
    virtual void DrawDebug() const override;

private:
    USVec2D m_target;
    USVec2D m_closest;
    USVec2D m_projection;
};
