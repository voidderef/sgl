#pragma once

#include "io/InputState.h"
#include "io/OutputState.h"

#include "LuaState.h"

namespace sgl {

class LuaIOPHelper
{
public:
    static void AddInputStateToStack(LuaState* refState, const io::InputState& state);

    static void GetInputStateFromStack(LuaState* refState, io::InputState& state);

    static void AddOutputStateToStack(LuaState* refState, const io::OutputState& state);

    static void GetOutputStateFromStack(LuaState* refState, io::OutputState& state);

private:
    LuaIOPHelper() = default;
    ~LuaIOPHelper() = default;
};

}
