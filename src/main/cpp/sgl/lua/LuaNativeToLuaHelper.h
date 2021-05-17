#pragma once

#include "LuaState.h"

namespace sgl {

class LuaNativeToLuaHelper
{
public:
    template<typename T>
    static T GetReturnValueAfterFuncCall(LuaState* refLuaState);

private:
    LuaNativeToLuaHelper() = default;

    ~LuaNativeToLuaHelper() = default;
};

}
