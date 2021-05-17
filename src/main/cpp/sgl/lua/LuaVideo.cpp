#include "LuaVideo.h"

#include "LuaToNativeHelper.h"

namespace sgl {

void LuaVideo::ExposeToState(LuaState* refState, VideoManager* refVideoManager)
{
    refState->RegisterUserdata("_sgl_video_manager", refVideoManager);

    refState->RegisterFunction("sgl_mov_create", LCreateVideo);
    refState->RegisterFunction("sgl_mov_destroy", LDestroyVideo);
    refState->RegisterFunction("sgl_mov_texture", LTexture);
    refState->RegisterFunction("sgl_mov_volume", LVolume);
    refState->RegisterFunction("sgl_mov_loop_start", LLoopStart);
    refState->RegisterFunction("sgl_mov_loop", LLoop);
    refState->RegisterFunction("sgl_mov_play", LPlay);
    refState->RegisterFunction("sgl_mov_stop", LStop);
    refState->RegisterFunction("sgl_mov_pause", LPause);
    refState->RegisterFunction("sgl_mov_reset", LReset);
    refState->RegisterFunction("sgl_mov_eof", LEof);
    refState->RegisterFunction("sgl_mov_running", LRunning);
}

int32_t LuaVideo::LCreateVideo(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto path = LuaToNativeHelper::FuncCallGetArg<const char*>(state, argc, 0);
    uint16_t ret;

    if (argc > 1) {
        auto withSound = LuaToNativeHelper::FuncCallGetArg<bool>(state, argc, 1);

        ret = videoManager->CreateVideo(path, withSound);
    } else {
        ret = videoManager->CreateVideo(path);
    }

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaVideo::LDestroyVideo(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    videoManager->DestroyVideo(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LTexture(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    uint16_t ret;

    ret = videoManager->GetVideoTexture(id);

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaVideo::LVolume(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto volume = LuaToNativeHelper::FuncCallGetArg<float>(state, argc, 1);

    videoManager->SetVolume(id, volume);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LLoopStart(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto loopStart = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 1);

    videoManager->SetLoopStart(id, loopStart);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LLoop(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto loop = LuaToNativeHelper::FuncCallGetArg<int32_t>(state, argc, 1);

    videoManager->SetLoop(id, loop);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LPlay(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    videoManager->Play(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LStop(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    videoManager->Stop(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LPause(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto pause = LuaToNativeHelper::FuncCallGetArg<bool>(state, argc, 1);

    videoManager->Pause(id, pause);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LReset(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);

    videoManager->Reset(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaVideo::LEof(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    bool ret;

    ret = videoManager->IsEof(id);

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaVideo::LRunning(lua_State* state)
{
    auto videoManager = LuaToNativeHelper::GetUserdata<VideoManager>(state, "_sgl_video_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    bool ret;

    ret = videoManager->IsRunning(id);

    return LuaToNativeHelper::ReturnValue(state, ret);
}

}