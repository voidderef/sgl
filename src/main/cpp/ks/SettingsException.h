#pragma once

#include "Exception.h"

namespace ks {

class SettingsException : public ks::Exception
{
public:
    template<typename... Args>
    explicit SettingsException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~SettingsException() override = default;
};

}
