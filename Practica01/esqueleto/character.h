#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <moaicore/MOAIEntity2D.h>
#include <params.h>

class Character : public MOAIEntity2D
{
public:
  DECL_LUA_FACTORY(Character)
protected:
  void OnStart() override;
  void OnStop() override;
  void OnUpdate(float step) override;
public:
  void DrawDebug() override;

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
private:
  USVec2D mLinearVelocity;
  float mAngularVelocity;

  Params mParams;

  /// Draw debug vars
  USVec2D mDirection;
  USVec2D mSteering;

  // Lua configuration
public:
  void RegisterLuaFuncs(MOAILuaState& state) override;
private:
  static int _setLinearVel(lua_State* L);
  static int _setAngularVel(lua_State* L);
};

#endif
