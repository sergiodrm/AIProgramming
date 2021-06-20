#include <stdafx.h>
#include "character.h"

#include "pathfinding/pathfinder.h"
#include "Steerings/AlignToMovement.h"
#include "Steerings/PathFollowing.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
    RTTI_BEGIN
    RTTI_EXTEND(MOAIEntity2D)
    RTTI_END

    m_steerings.push_back(new CPathFollowing(this));
    m_steerings.push_back(new CAlignToMovement(this));
    m_pathfinder = new Pathfinder();
    m_params.Load("params.xml");
}

Character::~Character()
{}

void Character::OnStart()
{}

void Character::OnStop()
{}

void Character::OnUpdate(float step)
{
    if (m_pathfinder->IsPathCompleted())
    {
        SSteeringResult steeringResult;
        for (CSteering* steering : m_steerings)
        {
            steeringResult += steering->GetSteering(USVec2D());
        }

        mLinearVelocity += steeringResult.Linear * step;
        mAngularVelocity += steeringResult.Angular * step;
        SetLoc(mLinearVelocity * step + GetLoc());
        SetRot(mAngularVelocity * step + GetRot());
    }
    else
    {
        m_pathfinder->PathfindStep();
    }
}

void Character::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
    gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);

    //MOAIDraw::DrawPoint(0.0f, 0.0f);
    m_pathfinder->DrawDebug();

    for (CSteering* it : m_steerings)
    {
        it->DrawDebug();
    }
}


// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
    MOAIEntity2D::RegisterLuaFuncs(state);

    luaL_Reg regTable[] = {
            {"setLinearVel", _setLinearVel},
            {"setAngularVel", _setAngularVel},
            {"setStartPosition", _setStartPosition},
            {"setEndPosition", _setEndPosition},
            {nullptr, nullptr}
        };

    luaL_register(state, nullptr, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U")

    float pX = state.GetValue<float>(2, 0.0f);
    float pY = state.GetValue<float>(2, 0.0f);
    self->SetLinearVelocity(pX, pY);
    return 0;
}

int Character::_setAngularVel(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U")

    float angle = state.GetValue<float>(2, 0.0f);
    self->SetAngularVelocity(angle);

    return 0;
}

int Character::_setStartPosition(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U")

    float x = state.GetValue<float>(2, 0.0f);
    float y = state.GetValue<float>(3, 0.0f);
    self->GetPathfinder()->SetStartPosition(x, y);
    self->SetLoc(USVec3D(x, y, 0.f));

    return 0;
}

int Character::_setEndPosition(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U")

    float x = state.GetValue<float>(2, 0.0f);
    float y = state.GetValue<float>(3, 0.0f);
    self->GetPathfinder()->SetEndPosition(x, y);

    return 0;
}
