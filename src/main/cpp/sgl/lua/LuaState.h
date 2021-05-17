#pragma once

#include <string>

extern "C" {
#include <lua/lua.h>
}

#include "ks/Object.h"

#include "LuaException.h"

namespace sgl {

class LuaState : public ks::Object
{
public:
    LuaState();

    ~LuaState();

    // for stuff that can't be wrapped, i.e. iterating tables
    lua_State* GetState() const {
        return m_state;
    }

    void LoadStdLibs();

    void RegisterUserdata(const std::string& key, void* ptr);

    void RegisterFunction(const std::string& name, lua_CFunction f);

    template<typename T>
    T LoadScript(const std::string& path);

    // loading with call to script and specified return values. return data
    // must be handled separately using the raw state
    void LoadScript(const std::string& path, uint32_t numReturnValues);

private:
    lua_State* m_state;

private:
    template<typename... Args>
    void __ThrowException(const std::string& msg, Args... args)
    {
        const char* error = lua_tostring(m_state, -1);
        LuaException e(this, msg + ": %s", args..., error);
        lua_pop(m_state, 1);
        throw e;
    }
};

}
