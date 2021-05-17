#include "LuaTime.h"

#include "LuaToNativeHelper.h"

#include "ks/Time.h"

namespace sgl {

void LuaTime::ExposeToState(LuaState* refState)
{
    refState->RegisterFunction("sgl_time_now_sec", LNowSec);
    refState->RegisterFunction("sgl_time_now_ms", LNowMs);
    refState->RegisterFunction("sgl_time_now_us", LNowUs);
    refState->RegisterFunction("sgl_time_now_ns", LNowNs);
}

int32_t LuaTime::LNowSec(lua_State* state)
{
    return LuaToNativeHelper::ReturnValue(state, ks::Time::NowSec());
}

int32_t LuaTime::LNowMs(lua_State* state)
{
    return LuaToNativeHelper::ReturnValue(state, ks::Time::NowMs());
}

int32_t LuaTime::LNowUs(lua_State* state)
{
    return LuaToNativeHelper::ReturnValue(state, ks::Time::NowUs());
}

int32_t LuaTime::LNowNs(lua_State* state)
{
    return LuaToNativeHelper::ReturnValue(state, ks::Time::NowNs());
}

}