#pragma once

#include "Steering.h"

class CPursue : public CSteering
{
public:
    CPursue(Character* character) : CSteering(character) {}
    virtual ~CPursue();

    void OnUpdate(float _deltaTime) override;
    virtual SSteeringResult GetSteering() override;
    virtual void DrawDebug() override;

private:
    USVec2D m_target;
    USVec2D m_closest;
    USVec2D m_projection;
};
