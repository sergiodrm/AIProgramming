#pragma once

struct SLink
{
    struct SPolygon* Neighbour;
    int32_t EdgeStart;
    int32_t EdgeEnd;
};

struct SPolygon
{
    std::vector<USVec2D> Vertex;
    std::vector<SLink> Links;

public:
    USVec2D GetCenter() const;
    bool IsInside(const USVec2D& _point) const;
    USVec2D GetMiddleEdgePoint(const SLink& _link) const;
};

namespace GeometryUtils
{
    bool IsPointInsidePolygon(const SPolygon& _polygon, const USVec2D& _point);
    USVec2D GetCenterOfPolygon(const SPolygon& _polygon);
}

class CNavMesh
{
public:
    CNavMesh();
    ~CNavMesh();

    static CNavMesh* Load(const char* _filename);

    const SPolygon* GetPolygon(const USVec2D& _screenPosition) const;
    SPolygon* GetPolygon(const USVec2D& _screenPosition);

    bool SelectStartPolygon(const USVec2D& _screenPosition);
    bool SelectEndPolygon(const USVec2D& _screenPosition);

    const SPolygon* GetStartPolygon() const { return m_startPolygon; }
    const SPolygon* GetEndPolygon() const { return m_endPolygon; }

    void DrawDebug() const;

private:
    std::vector<SPolygon*> m_polygons;
    SPolygon* m_startPolygon;
    SPolygon* m_endPolygon;
};
