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
}

Pathfinder::~Pathfinder()
{}

void Pathfinder::UpdatePath()
{
    USVec2D startGridPosition = m_grid->WorldToGridLocation(m_StartPosition);
    USVec2D endGridPosition = m_grid->WorldToGridLocation(m_EndPosition);

    printf("Start position: [%f, %f]\n", startGridPosition.mX, startGridPosition.mY);
    printf("End position: [%f, %f]\n", endGridPosition.mX, endGridPosition.mY);

    // Add start position in open nodes
    std::shared_ptr<SNode> node = std::make_shared<SNode>();
    node->Parent = nullptr;
    node->Position = startGridPosition;
    node->CostG = 0.f;
    node->CostH = CalculateManhattanDistance(startGridPosition, endGridPosition);

    // Check if start == end
    if (startGridPosition.Equals(endGridPosition))
    {
        printf("Start position equals end position\n");
        m_path.push_back(node);
        return;
    }
    else
    {
        m_openNodes.push_back(node);
    }

    static const std::vector<USVec2D> deltaPosition = {
            {-1.f, -1.f}, {-1.f, 0.f}, {-1.f, 1.f},
            {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f},
            {1.f, -1.f}, {0.f, -1.f}
        };
    bool pathCompleted = false;
    while (!pathCompleted && !m_openNodes.empty())
    {
        // Sort the array in order to pop the first as the best option
        std::sort(m_openNodes.begin(), m_openNodes.end(),
                  [](std::shared_ptr<SNode> _first, std::shared_ptr<SNode> _second) -> bool
                  {
                      return _first->GetCost() < _second->GetCost();
                  }
        );
        node = m_openNodes.at(0);

        // Add it to closed nodes and delete it from open
        m_closeNodes.push_back(node);
        m_openNodes.erase(m_openNodes.begin());

        // Create new nodes
        for (const USVec2D& it : deltaPosition)
        {
            std::shared_ptr<SNode> nextNode = std::make_shared<SNode>();
            nextNode->Parent = node.get();
            nextNode->Position = node->Position + it;
            nextNode->CostG = m_grid->GetCost(nextNode->Position) + node->CostG;
            nextNode->CostH = CalculateManhattanDistance(nextNode->Position, endGridPosition);

            // Check if we achieve the goal
            if (endGridPosition.Equals(nextNode->Position))
            {
                pathCompleted = true;
                node = nextNode;
                break;
            }

            // Search the new node in open list
            for (std::shared_ptr<SNode>& openNode : m_openNodes)
            {
                if (openNode->Position.Equals(nextNode->Position))
                {
                    if (nextNode->GetCost() < openNode->GetCost())
                    {
                        openNode->CostG = nextNode->CostG;
                        openNode->Parent = nextNode->Parent;
                    }
                    nextNode.reset();
                }
                else
                {
                    m_openNodes.push_back(nextNode);
                }
            }
        }
    }

    if (pathCompleted)
    {
        while (node->Parent)
        {
            m_path.push_back(node);
            node.reset(node->Parent);
        }
    }
    else
    {
        printf("Path not found :(\n\n");
    }
}

float Pathfinder::CalculateManhattanDistance(const USVec2D& _start, const USVec2D& _end)
{
    return fabsf(_start.mX - _end.mX) + fabsf(_start.mY - _end.mY);
}

void Pathfinder::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    // Draw grid.
    m_grid->DrawDebug();

    // Draw start and end position.
    gfxDevice.SetPenColor(0.2f, 0.7f, 0.4f, 1.f);
    USVec2D startCenter = m_grid->GridToWorldLocation(m_grid->WorldToGridLocation(m_StartPosition));
    startCenter += m_grid->GetRectSize() * 0.5f;
    MOAIDraw::DrawEllipseFill(startCenter.mX, startCenter.mY, 10.f, 10.f, 10);

    gfxDevice.SetPenColor(0.2f, 0.2f, 0.4f, 1.f);
    USVec2D endCenter = m_grid->GridToWorldLocation(m_grid->WorldToGridLocation(m_EndPosition));
    endCenter += m_grid->GetRectSize() * 0.5f;
    MOAIDraw::DrawEllipseFill(endCenter.mX, endCenter.mY, 10.f, 10.f, 10);


    // Draw path if exists.
    if (!m_path.empty())
    {
        MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

        for (std::shared_ptr<SNode>& it : m_path)
        {
            m_grid->DrawRectangle(it->Position, {0.2f, 1.f, 0.4f, 1.f});
        }
    }
}

bool Pathfinder::PathfindStep()
{
    // returns true if pathfinding process finished
    return true;
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
