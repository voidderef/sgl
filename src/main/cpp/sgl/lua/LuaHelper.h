#pragma once

#include "LuaState.h"

namespace sgl {

class LuaHelper
{
public:
    // generates a reference so the GC does not collect it
    static int GetFunctionReferenceFromTable(LuaState* state, int stackPos, const char* key);

    // create reference of what's currently on top of the stack
    static int CreateReference(LuaState* state);

    // make sure to unref func references if not used anymore, so they get collected by the GC
    static void UnrefReference(LuaState* state, int luaRef);

    template<typename T>
    static T GetValueFromTable(LuaState* state, int stackPos, const char* key);

    static void CreateTable(LuaState* state, uint32_t numEntries) {
        CreateTable(state->GetState(), numEntries);
    }

    static void CreateTable(lua_State* state, uint32_t numEntries);

    // make sure to call create table before this with the right size of entries
    template<typename T>
    static void AddElementTable(LuaState* state, const std::string& key, T value) {
        AddElementTable(state->GetState(), key, value);
    }

    template<typename T>
    static void AddElementTable(lua_State* state, const std::string& key, T value);

private:
    LuaHelper() = default;

    ~LuaHelper() = default;
};

}
