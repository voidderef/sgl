#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
}

#include "sgl/lua/LuaState.h"

#include "sgl/Renderer.h"

namespace sgl {

class LuaRenderer
{
public:
    static void ExposeToState(LuaState* refState, Renderer* refRenderer);

public:
    static int32_t LScreenWidth(lua_State* state);

    static int32_t LScreenHeight(lua_State* state);

private:
    LuaRenderer() = default;
    ~LuaRenderer() = default;
};

}
