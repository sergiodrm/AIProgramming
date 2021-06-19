#include <stdafx.h>
#include "ObstacleAvoidance.h"
#include "character.h"
#include "Obstacle.h"

const SSteeringResult& CObstacleAvoidance::GetSteering(const USVec2D& _target)
{
    if (!m_character)
    {
        printf("Invalid character\n");
        return SSteeringResult();
    }
    m_steering = SSteeringResult();

    const std::vector<std::shared_ptr<CObstacle>>& obstacles = m_character->GetObstacles();
    const float lookAhead = m_character->GetParams().obstacle_look_ahead;
    const float charRadius = m_character->GetParams().char_radius;
    USVec2D characterDirection = m_character->GetLinearVelocity();
    characterDirection.NormSafe();

    for (const std::shared_ptr<CObstacle>& obstacle : obstacles)
    {
        const USVec2D localObstacleLocation = obstacle->GetPosition() - m_character->GetLoc();
        const float proj = localObstacleLocation.Dot(characterDirection);
        if (proj > 0.f)
        {
            if (proj > lookAhead)
            {
                m_projLocation = m_character->GetLoc() + characterDirection * lookAhead;
            }
            else
            {
                m_projLocation = m_character->GetLoc() + characterDirection * proj;
            }
            const USVec2D diff = obstacle->GetPosition() - m_projLocation;
            const float distSquared = diff.LengthSquared();
            float limitCollisionSquared = obstacle->GetRadius() + charRadius;
            limitCollisionSquared *= limitCollisionSquared;
            if (distSquared < limitCollisionSquared)
            {
                // Collision
                USVec2D vecProduct = localObstacleLocation;
                const float result = vecProduct.Cross(m_projLocation);
                if (result >= 0.f)
                {
                    vecProduct.Rotate90Clockwise();
                }
                else
                {
                    vecProduct.Rotate90Anticlockwise();
                }
                vecProduct.NormSafe();
                vecProduct.Scale(m_character->GetParams().max_acceleration);
                m_steering.Linear += vecProduct;
            }
        }
    }

    return m_steering;
}

void CObstacleAvoidance::DrawDebug() const
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    gfxDevice.SetPenColor(0.4f, 0.1f, 0.5f, 1.f);
    MOAIDraw::DrawEllipseOutline(m_projLocation.mX, m_projLocation.mY, m_character->GetParams().char_radius,
                                 m_character->GetParams().char_radius, 10);
    MOAIDraw::DrawLine(USVec2D(m_character->GetLoc()), m_projLocation);
}
