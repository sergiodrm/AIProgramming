#pragma once

class CObstacle
{
public:
    static void LoadFromFile(std::vector<std::unique_ptr<CObstacle>>& obstacles_, const char* _filename);

    CObstacle(const USVec2D& _position, float _radius);

    const USVec2D& GetPosition() const;
    float GetRadius() const;

    void DrawDebug() const;
private:
    USVec2D m_position;
    float m_radius {0.f};
};
