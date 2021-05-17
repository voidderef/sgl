#include "LuaSprite.h"

#include "LuaToNativeHelper.h"

namespace sgl {

void LuaSprite::ExposeToState(LuaState* refState, SpriteRenderer* refSpriteRenderer)
{
    refState->RegisterUserdata("_sgl_sprite_renderer", refSpriteRenderer);

    refState->RegisterFunction("sgl_sprite_create", LCreate);
    refState->RegisterFunction("sgl_sprite_destroy", LDestroy);
    refState->RegisterFunction("sgl_sprite_activate", LActivate);
    refState->RegisterFunction("sgl_sprite_pos", LPos);
    refState->RegisterFunction("sgl_sprite_pos_x", LPosX);
    refState->RegisterFunction("sgl_sprite_pos_y", LPosY);
    refState->RegisterFunction("sgl_sprite_dim", LDimensions);
    refState->RegisterFunction("sgl_sprite_dim_x", LDimensionX);
    refState->RegisterFunction("sgl_sprite_dim_y", LDimensionY);
    refState->RegisterFunction("sgl_sprite_alpha", LAlpha);
    refState->RegisterFunction("sgl_sprite_layer", LLayer);
}

int32_t LuaSprite::LCreate(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto texId = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    uint16_t ret = spriteRenderer->CreateSprite(texId);

    return LuaToNativeHelper::ReturnValue(state, ret);
}

int32_t LuaSprite::LDestroy(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    spriteRenderer->DestroySprite(id);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LActivate(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto active = LuaToNativeHelper::FuncCallGetArg<bool>(state, argc, 1);
    spriteRenderer->SpriteSetActive(id, active);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LPos(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto x = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 1);
    auto y = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 2);
    spriteRenderer->SpriteSetPos(id, x, y);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LPosX(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto x = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 1);
    spriteRenderer->SpriteSetX(id, x);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LPosY(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto y = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 1);
    spriteRenderer->SpriteSetY(id, y);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LDimensions(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto x = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 1);
    auto y = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 2);
    spriteRenderer->SpriteSetSize(id, x, y);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LDimensionX(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto x = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 1);
    spriteRenderer->SpriteSetWidth(id, x);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LDimensionY(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto y = LuaToNativeHelper::FuncCallGetArg<uint32_t>(state, argc, 1);
    spriteRenderer->SpriteSetHeight(id, y);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LAlpha(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto val = LuaToNativeHelper::FuncCallGetArg<uint8_t>(state, argc, 1);
    spriteRenderer->SpriteSetAlpha(id, val);

    return LuaToNativeHelper::ReturnValueNone(state);
}

int32_t LuaSprite::LLayer(lua_State* state)
{
    auto spriteRenderer = LuaToNativeHelper::GetUserdata<SpriteRenderer>(state, "_sgl_sprite_renderer");

    uint32_t argc = LuaToNativeHelper::FuncCallGetArgCount(state);

    auto id = LuaToNativeHelper::FuncCallGetArg<uint16_t>(state, argc, 0);
    auto val = LuaToNativeHelper::FuncCallGetArg<uint8_t>(state, argc, 1);
    spriteRenderer->SpriteSetLayer(id, val);

    return LuaToNativeHelper::ReturnValueNone(state);
}

}