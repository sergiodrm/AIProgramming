#pragma once

class CPath
{
public:

    struct SPointInSegment
    {
        USVec2D SegmentPoint {};
        size_t SegmentIndex {0};
    };

    void Load(const char* filename);

    SPointInSegment GetClosestPoint(const USVec2D& _point) const;
    USVec2D GetPointAhead(const SPointInSegment& _point, float _distance) const;
    void DrawDebug();

private:
    std::vector<USVec2D> m_path;
};
