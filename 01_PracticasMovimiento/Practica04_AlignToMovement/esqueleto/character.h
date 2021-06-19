#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <moaicore/MOAIEntity2D.h>
#include <params.h>

class CSteering;

namespace Math
{
    void NormalizeRadAngle(float& angleInRadians);
    void NormalizeDegAngle(float& angleInDegrees);
    float ToRadians(float angleInDegrees);
    float ToDegrees(float angleInRadians);
}

class Character : public MOAIEntity2D
{
public:
    DECL_LUA_FACTORY(Character)
protected:
    virtual void OnStart() override;
    virtual void OnStop() override;
    virtual void OnUpdate(float step) override;
public:
    virtual void DrawDebug() override;

    Character();
    ~Character();

    void SetLinearVelocity(float x, float y)
    {
        mLinearVelocity.mX = x;
        mLinearVelocity.mY = y;
    }

    void SetAngularVelocity(float angle) { mAngularVelocity = angle; }

    USVec2D GetLinearVelocity() const { return mLinearVelocity; }
    float GetAngularVelocity() const { return mAngularVelocity; }
    const Params& GetParams() const { return mParams; }

private:

    USVec2D mLinearVelocity;
    float mAngularVelocity;

    Params mParams;

    CSteering* m_steering;

    // Lua configuration
public:
    virtual void RegisterLuaFuncs(MOAILuaState& state) override;
private:
    static int _setLinearVel(lua_State* L);
    static int _setAngularVel(lua_State* L);
};

#endif
