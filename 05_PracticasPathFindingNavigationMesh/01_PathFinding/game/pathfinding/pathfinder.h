#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>
#include "NavMesh.h"

class CPath;

struct SNode
{
    SNode* Parent;
    USVec2D Position;
    SPolygon* Polygon;
    float CostH;
    float CostG;
    float GetCost() const { return CostH + CostG; }

    SNode(SNode* _parent, const USVec2D& _position, SPolygon* _polygon, float _costG, float _costH)
        : Parent(_parent), Position(_position), Polygon(_polygon), CostG(_costG), CostH(_costH) {}
};

class Pathfinder : public virtual MOAIEntity2D
{
public:
    Pathfinder();
    ~Pathfinder();

    virtual void DrawDebug();

    void SetStartPosition(float x, float y)
    {
        m_StartPosition = USVec2D(x, y);
        m_navMesh->SelectStartPolygon(m_StartPosition);
        UpdatePath();
    }

    void SetEndPosition(float x, float y)
    {
        m_EndPosition = USVec2D(x, y);
        m_navMesh->SelectEndPolygon(m_EndPosition);
        UpdatePath();
    }

    const USVec2D& GetStartPosition() const { return m_StartPosition; }
    const USVec2D& GetEndPosition() const { return m_EndPosition; }

    bool PathfindStep();
private:
    void UpdatePath();
    float CalculateHeuristic(const USVec2D& _point) const;
    void Clear();
    void FillPath(const SNode* _node);
    SNode* GetNodeInVector(std::vector<SNode*>& _vector, const SPolygon* _polygon);
private:
    USVec2D m_StartPosition;
    USVec2D m_EndPosition;

    CNavMesh* m_navMesh;
    CPath* m_path;

    std::vector<SNode*> m_openNodes;
    std::vector<SNode*> m_closeNodes;

    // Lua configuration
public:
    DECL_LUA_FACTORY(Pathfinder)
public:
    virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
    static int _setStartPosition(lua_State* L);
    static int _setEndPosition(lua_State* L);
    static int _pathfindStep(lua_State* L);
};


#endif
