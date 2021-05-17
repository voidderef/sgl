#include "LuaState.h"

extern "C" {
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace sgl {

LuaState::LuaState() :
    m_state(luaL_newstate())
{

}

LuaState::~LuaState()
{
    lua_close(m_state);
}

void LuaState::LoadStdLibs()
{
    luaL_openlibs(m_state);
}

void LuaState::RegisterUserdata(const std::string& key, void* ptr)
{
    lua_pushlightuserdata(m_state, ptr);
    lua_setglobal(m_state, key.c_str());
}

void LuaState::RegisterFunction(const std::string& name, lua_CFunction f)
{
    lua_register(m_state, name.c_str(), f);
}

template<>
void LuaState::LoadScript<void>(const std::string& path)
{
    LoadScript(path, 0);
}

template<>
bool LuaState::LoadScript<bool>(const std::string& path)
{
    LoadScript(path, 1);

    if (!lua_isboolean(m_state, -1)) {
        __ThrowException("Invalid type of return value, not an integer number");
    }

    return static_cast<bool>(lua_toboolean(m_state, -1));
}

template<>
double LuaState::LoadScript<double>(const std::string& path)
{
    LoadScript(path, 1);

    if (!lua_isnumber(m_state, -1)) {
        __ThrowException("Invalid type of return value, not a number");
    }

    return lua_tonumber(m_state, -1);
}

template<>
float LuaState::LoadScript<float>(const std::string& path)
{
    return static_cast<float>(LoadScript<double>(path));
}

template<>
int64_t LuaState::LoadScript<int64_t>(const std::string& path)
{
    LoadScript(path, 1);

    if (!lua_isinteger(m_state, -1)) {
        __ThrowException("Invalid type of return value, not an integer number");
    }

    return lua_tointeger(m_state, -1);
}

template<>
int32_t LuaState::LoadScript<int32_t>(const std::string& path)
{
    return static_cast<int32_t>(LoadScript<int64_t>(path));
}

template<>
int16_t LuaState::LoadScript<int16_t>(const std::string& path)
{
    return static_cast<int16_t>(LoadScript<int64_t>(path));
}

template<>
int8_t LuaState::LoadScript<int8_t>(const std::string& path)
{
    return static_cast<int8_t>(LoadScript<int64_t>(path));
}

template<>
uint64_t LuaState::LoadScript<uint64_t>(const std::string& path)
{
    return static_cast<uint64_t>(LoadScript<int64_t>(path));
}

template<>
uint32_t LuaState::LoadScript<uint32_t>(const std::string& path)
{
    return static_cast<uint32_t>(LoadScript<int64_t>(path));
}

template<>
uint16_t LuaState::LoadScript<uint16_t>(const std::string& path)
{
    return static_cast<uint16_t>(LoadScript<int64_t>(path));
}

template<>
uint8_t LuaState::LoadScript<uint8_t>(const std::string& path)
{
    return static_cast<uint8_t>(LoadScript<int64_t>(path));
}

template<>
std::string LuaState::LoadScript<std::string>(const std::string& path)
{
    LoadScript(path, 1);

    if (!lua_isstring(m_state, -1)) {
        __ThrowException("Invalid type of return value, not an integer number");
    }

    return lua_tostring(m_state, -1);
}

void LuaState::LoadScript(const std::string& path, uint32_t numReturnValues)
{
    int result = luaL_loadfile(m_state, path.c_str());

    if (result != LUA_OK) {
        __ThrowException("Loading lua script file %s failed", path);
    }

   if (lua_pcall(m_state, 0, numReturnValues, 0) != LUA_OK) {
        __ThrowException("Executing lua script %s failed", path);
    }
}

}