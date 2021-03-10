#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

Character::~Character()
{

}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
}

void Character::OnStop()
{

}

void Character::OnUpdate(float step)
{
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);

	vector<USMetaVec2D<float>> targetPolygon;
	targetPolygon.push_back(USMetaVec2D<float>(mParams.targetPosition.mX - 10.f, mParams.targetPosition.mY - 10.f));
	targetPolygon.push_back(USMetaVec2D<float>(mParams.targetPosition.mX + 10.f, mParams.targetPosition.mY - 10.f));
	targetPolygon.push_back(USMetaVec2D<float>(mParams.targetPosition.mX + 10.f, mParams.targetPosition.mY + 10.f));
	targetPolygon.push_back(USMetaVec2D<float>(mParams.targetPosition.mX - 10.f, mParams.targetPosition.mY + 10.f));
	MOAIDraw::DrawPoint(0.0f, 0.0f);
	MOAIDraw::DrawPolygon(targetPolygon);
}





// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
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
	