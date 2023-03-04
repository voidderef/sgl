#pragma once

#include <string>

#include "ks/Object.h"

#include "sgl/attract/AttractLuaBindings.h"

#include "sgl/lua/LuaState.h"

#include "DataManager.h"
#include "ShellScript.h"

namespace sgl {

class Games : public ks::Object
{
public:
    class Entry
    {
    public:
        Entry(uint32_t id, const std::string& name, const std::string& pathStartScript,
            const std::string& pathEntryLuaScript, LuaState* refLuaStateGames, int luaRefResources, int luaRefArgs);

        ~Entry();

        uint32_t GetId() const {
            return m_id;
        }

        const std::string& GetName() const {
            return m_name;
        }

        const std::string& GetPathStartScript() const {
            return m_pathStartScript;
        }

        AttractLuaBindings& GetAttractBindings() {
            return m_attractBindings;
        }

        void LoadAttractBindings(LuaState* refLuaState);

        int GetLuaRefResources() const {
            return m_luaRefResources;
        }

        int GetLuaRefArgs() const {
            return m_luaRefArgs;
        }

        friend std::ostream& operator<<(std::ostream& os, const Entry& o)
        {
            return os << o.m_id << ", " << o.m_name << ", " << o.m_pathStartScript << ", " << o.m_attractBindings;
        }

    private:
        uint32_t m_id;
        std::string m_name;
        std::string m_pathStartScript;
        AttractLuaBindings m_attractBindings;

        LuaState* m_refLuaStateGames;
        int m_luaRefResources;
        int m_luaRefArgs;
    };

public:
    Games(DataManager* refAssetManager, LuaState* refLuaState);

    ~Games();

    void SortByName();

    void SortById();

    size_t GetNumEntries() const {
        return m_entries.size();
    }

    const std::vector<Entry*>& GetEntries() const {
        return m_entries;
    }

    friend std::ostream& operator<<(std::ostream& os, const Games& o)
    {
        for (auto& it : o.m_entries) {
            os << *it << std::endl;
        }

        return os;
    }

private:
    LuaState* m_refLuaState;
    std::vector<Entry*> m_entries;
};

}
