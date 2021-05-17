#include "LuaAsset.h"

#include "LuaHelper.h"
#include "LuaToNativeHelper.h"

#include "LuaDebug.h"

namespace sgl {

void LuaAsset::ExposeToState(LuaState* refState, DataManager* refAssetManager)
{
    refState->RegisterUserdata("_sgl_asset_manager", refAssetManager);

    refState->RegisterFunction("sgl_asset_paths", LPaths);
}

int32_t LuaAsset::LPaths(lua_State* state)
{
    auto assetManager = LuaToNativeHelper::GetUserdata<DataManager>(state, "_sgl_asset_manager");

    LuaHelper::CreateTable(state, 8);

    LuaHelper::AddElementTable(state, "aud", assetManager->GetRefFsAudio()->GetAbsolutePath());
    LuaHelper::AddElementTable(state, "font", assetManager->GetRefFsFont()->GetAbsolutePath());
    LuaHelper::AddElementTable(state, "io", assetManager->GetRefFsIo()->GetAbsolutePath());
    LuaHelper::AddElementTable(state, "lua", assetManager->GetRefFsLua()->GetAbsolutePath());
    LuaHelper::AddElementTable(state, "mov", assetManager->GetRefFsMov()->GetAbsolutePath());
    LuaHelper::AddElementTable(state, "sfx", assetManager->GetRefFsSfx()->GetAbsolutePath());
    LuaHelper::AddElementTable(state, "shell", assetManager->GetRefFsShell()->GetAbsolutePath());
    LuaHelper::AddElementTable(state, "title", assetManager->GetRefFsTex()->GetAbsolutePath());

    return LuaToNativeHelper::ReturnValueTable(state);
}

}