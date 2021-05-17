#pragma once

#include "Settings.h"

namespace ks {

class SettingsSaver
{
public:
    virtual void Save(const Settings& settings) = 0;

protected:
    SettingsSaver() = default;

    virtual ~SettingsSaver() = default;

    std::unordered_map<std::string, Settings::Entry*>& _GetStorage(Settings& settings) {
        return settings.m_storage;
    };
};

}
