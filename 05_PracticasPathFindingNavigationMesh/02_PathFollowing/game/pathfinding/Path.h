#pragma once

class CPath
{
public:
    struct SPointInSegment
    {
        USVec2D SegmentPoint {};
        size_t SegmentIndex {0};
    };

    void AddPoint(const USVec2D& _point);
    void Clear();
    bool Empty() const { return m_path.empty(); }

    SPointInSegment GetClosestPoint(const USVec2D& _point) const;
    USVec2D GetPointAhead(const SPointInSegment& _point, float _distance) const;

    void DrawDebug() const;
private:
    std::vector<USVec2D> m_path;
};
