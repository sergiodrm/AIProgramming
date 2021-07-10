#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <moaicore/MOAIEntity2D.h>
#include <params.h>


class CObstacle;
class CSteering;
class CPath;

typedef std::pair<CSteering*, float> SteeringWeight;

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
    const CPath* GetPath() const { return m_path; }
    const std::vector<CObstacle*> GetObstacles() const { return m_obstacles; }

    void AddSteering(CSteering* _steering, float _weight);


private:
    USVec2D mLinearVelocity;
    float mAngularVelocity;

    Params mParams;

    /** Path following */
    CPath* m_path;
    /** Obstacle avoidance */
    std::vector<CObstacle*> m_obstacles;
    /** Steering behaviors */
    std::map<CSteering*, float> m_steerings;

    // Lua configuration
public:
    virtual void RegisterLuaFuncs(MOAILuaState& state) override;
private:
    static int _setLinearVel(lua_State* L);
    static int _setAngularVel(lua_State* L);
};

#endif
