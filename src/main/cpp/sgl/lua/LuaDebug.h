#pragma once

#include <string>

struct lua_State;

namespace sgl {

// forward declaration
class LuaState;

class LuaDebug
{
public:
    static std::string StackToString(lua_State* state);

    static std::string StackToString(LuaState* refState);

    static void PrintStack(LuaState* refState);

    static std::string GlobalsToString(lua_State* state);

    static std::string GlobalsToString(LuaState* refState);

    static void PrintGlobals(LuaState* refState);

    static void PrintGlobals(lua_State* state);

private:
    static void __PrintStackPrintItem(lua_State* L, int i, int asKey, std::string& str);
    static char* __PrintStackGetFnString(lua_State* L, int i);
    static void __PrintStackPrintTable(lua_State* L, int i, std::string& str);
    static void __PrintStackPrintSeq(lua_State* L, int i, std::string& str);
    static int __PrintStackIsSeq(lua_State* L, int i);
    static int __PrintStackIsIdentifier(const char* s);

private:
    LuaDebug() = default;

    ~LuaDebug() = default;
};

}
