#pragma once

#include "ks/Exception.h"

namespace sgl {

class FontException : public ks::Exception
{
public:
    template<typename... Args>
    explicit FontException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~FontException() override = default;
};

}
