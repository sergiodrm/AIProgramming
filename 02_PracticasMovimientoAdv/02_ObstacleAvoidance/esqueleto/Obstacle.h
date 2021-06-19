#pragma once

class CObstacle
{
public:
    static void LoadFromFile(std::vector<std::shared_ptr<CObstacle>>& obstacles_, const char* _filename);

    CObstacle(const USVec2D& _position, float _radius);

    const USVec2D& GetPosition() const;
    float GetRadius() const;

    void DrawDebug() const;

    bool IsColliding() const { return m_isColliding; }
    void SetIsColliding(bool _isColliding) { m_isColliding = _isColliding; }

private:
    USVec2D m_position;
    float m_radius {0.f};
    bool m_isColliding {false};
};
