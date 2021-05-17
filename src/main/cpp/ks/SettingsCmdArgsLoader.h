#pragma once

#include "SettingsLoader.h"

namespace ks {

class SettingsCmdArgsLoader : public SettingsLoader
{
public:
    SettingsCmdArgsLoader(int argc, char** argv);

    ~SettingsCmdArgsLoader() override = default;

    void Load(Settings& settings) override;

private:
    int m_argc;
    char** m_argv;

    bool __IsInteger(const std::string& str);
    bool __IsFloat(const std::string& str);
    bool __IsBool(const std::string& str);
};

}
