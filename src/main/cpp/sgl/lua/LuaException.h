#pragma once

#include "ks/Exception.h"

#include "LuaDebug.h"

namespace sgl {

// forward declaration
class LuaState;

class LuaException : public ks::Exception
{
public:
    template<typename... Args>
    explicit LuaException(LuaState* state, const std::string& format, Args... args) :
        Exception(format + "\nStack contents:\n%s", args..., LuaDebug::StackToString(state))
    {

    }

    template<typename... Args>
    explicit LuaException(lua_State* state, const std::string& format, Args... args) :
        Exception(format + "\nStack contents:\n%s", args..., LuaDebug::StackToString(state))
    {

    }

    ~LuaException() override = default;
};

}
