#pragma once

#include "ks/Exception.h"

namespace sgl {

class TextureException : public ks::Exception
{
public:
    template<typename... Args>
    explicit TextureException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~TextureException() override = default;
};

}
