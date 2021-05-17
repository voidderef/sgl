#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
}

#include "sgl/lua/LuaState.h"

#include "sgl/TextureManager.h"

namespace sgl {

class LuaTexture
{
public:
    static void ExposeToState(LuaState* refState, TextureManager* refTextreManager);

public:
    static int32_t LCreate(lua_State* state);

    static int32_t LCreateText(lua_State* state);

    static int32_t LDestroy(lua_State* state);
};

}
