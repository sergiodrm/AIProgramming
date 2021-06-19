#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <queue>
#include <moaicore/MOAIEntity2D.h>
#include "Grid.h"

struct SNode;

class Pathfinder : public virtual MOAIEntity2D
{
public:
    Pathfinder();
    ~Pathfinder();

    virtual void DrawDebug();

    void SetStartPosition(float x, float y)
    {
        m_StartPosition = USVec2D(x, y);
        m_startGridPosition = m_grid->WorldToGridLocation(m_StartPosition);
        UpdatePath();
    }

    void SetEndPosition(float x, float y)
    {
        m_EndPosition = USVec2D(x, y);
        m_endGridPosition = m_grid->WorldToGridLocation(m_EndPosition);
        UpdatePath();
    }

    const USVec2D& GetStartPosition() const { return m_StartPosition; }
    const USVec2D& GetEndPosition() const { return m_EndPosition; }

    bool PathfindStep();
private:
    void UpdatePath();
    float CalculateHeuristic(const USVec2D& _point);
    void FillPath(SNode* _end);
    SNode* GetNodeInVector(std::vector<SNode*>& _vector, const USVec2D& _nodePosition) const;
    void DrawNodes(const std::vector<SNode*>& _vector, float _radius, const USVec4D& _color) const;
private:
    USVec2D m_StartPosition;
    USVec2D m_EndPosition;

    USVec2D m_startGridPosition;
    USVec2D m_endGridPosition;

    CGrid* m_grid;
    SNode* m_currentNode;
    std::vector<SNode*> m_openNodes;
    std::vector<SNode*> m_closeNodes;
    std::vector<SNode*> m_path;

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
