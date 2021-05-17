#include "LuaHelper.h"

extern "C" {
#include <lua/lauxlib.h>
}

namespace sgl {

int LuaHelper::GetFunctionReferenceFromTable(LuaState* state, int stackPos, const char* key)
{
    // push value of field onto stack
    lua_getfield(state->GetState(), stackPos, key);

    if (lua_isnil(state->GetState(), -1)) {
        throw LuaException(state, "Could not find value for key %s in table at %d", key, stackPos);
    }

    if (!lua_isfunction(state->GetState(), -1)) {
        throw LuaException(state, "Value for key %s is not a function in table at %d", key, stackPos);
    }

    return luaL_ref(state->GetState(), LUA_REGISTRYINDEX);
}

int LuaHelper::CreateReference(LuaState* state)
{
    return luaL_ref(state->GetState(), LUA_REGISTRYINDEX);
}

void LuaHelper::UnrefReference(LuaState* state, int luaRef)
{
    luaL_unref(state->GetState(), LUA_REGISTRYINDEX, luaRef);
}

template<>
bool LuaHelper::GetValueFromTable<bool>(LuaState* state, int stackPos, const char* key)
{
    // push value of field onto stack
    lua_getfield(state->GetState(), stackPos, key);

    if (lua_isnil(state->GetState(), -1)) {
        throw LuaException(state, "Could not find value for key %s in table at %d", key, stackPos);
    }

    if (!lua_isboolean(state->GetState(), -1)) {
        throw LuaException(state, "Value for key %s is not a boolean in table at %d", key, stackPos);
    }

    bool ret = static_cast<bool>(lua_toboolean(state->GetState(), -1));
    lua_pop(state->GetState(), 1);
    return ret;
}

template<>
std::string LuaHelper::GetValueFromTable<std::string>(LuaState* state, int stackPos, const char* key)
{
    // push value of field onto stack
    lua_getfield(state->GetState(), stackPos, key);

    if (lua_isnil(state->GetState(), -1)) {
        throw LuaException(state, "Could not find value for key %s in table at %d", key, stackPos);
    }

    if (!lua_isstring(state->GetState(), -1)) {
        throw LuaException(state, "Value for key %s is not a string in table at %d", key, stackPos);
    }

    std::string ret = lua_tostring(state->GetState(), -1);
    lua_pop(state->GetState(), 1);
    return ret;
}

template<>
double LuaHelper::GetValueFromTable<double>(LuaState* state, int stackPos, const char* key)
{
    // push value of field onto stack
    lua_getfield(state->GetState(), stackPos, key);

    if (lua_isnil(state->GetState(), -1)) {
        throw LuaException(state, "Could not find value for key %s in table at %d", key, stackPos);
    }

    if (!lua_isnumber(state->GetState(), -1)) {
        throw LuaException(state, "Value for key %s is not a number in table at %d", key, stackPos);
    }

    double ret = lua_tonumber(state->GetState(), -1);
    lua_pop(state->GetState(), 1);
    return ret;
}

template<>
float LuaHelper::GetValueFromTable<float>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<float>(GetValueFromTable<double>(state, stackPos, key));
}

template<>
int64_t LuaHelper::GetValueFromTable<int64_t>(LuaState* state, int stackPos, const char* key)
{
    // push value of field onto stack
    lua_getfield(state->GetState(), stackPos, key);

    if (lua_isnil(state->GetState(), -1)) {
        throw LuaException(state, "Could not find value for key %s in table at %d", key, stackPos);
    }

    if (!lua_isinteger(state->GetState(), -1)) {
        throw LuaException(state, "Value for key %s is not an integer number in table at %d", key, stackPos);
    }

    int64_t ret = lua_tointeger(state->GetState(), -1);
    lua_pop(state->GetState(), 1);
    return ret;
}

template<>
int32_t LuaHelper::GetValueFromTable<int32_t>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<int32_t>(GetValueFromTable<int64_t>(state, stackPos, key));
}

template<>
int16_t LuaHelper::GetValueFromTable<int16_t>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<int16_t>(GetValueFromTable<int64_t>(state, stackPos, key));
}

template<>
int8_t LuaHelper::GetValueFromTable<int8_t>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<int8_t>(GetValueFromTable<int64_t>(state, stackPos, key));
}

template<>
uint64_t LuaHelper::GetValueFromTable<uint64_t>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<uint64_t>(GetValueFromTable<int64_t>(state, stackPos, key));
}

template<>
uint32_t LuaHelper::GetValueFromTable<uint32_t>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<uint32_t>(GetValueFromTable<int64_t>(state, stackPos, key));
}

template<>
uint16_t LuaHelper::GetValueFromTable<uint16_t>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<uint16_t>(GetValueFromTable<int64_t>(state, stackPos, key));
}

template<>
uint8_t LuaHelper::GetValueFromTable<uint8_t>(LuaState* state, int stackPos, const char* key)
{
    return static_cast<uint8_t>(GetValueFromTable<int64_t>(state, stackPos, key));
}

void LuaHelper::CreateTable(lua_State* state, uint32_t numEntries)
{
    lua_createtable(state, 0, numEntries);
}

template<>
void LuaHelper::AddElementTable<bool>(lua_State* state, const std::string& key, bool value)
{
    lua_pushstring(state, key.c_str());
    lua_pushboolean(state, value);
    lua_settable(state, -3);
}

template<>
void LuaHelper::AddElementTable<double>(lua_State* state, const std::string& key, double value)
{
    lua_pushstring(state, key.c_str());
    lua_pushnumber(state, value);
    lua_settable(state, -3);
}

template<>
void LuaHelper::AddElementTable<float>(lua_State* state, const std::string& key, float value)
{
    AddElementTable(state, key, static_cast<double>(value));
}

template<>
void LuaHelper::AddElementTable<int64_t>(lua_State* state, const std::string& key, int64_t value)
{
    lua_pushstring(state, key.c_str());
    lua_pushinteger(state, value);
    lua_settable(state, -3);
}

template<>
void LuaHelper::AddElementTable<int32_t>(lua_State* state, const std::string& key, int32_t value)
{
    AddElementTable(state, key, static_cast<int64_t>(value));
}

template<>
void LuaHelper::AddElementTable<int16_t>(lua_State* state, const std::string& key, int16_t value)
{
    AddElementTable(state, key, static_cast<int64_t>(value));
}

template<>
void LuaHelper::AddElementTable<int8_t>(lua_State* state, const std::string& key, int8_t value)
{
    AddElementTable(state, key, static_cast<int64_t>(value));
}

template<>
void LuaHelper::AddElementTable<uint64_t>(lua_State* state, const std::string& key, uint64_t value)
{
    AddElementTable(state, key, static_cast<int64_t>(value));
}

template<>
void LuaHelper::AddElementTable<uint32_t>(lua_State* state, const std::string& key, uint32_t value)
{
    AddElementTable(state, key, static_cast<int64_t>(value));
}

template<>
void LuaHelper::AddElementTable<uint16_t>(lua_State* state, const std::string& key, uint16_t value)
{
    AddElementTable(state, key, static_cast<int64_t>(value));
}

template<>
void LuaHelper::AddElementTable<uint8_t>(lua_State* state, const std::string& key, uint8_t value)
{
    AddElementTable(state, key, static_cast<int64_t>(value));
}

template<>
void LuaHelper::AddElementTable<std::string>(lua_State* state, const std::string& key, std::string value)
{
    lua_pushstring(state, key.c_str());
    lua_pushstring(state, value.c_str());
    lua_settable(state, -3);
}

}