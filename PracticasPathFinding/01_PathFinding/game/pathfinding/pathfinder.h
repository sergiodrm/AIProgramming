#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <queue>
#include <moaicore/MOAIEntity2D.h>

class CGrid;
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
        UpdatePath();
    }

    void SetEndPosition(float x, float y)
    {
        m_EndPosition = USVec2D(x, y);
        UpdatePath();
    }

    const USVec2D& GetStartPosition() const { return m_StartPosition; }
    const USVec2D& GetEndPosition() const { return m_EndPosition; }

    bool PathfindStep();
private:
    void UpdatePath();
    static float CalculateManhattanDistance(const USVec2D& _start, const USVec2D& _end);
private:
    USVec2D m_StartPosition;
    USVec2D m_EndPosition;

    std::shared_ptr<CGrid> m_grid;
    std::vector<std::shared_ptr<SNode>> m_openNodes;
    std::vector<std::shared_ptr<SNode>> m_closeNodes;
    std::vector<std::shared_ptr<SNode>> m_path;

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
