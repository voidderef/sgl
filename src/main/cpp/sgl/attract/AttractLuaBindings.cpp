#include "AttractLuaBindings.h"

#include "ks/Logger.hpp"

#include "sgl/lua/LuaHelper.h"
#include "sgl/lua/LuaIOPHelper.h"
#include "sgl/lua/LuaNativeToLuaHelper.h"

namespace sgl {

AttractLuaBindings::AttractLuaBindings(const std::string& scriptPath) :
    m_scriptPath(scriptPath),
    m_refState(nullptr),
    m_funcLoad(0),
    m_funcUnload(0),
    m_funcUpdate(0),
    m_funcOnEnable(0),
    m_funcOnDisable(0),
    m_funcOnConfirm(0)
{

}

void AttractLuaBindings::Load(LuaState* refLuaState)
{
    refLuaState->LoadScript(m_scriptPath, 1);

    if (!lua_istable(refLuaState->GetState(), -1)) {
        throw LuaException(refLuaState, "Return value of attract lua script is not a table");
    }

    m_funcLoad = LuaHelper::GetFunctionReferenceFromTable(refLuaState, -1, "f_load");
    m_funcUnload = LuaHelper::GetFunctionReferenceFromTable(refLuaState, -1, "f_unload");
    m_funcUpdate = LuaHelper::GetFunctionReferenceFromTable(refLuaState, -1, "f_update");
    m_funcOnEnable = LuaHelper::GetFunctionReferenceFromTable(refLuaState, -1, "f_on_enable");
    m_funcOnDisable = LuaHelper::GetFunctionReferenceFromTable(refLuaState, -1, "f_on_disable");
    m_funcOnConfirm = LuaHelper::GetFunctionReferenceFromTable(refLuaState, -1, "f_on_confirm");
    m_funcOnTransition = LuaHelper::GetFunctionReferenceFromTable(refLuaState, -1, "f_on_transition");

    // pop table
    lua_pop(refLuaState->GetState(), 1);

    m_refState = refLuaState;

    KS_LOG_DEBUG("Loaded attract bindings: %s", m_scriptPath);
}

void AttractLuaBindings::Unload()
{
    LuaHelper::UnrefReference(m_refState, m_funcLoad);
    LuaHelper::UnrefReference(m_refState, m_funcUnload);
    LuaHelper::UnrefReference(m_refState, m_funcUpdate);
    LuaHelper::UnrefReference(m_refState, m_funcOnEnable);
    LuaHelper::UnrefReference(m_refState, m_funcOnDisable);
    LuaHelper::UnrefReference(m_refState, m_funcOnConfirm);

    m_refState = nullptr;
}

bool AttractLuaBindings::LLoad(int luaRefTableResources, int luaRefTableArgs)
{
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, m_funcLoad);

    // get the resource table using the reference and push to stack
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, luaRefTableResources);
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, luaRefTableArgs);

    if (lua_pcall(m_refState->GetState(), 2, 1, 0)) {
        __ThrowCallException("Load");
    }

    return LuaNativeToLuaHelper::GetReturnValueAfterFuncCall<bool>(m_refState);
}

void AttractLuaBindings::LUnload()
{
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, m_funcUnload);

    if (lua_pcall(m_refState->GetState(), 0, 0, 0)) {
        __ThrowCallException("Unload");
    }
}

void AttractLuaBindings::LUpdate(double delta, const io::InputState& input)
{
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, m_funcUpdate);

    lua_pushnumber(m_refState->GetState(), delta);
    LuaIOPHelper::AddInputStateToStack(m_refState, input);

    if (lua_pcall(m_refState->GetState(), 2, 0, 0)) {
        __ThrowCallException("Update");
    }
}

void AttractLuaBindings::LOnEnable()
{
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, m_funcOnEnable);

    if (lua_pcall(m_refState->GetState(), 0, 0, 0)) {
        __ThrowCallException("OnEnable");
    }
}

void AttractLuaBindings::LOnDisable()
{
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, m_funcOnDisable);

    if (lua_pcall(m_refState->GetState(), 0, 0, 0)) {
        __ThrowCallException("OnDisable");
    }
}

bool AttractLuaBindings::LOnConfirm()
{
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, m_funcOnConfirm);

    if (lua_pcall(m_refState->GetState(), 0, 1, 0)) {
        __ThrowCallException("OnConfirm");
    }

    return LuaNativeToLuaHelper::GetReturnValueAfterFuncCall<bool>(m_refState);
}

bool AttractLuaBindings::LOnTransition()
{
    lua_rawgeti(m_refState->GetState(), LUA_REGISTRYINDEX, m_funcOnTransition);

    if (lua_pcall(m_refState->GetState(), 0, 1, 0)) {
        __ThrowCallException("OnTransition");
    }

    return LuaNativeToLuaHelper::GetReturnValueAfterFuncCall<bool>(m_refState);
}

void AttractLuaBindings::__ThrowCallException(const std::string& function)
{
    const char* error = lua_tostring(m_refState->GetState(), -1);
    LuaException e(m_refState, "Calling function '%s' failed: %s", function, error);
    lua_pop(m_refState->GetState(), 1);
    throw e;
}

}