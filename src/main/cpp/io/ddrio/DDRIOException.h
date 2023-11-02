#pragma once

#include "ks/Exception.h"

namespace io {
namespace ddrio {

class DDRIOException : public ks::Exception
{
public:
    template<typename... Args>
    explicit DDRIOException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~DDRIOException() override = default;
};

}
}