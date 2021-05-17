#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
}

#include "sgl/lua/LuaState.h"

#include "sgl/FontManager.h"

namespace sgl {

class LuaFont
{
public:
    static void ExposeToState(LuaState* refState, FontManager* refFontManager);

public:
    static int32_t LCreate(lua_State* state);

    static int32_t LDestroy(lua_State* state);
};

}
