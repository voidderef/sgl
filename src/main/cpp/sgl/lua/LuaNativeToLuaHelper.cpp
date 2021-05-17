#include "LuaNativeToLuaHelper.h"

namespace sgl {

template<>
bool LuaNativeToLuaHelper::GetReturnValueAfterFuncCall<bool>(LuaState* refLuaState)
{
    if (!lua_isboolean(refLuaState->GetState(), -1)) {
        throw LuaException(refLuaState, "Return value after call is not of type bool");
    }

    bool ret = static_cast<bool>(lua_toboolean(refLuaState->GetState(), -1));
    lua_pop(refLuaState->GetState(), 1);
    return ret;
}

}