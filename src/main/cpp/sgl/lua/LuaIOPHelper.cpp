#include "LuaIOPHelper.h"

namespace sgl {

void LuaIOPHelper::AddInputStateToStack(LuaState* refState, const io::InputState& state)
{
    lua_createtable(refState->GetState(), 0, 2);

    lua_pushstring(refState->GetState(), "digital");
    lua_createtable(refState->GetState(), io::InputState::ms_maxDigitalStates, 0);

    for (uint16_t i = 0; i < io::InputState::ms_maxDigitalStates; i++) {
        lua_pushboolean(refState->GetState(), state.GetDigital(i));
        lua_rawseti(refState->GetState(), -2, i);
    }

    lua_settable(refState->GetState(), -3);

    lua_pushstring(refState->GetState(), "analog");
    lua_createtable(refState->GetState(), io::InputState::ms_maxAnalogStates, 0);

    for (uint16_t i = 0; i < io::InputState::ms_maxAnalogStates; i++) {
        lua_pushnumber(refState->GetState(), state.GetAnalog(i));
        lua_rawseti(refState->GetState(), -2, i);
    }

    lua_settable(refState->GetState(), -3);
}

void LuaIOPHelper::GetInputStateFromStack(LuaState* refState, io::InputState& state)
{
    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Value on top of stack is not a table");
    }

    lua_getfield(refState->GetState(), -1, "digital");

    if (lua_isnil(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'digital' in table");
    }

    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'digital' is not a table");
    }

    // iterate table entries
    lua_pushnil(refState->GetState());

    while (lua_next(refState->GetState(), -2)) {
        if (!lua_isnumber(refState->GetState(), -2)) {
            throw LuaException(refState, "Found key in table 'digital' not of type integer");
        }

        auto idx = static_cast<uint16_t>(lua_tointeger(refState->GetState(), -2));

        if (!lua_isboolean(refState->GetState(), -1)) {
            throw LuaException(refState, "Value for index %d in table 'digital' not of type bool", idx);
        }

        auto val = static_cast<bool>(lua_toboolean(refState->GetState(), -1));

        state.SetDigital(idx, val);

        // value, key popped by lua_next
        lua_pop(refState->GetState(), 1);
    }

    // pop digital table/array
    lua_pop(refState->GetState(), 1);

    // ------------------

    lua_getfield(refState->GetState(), -1, "analog");

    if (lua_isnil(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'analog' in table");
    }

    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'analog' is not a table");
    }

    // iterate table entries
    lua_pushnil(refState->GetState());

    while (lua_next(refState->GetState(), -2)) {
        if (!lua_isnumber(refState->GetState(), -2)) {
            throw LuaException(refState, "Found key in table 'analog' not of type integer");
        }

        auto idx = static_cast<uint16_t>(lua_tointeger(refState->GetState(), -2));

        if (!lua_isnumber(refState->GetState(), -1)) {
            throw LuaException(refState, "Value for index %d in table 'analog' not of type float", idx);
        }

        auto val = static_cast<float>(lua_tonumber(refState->GetState(), -1));

        state.SetAnalog(idx, val);

        // value, key popped by lua_next
        lua_pop(refState->GetState(), 1);
    }

    // pop analog table/array
    lua_pop(refState->GetState(), 1);

    // pop digital input state table
    lua_pop(refState->GetState(), 1);
}

void LuaIOPHelper::AddOutputStateToStack(LuaState* refState, const io::OutputState& state)
{
    lua_createtable(refState->GetState(), 0, 2);

    lua_pushstring(refState->GetState(), "digital");
    lua_createtable(refState->GetState(), io::OutputState::ms_maxDigitalStates, 0);

    for (uint16_t i = 0; i < io::OutputState::ms_maxDigitalStates; i++) {
        lua_pushboolean(refState->GetState(), state.GetDigital(i));
        lua_rawseti(refState->GetState(), -2, i);
    }

    lua_settable(refState->GetState(), -3);

    lua_pushstring(refState->GetState(), "analog");
    lua_createtable(refState->GetState(), io::OutputState::ms_maxAnalogFloatStates, 0);

    for (uint16_t i = 0; i < io::OutputState::ms_maxAnalogFloatStates; i++) {
        lua_pushnumber(refState->GetState(), state.GetAnalogFloat(i));
        lua_rawseti(refState->GetState(), -2, i);
    }

    lua_settable(refState->GetState(), -3);

    lua_pushstring(refState->GetState(), "analog_u8");
    lua_createtable(refState->GetState(), io::OutputState::ms_maxAnalogU8States, 0);

    for (uint16_t i = 0; i < io::OutputState::ms_maxAnalogU8States; i++) {
        lua_pushinteger(refState->GetState(), state.GetAnalogU8(i));
        lua_rawseti(refState->GetState(), -2, i);
    }

    lua_settable(refState->GetState(), -3);

    lua_pushstring(refState->GetState(), "text");
    lua_createtable(refState->GetState(), io::OutputState::ms_maxTextStates, 0);

    for (uint16_t i = 0; i < io::OutputState::ms_maxTextStates; i++) {
        lua_pushstring(refState->GetState(), state.GetText(i).c_str());
        lua_rawseti(refState->GetState(), -2, i);
    }

    lua_settable(refState->GetState(), -3);
}

