#include "LuaTexture.h"

#include "LuaToNativeHelper.h"

namespace sgl {

void LuaTexture::ExposeToState(LuaState* refState, TextureManager* refTextreManager)
{
    refState->RegisterUserdata("_sgl_texture_manager", refTextreManager);

    refState->RegisterFunction("sgl_tex_create", LCreate);
    refState->RegisterFunction("sgl_tex_create_text", LCreateText);
    refState->RegisterFunction("sgl_tex_destroy", LDestroy);
}

int32_t LuaTexture::LCreate(lua_State* state)
{
    auto textureManager = LuaToNativeHelper::GetUserdata<TextureManager>(state, "_sgl_texture_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto path = LuaToNativeHelper::FuncCallGetArg<const char*>(state, argc, 0);
    uint16_t ret = textureManager->CreateTextureFromFile(path);

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaTexture::LCreateText(lua_State* state)
{
    auto textureManager = LuaToNativeHelper::GetUserdata<TextureManager>(state, "_sgl_texture_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto text = LuaToNativeHelper::FuncCallGetArg<std::string>(state, argc, 0);
    auto r = LuaToNativeHelper::FuncCallGetArg<uint8_t>(state, argc, 1);
    auto g = LuaToNativeHelper::FuncCallGetArg<uint8_t>(state, argc, 2);
    auto b = LuaToNativeHelper::FuncCallGetArg<uint8_t>(state, argc, 3);
    auto a = LuaToNativeHelper::FuncCallGetArg<uint8_t>(state, argc, 4);
    auto fontId = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 5);
    uint16_t ret = textureManager->CreateTextureFromText(text, r, g, b, a, fontId);

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaTexture::LDestroy(lua_State* state)
{
    auto textureManager = LuaToNativeHelper::GetUserdata<TextureManager>(state, "_sgl_texture_manager");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    textureManager->DestroyTexture(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

}