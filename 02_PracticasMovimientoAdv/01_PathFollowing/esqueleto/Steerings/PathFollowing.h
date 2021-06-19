#pragma once

#include "Steering.h"

class CPathFollowing : public CSteering
{
public:
    CPathFollowing(Character* character) : CSteering(character) {}
    virtual ~CPathFollowing();

    void OnUpdate(float _deltaTime) override;
    virtual SSteeringResult GetSteering() override;
    virtual void DrawDebug() override;

private:
    USVec2D m_target;
    USVec2D m_closest;
    USVec2D m_projection;
};
