#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
};

#include "sgl/lua/LuaState.h"

#include "sgl/DataManager.h"

namespace sgl {

class LuaAsset
{
public:
    static void ExposeToState(LuaState* refState, DataManager* refAssetManager);

public:
    static int32_t LPaths(lua_State* state);

private:
    LuaAsset() = default;
    ~LuaAsset() = default;
};

}
