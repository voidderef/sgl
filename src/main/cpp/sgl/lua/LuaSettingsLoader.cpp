#include "LuaSettingsLoader.h"

namespace sgl {

LuaSettingsLoader::LuaSettingsLoader(const std::string& luaSettingsScript) :
    m_luaSettingsScript(luaSettingsScript)
{

}

void LuaSettingsLoader::Load(ks::Settings& settings)
{
    auto state = new LuaState();
    state->LoadScript(m_luaSettingsScript, 1);

    if (!lua_istable(state->GetState(), -1)) {
        throw LuaException(state, "Return value of settings script is not a table");
    }

    __ProcessTable(settings, state, "");

    // remove table from stack
    lua_pop(state->GetState(), 1);

    delete state;
}

void LuaSettingsLoader::__ProcessTable(ks::Settings& settings, LuaState* refState, std::string key)
{
    // make sure lua_next starts at the beginning
    lua_pushnil(refState->GetState());

    // table located at -2 in stack
    while (lua_next(refState->GetState(), -2)) {
        // key at -2
        // value at -1

        if (!lua_isstring(refState->GetState(), -2)) {
            throw LuaException(refState, "Key of table must be a string");
        }

        std::string subKey = lua_tostring(refState->GetState(), -2);

        if (!key.empty()) {
            subKey = key + ks::Settings::ms_delim + subKey;
        }

        ks::Settings::Entry* entry = nullptr;

        if (lua_istable(refState->GetState(), -1)) {
            // check if table is actually an array -> numerical index
            lua_rawgeti(refState->GetState(), -1, 1);

            if (lua_isnil(refState->GetState(), -1)) {
                // pop nil value
                lua_pop(refState->GetState(), 1);

                // value is a nested table
                __ProcessTable(settings, refState, subKey);
            } else {
                // array

                // determine type
                ks::Settings::Entry::Type type = ks::Settings::Entry::e_TypeInt;

                if (lua_isstring(refState->GetState(), -1)) {
                    type = ks::Settings::Entry::e_TypeStringList;
                }

                lua_pop(refState->GetState(), 1);

                if (type == ks::Settings::Entry::e_TypeStringList) {
                    std::vector<std::string> values;

                    // iterate table entries
                    lua_pushnil(refState->GetState());

                    while (lua_next(refState->GetState(), -2)) {
                        if (!lua_isnumber(refState->GetState(), -2)) {
                            throw LuaException(refState, "Found key in array of non type integer");
                        }

                        auto idx = static_cast<size_t>(lua_tointeger(refState->GetState(), -2));

                        if (!lua_isstring(refState->GetState(), -1)) {
                            throw LuaException(refState, "Value for index %d in array is not of type string", idx);
                        }

                        values.emplace_back(lua_tostring(refState->GetState(), -1));

                        // value, key popped by lua_next
                        lua_pop(refState->GetState(), 1);
                    }

                    entry = new ks::Settings::EntryStringList(subKey, values);
                }
            }
        } else {
            // value of any type
            if (lua_isinteger(refState->GetState(), -1)) {
                entry = new ks::Settings::EntryInt(subKey, lua_tointeger(refState->GetState(), -1));
            } else if (lua_isnumber(refState->GetState(), -1)) {
                entry = new ks::Settings::EntryFloat(subKey, lua_tonumber(refState->GetState(), -1));
            } else if (lua_isstring(refState->GetState(), -1)) {
                entry = new ks::Settings::EntryString(subKey, lua_tostring(refState->GetState(), -1));
            } else if (lua_isboolean(refState->GetState(), -1)) {
                entry = new ks::Settings::EntryBool(subKey, static_cast<bool>(lua_toboolean(refState->GetState(), -1)));
            }
        }

        if (entry) {
            // check if there is an existing entry
            auto it = _GetStorage(settings).find(subKey);

            if (it != _GetStorage(settings).end()) {
                // delete existing entry, i.e. overriding it with a new value
                delete it->second;

                _GetStorage(settings).erase(it);
            }

            _GetStorage(settings).insert(std::make_pair(entry->GetKey(), entry));
        }

        // value, key popped by lua_next
        lua_pop(refState->GetState(), 1);
    }
}

}