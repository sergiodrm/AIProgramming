#pragma once
#include "character.h"


class Enemy : public MOAIEntity2D
{
public:
    DECL_LUA_FACTORY(Enemy)
protected:
    virtual void OnStart() override;
    virtual void OnStop() override;
    virtual void OnUpdate(float step) override;
    void SetLinearVelocity(float x, float y) {}
    void SetAngularVelocity(float x) {}
public:
    Enemy();
    ~Enemy();
    virtual void DrawDebug() override;

    // Lua configuration
public:
    virtual void RegisterLuaFuncs(MOAILuaState& state) override;
private:
    static int _setLinearVel(lua_State* L);
    static int _setAngularVel(lua_State* L);
};
