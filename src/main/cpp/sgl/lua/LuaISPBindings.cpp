#include "LuaISPBindings.h"

#include "ks/Logger.hpp"

#include "sgl/lua/LuaHelper.h"
#include "sgl/lua/LuaIOPHelper.h"
#include "sgl/lua/LuaTime.h"

namespace sgl {

LuaISPBindings::LuaISPBindings(const std::string& scriptPath) :
    m_scriptPath(scriptPath),
    m_state(nullptr),
    m_device(),
    m_screen(),
    m_funcInit(0),
    m_funcActivate(0),
    m_funcDeactivate(0),
    m_funcProcess(0)
{

}

void LuaISPBindings::Load()
{
    m_state = new LuaState();
    m_state->LoadStdLibs();

    LuaTime::ExposeToState(m_state);

    m_state->LoadScript(m_scriptPath, 1);

    if (!lua_istable(m_state->GetState(), -1)) {
        throw LuaException(m_state, "Return value of io processor lua script is not a table");
    }

    m_device = LuaHelper::GetValueFromTable<std::string>(m_state, -1, "device");
    m_screen = LuaHelper::GetValueFromTable<std::string>(m_state, -1, "screen");
    m_funcInit = LuaHelper::GetFunctionReferenceFromTable(m_state, -1, "f_init");
    m_funcActivate = LuaHelper::GetFunctionReferenceFromTable(m_state, -1, "f_activate");
    m_funcDeactivate = LuaHelper::GetFunctionReferenceFromTable(m_state, -1, "f_deactivate");
    m_funcProcess = LuaHelper::GetFunctionReferenceFromTable(m_state, -1, "f_process");

    // pop table
    lua_pop(m_state->GetState(), 1);

    KS_LOG_DEBUG("Loaded io processor bindings: %s", m_scriptPath);
}

void LuaISPBindings::Unload()
{
    m_device.clear();
    m_screen.clear();
    LuaHelper::UnrefReference(m_state, m_funcInit);
    LuaHelper::UnrefReference(m_state, m_funcActivate);
    LuaHelper::UnrefReference(m_state, m_funcDeactivate);
    LuaHelper::UnrefReference(m_state, m_funcProcess);

    delete m_state;
    m_state = nullptr;
}

void LuaISPBindings::LInit()
{
    lua_rawgeti(m_state->GetState(), LUA_REGISTRYINDEX, m_funcInit);

    if (lua_pcall(m_state->GetState(), 0, 0, 0)) {
        __ThrowCallException("Init");
    }
}

void LuaISPBindings::LActivate()
{
    lua_rawgeti(m_state->GetState(), LUA_REGISTRYINDEX, m_funcActivate);

    if (lua_pcall(m_state->GetState(), 0, 0, 0)) {
        __ThrowCallException("Activate");
    }
}

void LuaISPBindings::LDeactivate()
{
    lua_rawgeti(m_state->GetState(), LUA_REGISTRYINDEX, m_funcDeactivate);

    if (lua_pcall(m_state->GetState(), 0, 0, 0)) {
        __ThrowCallException("Deactivate");
    }
}

void LuaISPBindings::LProcess(const io::InputState& src, const io::InputState& srcPrev,
        io::InputState& dest, io::OutputState& out)
{
    lua_rawgeti(m_state->GetState(), LUA_REGISTRYINDEX, m_funcProcess);

    LuaIOPHelper::AddInputStateToStack(m_state, src);
    LuaIOPHelper::AddInputStateToStack(m_state, srcPrev);

    if (lua_pcall(m_state->GetState(), 2, 2, 0)) {
        __ThrowCallException("ProcessInput");
    }

    LuaIOPHelper::GetOutputStateFromStack(m_state, out);
    LuaIOPHelper::GetInputStateFromStack(m_state, dest);
}

void LuaISPBindings::__ThrowCallException(const std::string& function)
{
    const char* error = lua_tostring(m_state->GetState(), -1);
    LuaException e(m_state, "Calling function '%s' failed: %s", function, error);
    lua_pop(m_state->GetState(), 1);
    throw e;
}

}