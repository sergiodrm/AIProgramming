#include "stdafx.h"
#include "Enemy.h"

void Enemy::OnStart() {}
void Enemy::OnStop() {}

void Enemy::OnUpdate(float step)
{}

Enemy::Enemy()
{
    RTTI_BEGIN
    RTTI_EXTEND(MOAIEntity2D)
    RTTI_END
}

Enemy::~Enemy()
{ }

void Enemy::DrawDebug() {}


// Lua configuration

void Enemy::RegisterLuaFuncs(MOAILuaState& state)
{
    MOAIEntity2D::RegisterLuaFuncs(state);

    luaL_Reg regTable[] = {
            {"setLinearVel", _setLinearVel},
            {"setAngularVel", _setAngularVel},
            {nullptr, nullptr}
        };

    luaL_register(state, nullptr, regTable);
}

int Enemy::_setLinearVel(lua_State* L)
{
    MOAI_LUA_SETUP(Enemy, "U")

    float pX = state.GetValue<float>(2, 0.0f);
    float pY = state.GetValue<float>(3, 0.0f);
    self->SetLinearVelocity(pX, pY);
    return 0;
}

int Enemy::_setAngularVel(lua_State* L)
{
    MOAI_LUA_SETUP(Enemy, "U")

    float angle = state.GetValue<float>(2, 0.0f);
    self->SetAngularVelocity(angle);

    return 0;
}
