#include "LuaRenderer.h"

#include "LuaToNativeHelper.h"

namespace sgl {

void LuaRenderer::ExposeToState(LuaState* refState, Renderer* refRenderer)
{
    refState->RegisterUserdata("_sgl_renderer", refRenderer);

    refState->RegisterFunction("sgl_ren_screen_width", LScreenWidth);
    refState->RegisterFunction("sgl_ren_screen_height", LScreenHeight);
}

int32_t LuaRenderer::LScreenWidth(lua_State* state)
{
    auto renderer = LuaToNativeHelper::GetUserdata<Renderer>(state, "_sgl_renderer");

    auto width = renderer->GetResolutionX();

    return LuaToNativeHelper::ReturnValue(state, width);
}

int32_t LuaRenderer::LScreenHeight(lua_State* state)
{
    auto renderer = LuaToNativeHelper::GetUserdata<Renderer>(state, "_sgl_renderer");

    auto height = renderer->GetResolutionY();

    return LuaToNativeHelper::ReturnValue(state, height);
}

}