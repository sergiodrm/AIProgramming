#include <stdafx.h>

#include "pathfinder.h"

#include <algorithm>

#include "Grid.h"

Pathfinder::Pathfinder() : MOAIEntity2D(), m_StartPosition(0.f, 0.f), m_EndPosition(0.f, 0.f)
{
    RTTI_BEGIN
        RTTI_EXTEND(MOAIEntity2D)
    RTTI_END
    m_grid = CGrid::Load("map.txt", "codeMap.txt");
    m_currentNode = nullptr;
}

Pathfinder::~Pathfinder()
{
    m_path.clear();
    m_openNodes.clear();
    m_closeNodes.clear();
}

void Pathfinder::UpdatePath()
{
    m_openNodes.clear();
    m_closeNodes.clear();
    m_path.clear();
    m_openNodes.push_back(new SNode(nullptr, m_startGridPosition, 0, CalculateHeuristic(m_startGridPosition)));
}

float Pathfinder::CalculateHeuristic(const USVec2D& _point)
{
    // Manhattan distance
    return fabsf(_point.mX - m_endGridPosition.mX) + fabsf(_point.mY - m_endGridPosition.mY);
}

void Pathfinder::FillPath(SNode* _end)
{
    SNode* node = _end;
    while (node)
    {
        m_path.insert(m_path.begin(), node);
        node = node->Parent;
    }
}

SNode* Pathfinder::GetNodeInVector(std::vector<SNode*>& _vector, const USVec2D& _nodePosition) const
{
    for (SNode* it : _vector)
    {
        if (it->Position.Equals(_nodePosition))
        {
            return it;
        }
    }
    return nullptr;
}

void Pathfinder::DrawNodes(const std::vector<SNode*>& _vector, float _radius, const USVec4D& _color) const
{
    for (const SNode* it : _vector)
    {
        USVec2D center = m_grid->GridToWorldLocation(it->Position);
        center += m_grid->GetRectSize() * 0.5f;
        MOAIGfxDevice::Get().SetPenColor(_color.mX, _color.mY, _color.mZ, _color.mW);
        MOAIDraw::DrawEllipseFill(center.mX, center.mY, _radius, _radius, 10);
    }
}

void Pathfinder::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    // Draw grid.
    m_grid->DrawDebug();

    // Draw start and end position.
    gfxDevice.SetPenColor(0.2f, 0.7f, 0.4f, 1.f);
    USVec2D startCenter = m_grid->GridToWorldLocation(m_startGridPosition);
    startCenter += m_grid->GetRectSize() * 0.5f;
    MOAIDraw::DrawEllipseFill(startCenter.mX, startCenter.mY, 10.f, 10.f, 10);

    gfxDevice.SetPenColor(0.2f, 0.2f, 0.4f, 1.f);
    USVec2D endCenter = m_grid->GridToWorldLocation(m_endGridPosition);
    endCenter += m_grid->GetRectSize() * 0.5f;
    MOAIDraw::DrawEllipseFill(endCenter.mX, endCenter.mY, 10.f, 10.f, 10);

    DrawNodes(m_closeNodes, 5.f, USVec4D(0.3f, 0.1f, 0.1f, 1.f));
    DrawNodes(m_openNodes, 5.f, USVec4D(0.7f, 0.1f, 0.1f, 1.f));
    DrawNodes(m_path, 5.f, USVec4D(0.2f, 0.4f, 0.8f, 1.f));
}

bool Pathfinder::PathfindStep()
{
    // If path is empty, the pathfinding didn't finish yet
    if (m_path.empty())
    {
        // Sort open list and get the first value
        std::sort(m_openNodes.begin(), m_openNodes.end(), 
            [](const SNode* _first, const SNode* _second) -> bool 
            {
                return _first->GetCost() < _second->GetCost();
            }
        );

        m_currentNode = m_openNodes.at(0);
        m_openNodes.erase(m_openNodes.begin());
        m_closeNodes.push_back(m_currentNode);

        // Check if the current node is our aim
        if (m_currentNode->Position.Equals(m_endGridPosition))
        {
            FillPath(m_currentNode);
            return true;
        }

        static const std::vector<USVec2D> deltaPositionOptions = { 
            /*{-1.f, -1.f},*/ {-1.f, 0.f}, /*{-1.f, 1.f},*/ {0.f, 1.f}, 
            /*{1.f, 1.f},*/ {1.f, 0.f}, /*{1.f, -1.f},*/ {0.f, -1.f} 
        };
        for (const USVec2D& deltaPosition : deltaPositionOptions)
        {
            const USVec2D nextPosition = m_currentNode->Position + deltaPosition;
            // Is next position valid?
            if (m_grid->IsValidPosition(nextPosition) && m_grid->GetCost(nextPosition) >= 0.f)
            {
                SNode* repeatedNode = GetNodeInVector(m_closeNodes, nextPosition);
                // Check if the position represents some node which already exists in any vector
                if (repeatedNode == nullptr)
                {
                    repeatedNode = GetNodeInVector(m_openNodes, nextPosition);
                    const float costG = m_currentNode->CostG + m_grid->GetCost(nextPosition);
                    const float costH = CalculateHeuristic(nextPosition);
                    if (repeatedNode == nullptr)
                    {
                        SNode* newNode = new SNode(m_currentNode, nextPosition, costG, costH);
                        m_openNodes.push_back(newNode);
                    }
                    else
                    {
                        if (costG < repeatedNode->CostG)
                        {
                            repeatedNode->CostG = costG;
                            repeatedNode->Parent = m_currentNode;
                        }
                    }
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
