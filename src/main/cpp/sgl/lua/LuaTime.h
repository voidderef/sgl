#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
}

#include "sgl/lua/LuaState.h"

namespace sgl {

class LuaTime
{
public:
    static void ExposeToState(LuaState* refState);

public:
    static int32_t LNowSec(lua_State* state);

    static int32_t LNowMs(lua_State* state);

    static int32_t LNowUs(lua_State* state);

    static int32_t LNowNs(lua_State* state);
};

}
