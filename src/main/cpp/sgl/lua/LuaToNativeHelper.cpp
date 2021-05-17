#include "LuaToNativeHelper.h"

namespace sgl {

uint32_t LuaToNativeHelper::FuncCallGetArgCount(lua_State* refState)
{
    // -1, don't count last null argument
    return static_cast<uint32_t>(lua_gettop(refState) - 1);
}

int LuaToNativeHelper::ReturnValueNone(lua_State* refState)
{
    return 0;
}

template<>
bool LuaToNativeHelper::FuncCallGetArg<bool>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    if (idx > argc) {
        throw LuaException(refState, "Invalid argument index %d, argc %d", idx, argc);
    }

    // argument starts with 0 for first argument, but lua starts with 1
    if (!lua_isboolean(refState, idx + 1)) {
        throw LuaException(refState, "Argument %d of invalid type (not bool)", idx);
    }

    return static_cast<bool>(lua_toboolean(refState, idx + 1));
}

template<>
double LuaToNativeHelper::FuncCallGetArg<double>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    if (idx > argc) {
        throw LuaException(refState, "Invalid argument index %d, argc %d", idx, argc);
    }

    // argument starts with 0 for first argument, but lua starts with 1
    if (!lua_isnumber(refState, idx + 1)) {
        throw LuaException(refState, "Argument %d of invalid type (not a number)", idx);
    }

    return static_cast<double>(lua_tonumber(refState, idx + 1));
}

template<>
float LuaToNativeHelper::FuncCallGetArg<float>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<float>(FuncCallGetArg<double>(refState, argc, idx));
}

template<>
int64_t LuaToNativeHelper::FuncCallGetArg<int64_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    if (idx > argc) {
        throw LuaException(refState, "Invalid argument index %d, argc %d", idx, argc);
    }

    // argument starts with 0 for first argument, but lua starts with 1
    if (!lua_isinteger(refState, idx + 1)) {
        throw LuaException(refState, "Argument %d of invalid type (not an integer number)", idx);
    }

    return static_cast<int64_t>(lua_tointeger(refState, idx + 1));
}

template<>
int32_t LuaToNativeHelper::FuncCallGetArg<int32_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<int32_t>(FuncCallGetArg<int64_t>(refState, argc, idx));
}

template<>
int16_t LuaToNativeHelper::FuncCallGetArg<int16_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<int16_t>(FuncCallGetArg<int64_t>(refState, argc, idx));
}

template<>
int8_t LuaToNativeHelper::FuncCallGetArg<int8_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<int8_t>(FuncCallGetArg<int64_t>(refState, argc, idx));
}

template<>
uint64_t LuaToNativeHelper::FuncCallGetArg<uint64_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<uint64_t>(FuncCallGetArg<int64_t>(refState, argc, idx));
}

template<>
uint32_t LuaToNativeHelper::FuncCallGetArg<uint32_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<uint32_t>(FuncCallGetArg<int64_t>(refState, argc, idx));
}

template<>
uint16_t LuaToNativeHelper::FuncCallGetArg<uint16_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<uint16_t>(FuncCallGetArg<int64_t>(refState, argc, idx));
}

template<>
uint8_t LuaToNativeHelper::FuncCallGetArg<uint8_t>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    return static_cast<uint8_t>(FuncCallGetArg<int64_t>(refState, argc, idx));
}

template<>
const char* LuaToNativeHelper::FuncCallGetArg<const char*>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    if (idx > argc) {
        throw LuaException(refState, "Invalid argument index %d, argc %d", idx, argc);
    }

    // argument starts with 0 for first argument, but lua starts with 1
    if (!lua_isstring(refState, idx + 1)) {
        throw LuaException(refState, "Argument %d of invalid type (not a string)", idx);
    }

    return lua_tostring(refState, idx + 1);
}

template<>
std::string LuaToNativeHelper::FuncCallGetArg<std::string>(lua_State* refState, uint32_t argc, uint32_t idx)
{
    auto str = FuncCallGetArg<const char*>(refState, argc, idx);
    return std::string(str);
}

template<>
int LuaToNativeHelper::ReturnValue<bool>(lua_State* refState, bool val)
{
    lua_pushboolean(refState, val);
    return 1;
}

template<>
int LuaToNativeHelper::ReturnValue<double>(lua_State* refState, double val)
{
    lua_pushnumber(refState, val);
    return 1;
}

template<>
int LuaToNativeHelper::ReturnValue<float>(lua_State* refState, float val)
{
    return ReturnValue<double>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<int64_t>(lua_State* refState, int64_t val)
{
    lua_pushinteger(refState, val);
    return 1;
}

template<>
int LuaToNativeHelper::ReturnValue<int32_t>(lua_State* refState, int32_t val)
{
    return ReturnValue<int64_t>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<int16_t>(lua_State* refState, int16_t val)
{
    return ReturnValue<int64_t>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<int8_t>(lua_State* refState, int8_t val)
{
    return ReturnValue<int64_t>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<uint64_t>(lua_State* refState, uint64_t val)
{
    return ReturnValue<int64_t>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<uint32_t>(lua_State* refState, uint32_t val)
{
    return ReturnValue<int64_t>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<uint16_t>(lua_State* refState, uint16_t val)
{
    return ReturnValue<int64_t>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<uint8_t>(lua_State* refState, uint8_t val)
{
    return ReturnValue<int64_t>(refState, val);
}

template<>
int LuaToNativeHelper::ReturnValue<const char*>(lua_State* refState, const char* val)
{
    lua_pushstring(refState, val);
    return 1;
}

int LuaToNativeHelper::ReturnValueTable(lua_State* refState)
{
    return 1;
}

}