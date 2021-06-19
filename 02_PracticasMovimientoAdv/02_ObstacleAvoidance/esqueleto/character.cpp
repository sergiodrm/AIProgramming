#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>

#include "../../02_ObstacleAvoidance/esqueleto/Obstacle.h"
#include "Steerings/AlignToMovement.h"
#include "Steerings/ArriveSteering.h"
#include "Steerings/ObstacleAvoidance.h"
#include "Steerings/PathFollowing.h"

// ************************************************************************
namespace Math
{
    void NormalizeRadAngle(float& angleInRadians)
    {
        if (angleInRadians < -static_cast<float>(PI) || angleInRadians > static_cast<float>(PI))
        {
            const int rounds = static_cast<int>(fmodf(angleInRadians, PI));
            angleInRadians += 2 * static_cast<float>(PI) * -1.f * static_cast<float>(rounds);
        }
    }

    void NormalizeDegAngle(float& angleInDegrees)
    {
        while (angleInDegrees > 180.f)
        {
            angleInDegrees -= 360.f;
        }
        while (angleInDegrees < -180.f)
        {
            angleInDegrees += 360.f;
        }
    }

    float ToRadians(float angleInDegrees)
    {
        return angleInDegrees * static_cast<float>(PI) / 180.f;
    }

    float ToDegrees(float angleInRadians)
    {
        return angleInRadians * 180.f / static_cast<float>(PI);
    }
}

// ************************************************************************

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
    RTTI_BEGIN
    RTTI_EXTEND(MOAIEntity2D)
    RTTI_END

    m_steerings.push_back(std::make_unique<CObstacleAvoidance>(this));
}

Character::~Character()
{
    m_steerings.clear();
}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
    mPath.Load("path.xml");
    CObstacle::LoadFromFile(m_obstacles, "obstacles.xml");
}

void Character::OnStop()
{}

void Character::OnUpdate(float step)
{
    MOAIEntity2D::OnUpdate(step);

    SSteeringResult steering;
    for (std::unique_ptr<CSteering>& it : m_steerings)
    {
        if (it)
        {
            steering += it->GetSteering(USVec2D());
        }
    }

    mLinearVelocity += steering.Linear * step;
    mAngularVelocity += steering.Angular * step;
    SetLoc(GetLoc() + mLinearVelocity * step);
    SetRot(GetRot() + mAngularVelocity * step);
}

void Character::DrawDebug()
{
    MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

    // Draw target position
    gfxDevice.SetPenColor(0.7f, 0.2f, 0.2f, 1.f);
    MOAIDraw::DrawPoint(mParams.targetPosition.mX, mParams.targetPosition.mY);
    // Arrive radius
    MOAIDraw::DrawEllipseOutline(mParams.targetPosition.mX, mParams.targetPosition.mY, mParams.arrive_radius,
                                 mParams.arrive_radius, 10);
    // Dest radius
    MOAIDraw::DrawEllipseOutline(mParams.targetPosition.mX, mParams.targetPosition.mY, mParams.dest_radius,
                                 mParams.dest_radius, 10);


    gfxDevice.SetPenColor(0.7f, 0.2f, 0.2f, 1.f);

    for (std::unique_ptr<CSteering>& it : m_steerings)
    {
        it->DrawDebug();
    }
    for (std::shared_ptr<CObstacle>& it : m_obstacles)
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
            {nullptr, nullptr}
        };

    luaL_register(state, nullptr, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U")

    float pX = state.GetValue<float>(2, 0.0f);
    float pY = state.GetValue<float>(3, 0.0f);
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
