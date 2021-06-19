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
};

class CNavMesh
{
public:

    CNavMesh();
    ~CNavMesh();

    static CNavMesh* Load(const char* _filename);

    void DrawDebug() const;

private:

    std::vector<SPolygon*> m_polygons;

};

