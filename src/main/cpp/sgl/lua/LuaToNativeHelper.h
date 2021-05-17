#pragma once

#include <string>

extern "C" {
#include <lua/lua.h>
}

#include "LuaException.h"

namespace sgl {

class LuaToNativeHelper
{
public:
    template<typename T>
    static T* GetUserdata(lua_State* refState, const std::string& key)
    {
        // add global to top of the stack and remove it
        lua_getglobal(refState, key.c_str());
        return static_cast<T*>(lua_touserdata(refState, -1));
    }

    static uint32_t FuncCallGetArgCount(lua_State* refState);

    template<typename T>
    static T FuncCallGetArg(lua_State* refState, uint32_t argc, uint32_t idx);

    static int ReturnValueNone(lua_State* refState);

    template<typename T>
    static int ReturnValue(lua_State* refState, T val);

    // expecting table on stack
    static int ReturnValueTable(lua_State* refState);
private:
    LuaToNativeHelper() = default;
    ~LuaToNativeHelper() = default;
};

}
