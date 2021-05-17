#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
}

#include "sgl/lua/LuaState.h"

#include "sgl/SpriteRenderer.h"

namespace sgl {

class LuaSprite
{
public:
    static void ExposeToState(LuaState* refState, SpriteRenderer* refSpriteRenderer);

public:
    static int32_t LCreate(lua_State* state);

    static int32_t LDestroy(lua_State* state);

    static int32_t LActivate(lua_State* state);

    static int32_t LPos(lua_State* state);

    static int32_t LPosX(lua_State* state);

    static int32_t LPosY(lua_State* state);

    static int32_t LDimensions(lua_State* state);

    static int32_t LDimensionX(lua_State* state);

    static int32_t LDimensionY(lua_State* state);

    static int32_t LAlpha(lua_State* state);

    static int32_t LLayer(lua_State* state);
};

}
