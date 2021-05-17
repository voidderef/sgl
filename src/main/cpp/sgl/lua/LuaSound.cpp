#include "LuaSound.h"

#include "LuaToNativeHelper.h"

namespace sgl {

void LuaSound::ExposeToState(LuaState* refState, SoundRenderer* refSoundRenderer)
{
    refState->RegisterUserdata("_sgl_sound_renderer", refSoundRenderer);

    refState->RegisterFunction("sgl_snd_create", LCreateSound);
    refState->RegisterFunction("sgl_snd_destroy", LDestroySound);
    refState->RegisterFunction("sgl_snd_volume", LVolume);
    refState->RegisterFunction("sgl_snd_loop", LLoop);
    refState->RegisterFunction("sgl_snd_playing", LPlaying);
    refState->RegisterFunction("sgl_snd_play", LPlay);
    refState->RegisterFunction("sgl_snd_stop", LStop);
    refState->RegisterFunction("sgl_snd_pause", LPause);
}

int32_t LuaSound::LCreateSound(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto path = LuaToNativeHelper::FuncCallGetArg<const char*>(state, argc, 0);
    uint16_t ret;

    if (argc > 1) {
        auto streamed = LuaToNativeHelper::FuncCallGetArg<bool>(state, argc, 1);

        ret = soundRenderer->CreateSound(path, streamed);
    } else {
        ret = soundRenderer->CreateSound(path);
    }

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaSound::LDestroySound(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    soundRenderer->DestorySound(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSound::LVolume(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto volume = LuaToNativeHelper::FuncCallGetArg<float>(state, argc, 1);

    soundRenderer->SetVolume(id, volume);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSound::LLoop(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto loop = LuaToNativeHelper::FuncCallGetArg<int32_t>(state, argc, 1);

    soundRenderer->SetLoop(id, loop);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSound::LPlaying(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    bool ret = soundRenderer->IsPlaying(id);

    return LuaToNativeHelper::ReturnValue<bool>(state, ret);
}

int32_t LuaSound::LPlay(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    soundRenderer->Play(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSound::LStop(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    soundRenderer->Stop(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSound::LPause(lua_State* state)
{
    auto soundRenderer = LuaToNativeHelper::GetUserdata<SoundRenderer>(state, "_sgl_sound_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    bool pause = LuaToNativeHelper::FuncCallGetArg<bool>(state, argc, 1);

    soundRenderer->Pause(id, pause);

    return LuaToNativeHelper::ReturnValueNone(state);
}

}