#pragma once

#include "ks/Exception.h"

namespace io {

class IOException : public ks::Exception
{
public:
    template<typename... Args>
    explicit IOException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~IOException() override = default;
};

}