void LuaIOPHelper::GetOutputStateFromStack(LuaState* refState, io::OutputState& state)
{
    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Value on top of stack is not a table");
    }

    lua_getfield(refState->GetState(), -1, "digital");

    if (lua_isnil(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'digital' in table");
    }

    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'digital' is not a table");
    }

    // iterate table entries
    lua_pushnil(refState->GetState());

    while (lua_next(refState->GetState(), -2)) {
        if (!lua_isnumber(refState->GetState(), -2)) {
            throw LuaException(refState, "Found key in table 'digital' not of type integer");
        }

        auto idx = static_cast<uint16_t>(lua_tointeger(refState->GetState(), -2));

        if (!lua_isboolean(refState->GetState(), -1)) {
            throw LuaException(refState, "Value for index %d in table 'digital' not of type bool", idx);
        }

        auto val = static_cast<bool>(lua_toboolean(refState->GetState(), -1));

        state.SetDigital(idx, val);

        // value, key popped by lua_next
        lua_pop(refState->GetState(), 1);
    }

    // pop digital table/array
    lua_pop(refState->GetState(), 1);

    // ------------------

    lua_getfield(refState->GetState(), -1, "analog");

    if (lua_isnil(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'analog' in table");
    }

    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'analog' is not a table");
    }

    // iterate table entries
    lua_pushnil(refState->GetState());

    while (lua_next(refState->GetState(), -2)) {
        if (!lua_isnumber(refState->GetState(), -2)) {
            throw LuaException(refState, "Found key in table 'analog' not of type integer");
        }

        auto idx = static_cast<uint16_t>(lua_tointeger(refState->GetState(), -2));

        if (!lua_isnumber(refState->GetState(), -1)) {
            throw LuaException(refState, "Value for index %d in table 'analog' not of type float", idx);
        }

        auto val = static_cast<float>(lua_tonumber(refState->GetState(), -1));

        state.SetAnalogFloat(idx, val);

        // value, key popped by lua_next
        lua_pop(refState->GetState(), 1);
    }

    // pop analog table/array
    lua_pop(refState->GetState(), 1);

    // ------------------

    lua_getfield(refState->GetState(), -1, "analog_u8");

    if (lua_isnil(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'analog_u8' in table");
    }

    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'analog_u8' is not a table");
    }

    // iterate table entries
    lua_pushnil(refState->GetState());

    while (lua_next(refState->GetState(), -2)) {
        if (!lua_isnumber(refState->GetState(), -2)) {
            throw LuaException(refState, "Found key in table 'analog_u8' not of type integer");
        }

        auto idx = static_cast<uint16_t>(lua_tointeger(refState->GetState(), -2));

        if (!lua_isinteger(refState->GetState(), -1)) {
            throw LuaException(refState, "Value for index %d in table 'analog_u8' not of type int", idx);
        }

        auto val = static_cast<uint8_t>(lua_tointeger(refState->GetState(), -1));

        state.SetAnalogU8(idx, val);

        // value, key popped by lua_next
        lua_pop(refState->GetState(), 1);
    }

    // pop analog table/array
    lua_pop(refState->GetState(), 1);

    // ------------------

    lua_getfield(refState->GetState(), -1, "text");

    if (lua_isnil(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'text' in table");
    }

    if (!lua_istable(refState->GetState(), -1)) {
        throw LuaException(refState, "Missing field 'text' is not a table");
    }

    // iterate table entries
    lua_pushnil(refState->GetState());

    while (lua_next(refState->GetState(), -2)) {
        if (!lua_isnumber(refState->GetState(), -2)) {
            throw LuaException(refState, "Found key in table 'text' not of type string");
        }

        auto idx = static_cast<uint16_t>(lua_tointeger(refState->GetState(), -2));

        if (!lua_isstring(refState->GetState(), -1)) {
            throw LuaException(refState, "Value for index %d in table 'text' not of type string", idx);
        }

        auto val = std::string(lua_tostring(refState->GetState(), -1));

        state.SetText(idx, val);

        // value, key popped by lua_next
        lua_pop(refState->GetState(), 1);
    }

    // pop analog table/array
    lua_pop(refState->GetState(), 1);

    // pop output state table
    lua_pop(refState->GetState(), 1);
}

}