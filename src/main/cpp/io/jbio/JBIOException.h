#pragma once

#include "ks/Exception.h"

namespace io {
namespace jbio {

class JBIOException : public ks::Exception
{
public:
    template<typename... Args>
    explicit JBIOException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~JBIOException() override = default;
};

}
}
