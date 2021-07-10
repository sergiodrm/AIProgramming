#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>

#include "Steerings/AlignToMovement.h"
#include "Steerings/ArriveSteering.h"
#include "Steerings/ObstacleAvoidance.h"
#include "Steerings/PathFollowing.h"
#include "Obstacle.h"
#include "StateMachine/StateMachine.h"

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

Character::Character()
    : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f), m_path(nullptr), m_target(nullptr), m_stateMachine(nullptr)
{
    RTTI_BEGIN
    RTTI_EXTEND(MOAIEntity2D)
    RTTI_END

    // Add steerings
    AddSteering(new CArriveSteering(this));
    AddSteering(new CAlignToMovement(this));

    m_stateMachine = new CStateMachine(this);
}

Character::~Character()
{
    for (std::map<CSteering*, float>::iterator it = m_steerings.begin(); it != m_steerings.end(); ++it)
    {
        if (it->first)
        {
            it->first->OnDestroy();
            delete it->first;
        }
    }
    m_steerings.clear();
}

void Character::AddSteering(CSteering* _steering, float _weight)
{
    assert(_steering != nullptr);
    SteeringWeight newSteering(_steering, _weight);
    m_steerings.insert(m_steerings.end(), newSteering);
}

void Character::MoveTo(const USVec2D& _worldPosition)
{
    m_steeringTarget = _worldPosition;
}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
}

void Character::OnStop()
{}

void Character::OnUpdate(float step)
{
    MOAIEntity2D::OnUpdate(step);

    if (m_stateMachine)
    {
        m_stateMachine->Update(step);
    }

    SSteeringResult steering;
    uint32_t index = 0;
    for (std::map<CSteering*, float>::iterator it = m_steerings.begin(); it != m_steerings.end(); ++it)
    {
        SSteeringResult result;
        result.Linear += it->first->GetSteering(m_steeringTarget).Linear * it->second;
        result.Angular += it->first->GetSteering(0.f).Angular * it->second;
        steering += result;
        ++index;
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


    if (m_stateMachine)
    {
        m_stateMachine->DrawDebug();
    }

    gfxDevice.SetPenColor(0.7f, 0.2f, 0.2f, 1.f);
    for (std::map<CSteering*, float>::iterator it = m_steerings.begin(); it != m_steerings.end(); ++it)
    {
        it->first->DrawDebug();
    }
}


// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
    MOAIEntity2D::RegisterLuaFuncs(state);

    luaL_Reg regTable[] = {
            {"setLinearVel", _setLinearVel},
            {"setAngularVel", _setAngularVel},
            {"moveTo", _moveTo},
            {"loadStateMachine", _loadStateMachine},
            {"startStateMachine", _startStateMachine},
            {"setTarget", _setTarget},
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

int Character::_moveTo(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U");

    float x = state.GetValue<float>(2, 0.f);
    float y = state.GetValue<float>(3, 0.f);
    self->MoveTo(USVec2D(x, y));
    return 0;
}

int Character::_loadStateMachine(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U");

    self->m_stateMachine->Load();
    return 0;
}

int Character::_setTarget(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U");

    self->m_target = state.GetLuaObject<Character>(2, false);
    return 0;
}

int Character::_startStateMachine(lua_State* L)
{
    MOAI_LUA_SETUP(Character, "U");

    self->m_stateMachine->Start();
    return 0;
}
