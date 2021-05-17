#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
}

#include "sgl/lua/LuaState.h"

#include "sgl/VideoManager.h"

namespace sgl {

class LuaVideo
{
public:
    static void ExposeToState(LuaState* refState, VideoManager* refVideoManager);

public:
    static int32_t LCreateVideo(lua_State* state);

    static int32_t LDestroyVideo(lua_State* state);

    static int32_t LTexture(lua_State* state);

    static int32_t LVolume(lua_State* state);

    static int32_t LLoopStart(lua_State* state);

    static int32_t LLoop(lua_State* state);

    static int32_t LPlay(lua_State* state);

    static int32_t LStop(lua_State* state);

    static int32_t LPause(lua_State* state);

    static int32_t LReset(lua_State* state);

    static int32_t LEof(lua_State* state);

    static int32_t LRunning(lua_State* state);

private:
    LuaVideo() = default;
    ~LuaVideo() = default;
};

}
