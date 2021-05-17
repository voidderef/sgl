#pragma once

#include "Settings.h"

namespace ks {

class SettingsLoader
{
public:
    virtual void Load(Settings& settings) = 0;

protected:
    SettingsLoader() = default;

    virtual ~SettingsLoader() = default;

    std::unordered_map<std::string, Settings::Entry*>& _GetStorage(Settings& settings) {
        return settings.m_storage;
    };
};

}
