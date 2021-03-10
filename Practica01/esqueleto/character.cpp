#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
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

  // Get direction
  USVec3D position = GetLoc();
  USVec3D destination = mParams.targetPosition;
  mDirection = destination - position;
  mDirection.SetLength(1.f);

  // Calculate velocity
  mSteering = mDirection * mParams.max_velocity;
  // Calculate acceleration
  mSteering = mSteering - mLinearVelocity;
  mSteering.Clamp(USVec2D(mParams.max_acceleration, mParams.max_acceleration));
  mLinearVelocity += mSteering;
  mLinearVelocity.Clamp(USVec2D(mParams.max_velocity, mParams.max_velocity));

  // Update position
  position = position + mLinearVelocity * step;
  SetLoc(position);
}

void Character::DrawDebug()
{
  MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
  gfxDevice.SetPenColor(0.7f, 0.2f, 0.2f, 1.f);

  // Draw target
  MOAIDraw::DrawPoint(mParams.targetPosition.mX, mParams.targetPosition.mY);
  MOAIDraw::DrawEllipseOutline(mParams.targetPosition.mX, mParams.targetPosition.mY, mParams.dest_radius,
                               mParams.dest_radius, 10);

  // Draw debug vars
  MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, mDirection.mX, mDirection.mY);
  gfxDevice.SetPenColor(0.7f, 0.7f, 0.2f, 1.f);
  MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, mLinearVelocity.mX, mLinearVelocity.mY);
  gfxDevice.SetPenColor(0.2f, 0.2f, 1.f, 1.f);
  MOAIDraw::DrawLine(GetLoc().mX, GetLoc().mY, mSteering.mX, mSteering.mY);
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
