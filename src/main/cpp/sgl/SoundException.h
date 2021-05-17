#pragma once

#include "ks/Exception.h"

namespace sgl {

class SoundException : public ks::Exception
{
public:
    template<typename... Args>
    explicit SoundException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~SoundException() override = default;
};

}
