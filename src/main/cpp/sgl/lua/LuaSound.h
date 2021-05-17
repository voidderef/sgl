#pragma once

#include <cstdint>

extern "C" {
#include <lua/lua.h>
}

#include "sgl/lua/LuaState.h"

#include "sgl/SoundRenderer.h"

namespace sgl {

class LuaSound
{
public:
    static void ExposeToState(LuaState* refState, SoundRenderer* refSoundRenderer);

public:
    static int32_t LCreateSound(lua_State* state);

    static int32_t LDestroySound(lua_State* state);

    static int32_t LVolume(lua_State* state);

    static int32_t LLoop(lua_State* state);

    static int32_t LPlaying(lua_State* state);

    static int32_t LPlay(lua_State* state);

    static int32_t LStop(lua_State* state);

    static int32_t LPause(lua_State* state);

private:
    LuaSound() = default;
    ~LuaSound() = default;
};

}
