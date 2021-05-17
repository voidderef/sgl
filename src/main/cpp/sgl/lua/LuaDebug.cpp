#include "LuaDebug.h"

#include "LuaState.h"

namespace sgl {

std::string LuaDebug::StackToString(lua_State* state)
{
    std::string str;

    // based on source from:
    // https://gist.github.com/tylerneylon/5b9ab5ffcc616cd2a337
    int n = lua_gettop(state);

    for (int i = 1; i <= n; ++i) {
        str += '-' + std::to_string(n - i + 1) + "  ";

        // 0 --> as_key
        __PrintStackPrintItem(state, i, 0, str);
        str += '\n';
    }

    if (n == 0) {
        str += "  <empty>";
    }

    return str;
}

std::string LuaDebug::StackToString(LuaState* refState)
{
    return StackToString(refState->GetState());
}

void LuaDebug::PrintStack(LuaState* refState)
{
    printf("Lua stack:\n%s\n", StackToString(refState).c_str());
}

std::string LuaDebug::GlobalsToString(lua_State* state)
{
    std::string str;

    lua_pushglobaltable(state);

    lua_pushnil(state);

    while (lua_next(state, -2)) {
        str += lua_tostring(state, -2);
        str += " = ";
        __PrintStackPrintItem(state, -1, 0, str);
        str += "\n";

        // remove value
        lua_pop(state, 1);
    }

    // remove global table
    lua_pop(state, 1);

    return str;
}

std::string LuaDebug::GlobalsToString(LuaState* refState)
{
    return GlobalsToString(refState->GetState());
}

void LuaDebug::PrintGlobals(LuaState* refState)
{
    PrintGlobals(refState->GetState());
}

void LuaDebug::PrintGlobals(lua_State* state)
{
    printf("Lua globals\n%s\n", GlobalsToString(state).c_str());
}

void LuaDebug::__PrintStackPrintItem(lua_State* L, int i, int asKey, std::string& str)
{
    int ltype = lua_type(L, i);
    // Set up first, last and start and end delimiters.
    const char *first = (asKey ? "[" : "");
    const char *last  = (asKey ? "]" : "");

    switch(ltype) {
        case LUA_TNIL:
            str += "nil";  // This can't be a key, so we can ignore as_key here.
            return;

        case LUA_TNUMBER:
            if (lua_isinteger(L, i)) {
                str += first + std::to_string(static_cast<int64_t>(lua_tonumber(L, i))) + last;
            } else {
                str += first + std::to_string(lua_tonumber(L, i)) + last;
            }

            return;

        case LUA_TBOOLEAN:
            str += first + std::string(lua_toboolean(L, i) ? "true" : "false") + last;
            return;

        case LUA_TSTRING: {
            const char *s = lua_tostring(L, i);

            if (__PrintStackIsIdentifier(s) && asKey) {
                str += s;
            } else {
                str += first + std::string(s) + last;
            }
        }

        return;

        case LUA_TTABLE:
            str += first;
            __PrintStackPrintTable(L, i, str);
            str += last;
            return;

        case LUA_TFUNCTION:
            str += first + std::string(__PrintStackGetFnString(L, i)) + last;
            return;

        case LUA_TUSERDATA:
        case LUA_TLIGHTUSERDATA:
            str += std::string(first) + "userdata:";
            break;

        case LUA_TTHREAD:
            str += std::string(first) + "thread:";
            break;

        default:
            str += "<internal_error_in_print_stack_item!>";
            return;
    }

    // If we reach here, then we've got a type that we print as a pointer.
    str += std::to_string((const uintptr_t) lua_topointer(L, i)) + last;
}

char* LuaDebug::__PrintStackGetFnString(lua_State* L, int i)
{
    static char fn_name[1024];

    // Ensure i is an absolute index as we'll be pushing/popping things after it.
    if (i < 0) {
        i = lua_gettop(L) + i + 1;
    }

    // Check to see if the function has a global name.
    // stack = [..]
    lua_getglobal(L, "_G");
    // stack = [.., _G]
    lua_pushnil(L);
    // stack = [.., _G, nil]

    while (lua_next(L, -2)) {
        // stack = [.., _G, key, value]
        if (lua_rawequal(L, i, -1)) {
            snprintf(fn_name, 1024, "function:%s", lua_tostring(L, -2));
            lua_pop(L, 3);
            // stack = [..]
            return fn_name;
        }

        // stack = [.., _G, key, value]
        lua_pop(L, 1);
        // stack = [.., _G, key]
    }

    // If we get here, the function didn't have a global name; print a pointer.
    // stack = [.., _G]
    lua_pop(L, 1);
    // stack = [..]
    snprintf(fn_name, 1024, "function:%p", lua_topointer(L, i));
    return fn_name;
}

void LuaDebug::__PrintStackPrintTable(lua_State* L, int i, std::string& str)
{
    // Ensure i is an absolute index as we'll be pushing/popping things after it.
    if (i < 0) {
        i = lua_gettop(L) + i + 1;
    }

    const char *prefix = "{";

    if (__PrintStackIsSeq(L, i)) {
        // This case includes all empty tables.
        __PrintStackPrintSeq(L, i, str);
    } else {
        // stack = [..]
        lua_pushnil(L);
        // stack = [.., nil]

        while (lua_next(L, i)) {
            str += prefix;
            // stack = [.., key, value]
            // 1 --> as_key
            __PrintStackPrintItem(L, -2, 1, str);
            str += " = ";
            // 0 --> as_key
            __PrintStackPrintItem(L, -1, 0, str);
            lua_pop(L, 1);  // So the last-used key is on top.
            // stack = [.., key]
            prefix = ", ";
        }

        // stack = [..]
        str += '}';
    }
}

void LuaDebug::__PrintStackPrintSeq(lua_State* L, int i, std::string& str)
{
    str += '{';

    for (int k = 1;; ++k) {
        // stack = [..]
        lua_rawgeti(L, i, k);

        // stack = [.., t[k]]
        if (lua_isnil(L, -1)) {
            break;
        }

        if (k > 1) {
            str += ", ";
        }

        // 0 --> as_key
        __PrintStackPrintItem(L, -1, 0, str);
        lua_pop(L, 1);
        // stack = [..]
    }

    // stack = [.., nil]
    lua_pop(L, 1);
    // stack = [..]

    str += '}';
}

int LuaDebug::__PrintStackIsSeq(lua_State* L, int i)
{
    // stack = [..]
    lua_pushnil(L);
    // stack = [.., nil]
    int keynum = 1;

    while (lua_next(L, i)) {
        // stack = [.., key, value]
        lua_rawgeti(L, i, keynum);

        // stack = [.., key, value, t[keynum]]
        if (lua_isnil(L, -1)) {
            lua_pop(L, 3);
            // stack = [..]
            return 0;
        }

        lua_pop(L, 2);
        // stack = [.., key]
        keynum++;
    }

  // stack = [..]
  return 1;
}

int LuaDebug::__PrintStackIsIdentifier(const char* s)
{
    while (*s) {
        if (!isalnum(*s) && *s != '_') {
            return 0;
        }

        ++s;
    }

    return 1;
}

}