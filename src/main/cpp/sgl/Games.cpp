#include "Games.h"

#include "ks/Logger.hpp"

#include "lua/LuaHelper.h"

#include "ResourceException.h"

namespace sgl {

Games::Entry::Entry(uint32_t id, const std::string& name, const std::string& pathStartScript,
        const std::string& pathEntryLuaScript, LuaState* refLuaStateGames, int luaRefResources, int luaRefArgs) :
    m_id(id),
    m_name(name),
    m_shellScript(pathStartScript),
    m_attractBindings(pathEntryLuaScript),
    m_refLuaStateGames(refLuaStateGames),
    m_luaRefResources(luaRefResources),
    m_luaRefArgs(luaRefArgs)
{

}

Games::Entry::~Entry()
{
    LuaHelper::UnrefReference(m_refLuaStateGames, m_luaRefResources);
    LuaHelper::UnrefReference(m_refLuaStateGames, m_luaRefArgs);
}

void Games::Entry::LoadAttractBindings(LuaState* refLuaState)
{
    KS_LOG_DEBUG("Loading script %s of entry %d: %s", m_attractBindings, m_id, m_name);

    m_attractBindings.Load(refLuaState);
}

Games::Games(DataManager* refAssetManager, LuaState* refLuaState) :
    m_refLuaState(refLuaState)
{
    m_refLuaState->LoadScript(refAssetManager->GetGamesScriptPath(), 1);

    if (!lua_istable(m_refLuaState->GetState(), -1)) {
        throw LuaException(m_refLuaState, "Return value of games script is not a table");
    }

    // make sure lua_next starts at the beginning
    lua_pushnil(m_refLuaState->GetState());

    // our games list is a table (actually array) with tables as game entries

    // table located at -2 in stack
    while (lua_next(m_refLuaState->GetState(), -2)) {
        // index of current array element at -2
        // value at -1

        if (!lua_isinteger(m_refLuaState->GetState(), -2)) {
            throw LuaException(m_refLuaState, "Index of array is not an integer value");
        }

        // value must be a table
        if (!lua_istable(m_refLuaState->GetState(), -1)) {
            throw LuaException(m_refLuaState, "Value of array is not a table");
        }

        auto id = LuaHelper::GetValueFromTable<uint32_t>(m_refLuaState, -1, "id");
        auto name = LuaHelper::GetValueFromTable<std::string>(m_refLuaState, -1, "name");
        auto pathStartScript = LuaHelper::GetValueFromTable<std::string>(m_refLuaState, -1, "start_script");

        if (!refAssetManager->GetRefFsShell()->Exists(pathStartScript)) {
            throw ResourceException(refAssetManager->GetRefFsShell(), "Missing start_script %s", pathStartScript);
        }

        auto pathEntryLuaScript = LuaHelper::GetValueFromTable<std::string>(m_refLuaState, -1, "lua");

        if (!refAssetManager->GetRefFsLua()->Exists(pathEntryLuaScript)) {
            throw ResourceException(refAssetManager->GetRefFsLua(), "Missing lua script %s", pathEntryLuaScript);
        }

        lua_getfield(m_refLuaState->GetState(), -1, "res");

        if (lua_isnil(m_refLuaState->GetState(), -1)) {
            throw LuaException(m_refLuaState, "Missing res for entry for id %d, name %s", id, name);
        }

        if (!lua_istable(m_refLuaState->GetState(), -1)) {
            throw LuaException(m_refLuaState, "res for entry for id %d, name %s is not a table", id, name);
        }

        // create a reference to the table and avoid it getting GC'd
        int luaRefResources = LuaHelper::CreateReference(m_refLuaState);

        lua_getfield(m_refLuaState->GetState(), -1, "args");

        if (lua_isnil(m_refLuaState->GetState(), -1)) {
            throw LuaException(m_refLuaState, "Missing args for entry for id %d, name %s", id, name);
        }

        if (!lua_istable(m_refLuaState->GetState(), -1)) {
            throw LuaException(m_refLuaState, "args for entry for id %d, name %s is not a table", id, name);
        }

        // create a reference to the table and avoid it getting GC'd
        int luaRefArgs = LuaHelper::CreateReference(m_refLuaState);

        m_entries.push_back(new Games::Entry(id, name,
            refAssetManager->GetRefFsShell()->GetAbsolutePath(pathStartScript),
            refAssetManager->GetRefFsLua()->GetAbsolutePath(pathEntryLuaScript), m_refLuaState, luaRefResources,
            luaRefArgs));

        // value, key popped by lua_next
        lua_pop(m_refLuaState->GetState(), 1);
    }

    // remove game table from stack
    lua_pop(m_refLuaState->GetState(), 1);
}

Games::~Games()
{
    for (auto& it : m_entries) {
        delete it;
    }
}

void Games::SortByName()
{
    std::sort(m_entries.begin(), m_entries.end(),
        [](const Entry* a, const Entry* b) -> bool {
            return a->GetName().compare(b->GetName()) < 0;
        }
    );
}

void Games::SortById()
{
    std::sort(m_entries.begin(), m_entries.end(),
        [](const Entry* a, const Entry* b) -> bool {
            return a->GetId() < b->GetId();
        }
    );
}

}