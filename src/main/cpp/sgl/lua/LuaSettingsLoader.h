#pragma once

#include "ks/SettingsLoader.h"

#include "LuaState.h"

namespace sgl {

class LuaSettingsLoader : public ks::SettingsLoader
{
public:
    explicit LuaSettingsLoader(const std::string& luaSettingsScript);

    ~LuaSettingsLoader() override = default;

    void Load(ks::Settings& settings) override;

private:
    const std::string m_luaSettingsScript;

    void __ProcessTable(ks::Settings& settings, LuaState* refState, std::string key);
};

}
