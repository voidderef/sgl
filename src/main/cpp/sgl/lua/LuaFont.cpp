#include "LuaFont.h"

#include "LuaToNativeHelper.h"

namespace sgl {

void LuaFont::ExposeToState(LuaState* refState, FontManager* refFontManager)
{
    refState->RegisterUserdata("_sgl_font_manager", refFontManager);

    refState->RegisterFunction("sgl_font_create", LCreate);
    refState->RegisterFunction("sgl_font_destroy", LDestroy);
}

int32_t LuaFont::LCreate(lua_State* state)
{
    auto fontManager = LuaToNativeHelper::GetUserdata<FontManager>(state, "_sgl_font_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto path = LuaToNativeHelper::FuncCallGetArg<const char*>(state, argc, 0);
    auto size = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 1);
    uint16_t ret = fontManager->CreateFontFromTTFFile(path, size);

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaFont::LDestroy(lua_State* state)
{
    auto fontManager = LuaToNativeHelper::GetUserdata<FontManager>(state, "_sgl_font_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    fontManager->DestroyFont(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

}