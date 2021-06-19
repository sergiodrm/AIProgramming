#pragma once

class CPath
{
public:
    void AddPoint(const USVec2D& _point);
    void Clear();
    bool Empty() const { return m_path.empty(); }
    void DrawDebug() const;
private:
    std::vector<USVec2D> m_path;
};
