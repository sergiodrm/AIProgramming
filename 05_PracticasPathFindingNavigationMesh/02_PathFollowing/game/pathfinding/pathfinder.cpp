#include <stdafx.h>

#include "pathfinder.h"

#include <algorithm>

#include "NavMesh.h"
#include "Path.h"


Pathfinder::Pathfinder() : MOAIEntity2D()
{
    RTTI_BEGIN
    RTTI_EXTEND(MOAIEntity2D)
    RTTI_END

    m_navMesh = CNavMesh::Load("escenario_navmesh.xml");
    m_path = new CPath();

    if (s_instance == nullptr)
    {
        s_instance = this;
    }
}

Pathfinder::~Pathfinder()
{
    Clear();
}

void Pathfinder::UpdatePath()
{
    Clear();

    SPolygon* startPolygon = m_navMesh->GetPolygon(m_StartPosition);
    if (startPolygon)
    {
        m_openNodes.push_back(new SNode(nullptr, m_StartPosition, startPolygon, 0,
                                        CalculateHeuristic(m_StartPosition)));
    }
}

float Pathfinder::CalculateHeuristic(const USVec2D& _point) const
{
    return _point.DistSqrd(m_EndPosition);
}

void Pathfinder::Clear()
{
    for (SNode*& it : m_openNodes)
    {
        if (it)
        {
            delete it;
            it = nullptr;
        }
    }
    for (SNode*& it : m_closeNodes)
    {
        if (it)
        {
            delete it;
            it = nullptr;
        }
    }
    m_path->Clear();
    m_openNodes.clear();
    m_closeNodes.clear();
}

void Pathfinder::FillPath(const SNode* _node)
{
    const SNode* it = _node;
    m_path->AddPoint(m_EndPosition);
    while (it)
    {
        m_path->AddPoint(it->Position);
        it = it->Parent;
    }
}

SNode* Pathfinder::GetNodeInVector(std::vector<SNode*>& _vector, const SPolygon* _polygon)
{
    for (SNode* it : _vector)
    {
        if (it->Polygon == _polygon)
        {
            return it;
        }
    }
    return nullptr;
}

void Pathfinder::DrawDebug()
{
    m_navMesh->DrawDebug();

    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    // Draw start and end position
    gfxDevice.SetPenColor(0.2f, 0.9f, 0.5f, 1.f);
    MOAIDraw::DrawEllipseFill(m_StartPosition.mX, m_StartPosition.mY, 10.f, 10.f, 10);
    gfxDevice.SetPenColor(0.9f, 0.2f, 0.5f, 1.f);
    MOAIDraw::DrawEllipseFill(m_EndPosition.mX, m_EndPosition.mY, 10.f, 10.f, 10);

    // Draw open and close nodes
    gfxDevice.SetPenColor(0.8f, 0.3f, 0.3f, 1.f);
    for (SNode* it : m_openNodes)
    {
        if (it)
        {
            MOAIDraw::DrawEllipseFill(it->Position.mX, it->Position.mY, 5.f, 5.f, 10);
        }
    }
    gfxDevice.SetPenColor(0.3f, 0.3f, 0.3f, 1.f);
    for (SNode* it : m_closeNodes)
    {
        if (it)
        {
            MOAIDraw::DrawEllipseFill(it->Position.mX, it->Position.mY, 5.f, 5.f, 10);
        }
    }

    // Draw path
    m_path->DrawDebug();
}

bool Pathfinder::PathfindStep()
{
    // If path is empty, the pathfinding didn't finish yet
    if (m_path->Empty() && !m_openNodes.empty())
    {
        // Sort open list and get the first value
        std::sort(m_openNodes.begin(), m_openNodes.end(),
                  [](const SNode* _first, const SNode* _second) -> bool
                  {
                      return _first->GetCost() < _second->GetCost();
                  }
        );

        SNode* currentNode = m_openNodes.at(0);
        m_openNodes.erase(m_openNodes.begin());
        m_closeNodes.push_back(currentNode);

        // Check if the current node is our aim
        if (currentNode->Polygon == m_navMesh->GetEndPolygon())
        {
            FillPath(currentNode);
            return true;
        }

        for (const SLink& link : currentNode->Polygon->Links)
        {
            SPolygon* nextPolygon = link.Neighbour;
            SNode* repeatedNode = GetNodeInVector(m_closeNodes, nextPolygon);
            if (!repeatedNode)
            {
                repeatedNode = GetNodeInVector(m_openNodes, nextPolygon);
                const float costG = currentNode->CostG + 1.f;
                if (repeatedNode)
                {
                    if (costG < repeatedNode->CostG)
                    {
                        repeatedNode->CostG = costG;
                        repeatedNode->Parent = currentNode;
                    }
                }
                else
                {
                    const USVec2D pos = currentNode->Polygon->GetMiddleEdgePoint(link);
                    SNode* node = new SNode(currentNode, pos, nextPolygon,
                                            costG, CalculateHeuristic(pos));
                    m_openNodes.push_back(node);
                }
            }
        }
    }
    return false;
}


//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state)
{
    MOAIEntity::RegisterLuaFuncs(state);

    luaL_Reg regTable[] = {
            {"setStartPosition", _setStartPosition},
            {"setEndPosition", _setEndPosition},
            {"pathfindStep", _pathfindStep},
            {nullptr, nullptr}
        };

    luaL_register(state, nullptr, regTable);
}

int Pathfinder::_setStartPosition(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    float pX = state.GetValue<float>(2, 0.0f);
    float pY = state.GetValue<float>(3, 0.0f);
    self->SetStartPosition(pX, pY);
    return 0;
}

int Pathfinder::_setEndPosition(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    float pX = state.GetValue<float>(2, 0.0f);
    float pY = state.GetValue<float>(3, 0.0f);
    self->SetEndPosition(pX, pY);
    return 0;
}

int Pathfinder::_pathfindStep(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    self->PathfindStep();
    return 0;
}
