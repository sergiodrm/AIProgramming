#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f), m_steering(this)
{
    RTTI_BEGIN
    RTTI_EXTEND(MOAIEntity2D)
    RTTI_END
}

Character::~Character()
{}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
}

void Character::OnStop()
{}

void Character::OnUpdate(float step)
{
    MOAIEntity2D::OnUpdate(step);

    // Acceleration
    float steering = m_steering.GetSteering();
    mAngularVelocity += steering * step;
    printf("Steering: %f\n", steering);

    //Update position
    float rotation = GetRot();
    rotation += mAngularVelocity * step;
    SetRot(rotation);

    /*USVec2D steering = m_steering.GetSteering(mParams.targetPosition);
    mLinearVelocity += steering * step;
    USVec2D position = GetLoc();
    position += mLinearVelocity * step;
    SetLoc(position);*/
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


    // Draw target rotation
    float targetRotation = m_steering.ToRadians(mParams.targetRotation);
    m_steering.NormalizeAngle(targetRotation);
    USVec2D delta = USVec2D(cosf(targetRotation), sinf(targetRotation)) * 50.f + GetLoc();
    MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, delta.mX, delta.mY);

    float arriveRadius = m_steering.ToRadians(mParams.angularArriveRadius);
    delta = USVec2D(cosf(targetRotation + arriveRadius), sinf(targetRotation + arriveRadius)) * 50.f + GetLoc();
    MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, delta.mX, delta.mY);
    delta = USVec2D(cosf(targetRotation - arriveRadius), sinf(targetRotation - arriveRadius)) * 50.f + GetLoc();
    MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, delta.mX, delta.mY);

    // Show current rotation
    gfxDevice.SetPenColor(0.7f, 1.f, 0.2f, 1.f);
    float currentRotation = m_steering.ToRadians(GetRot());
    delta = USVec2D(cosf(currentRotation), sinf(currentRotation)) * 50.f + GetLoc();
    MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, delta.mX, delta.mY);


    m_steering.DrawDebug();
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
