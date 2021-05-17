#pragma once

#include "Exception.h"

namespace ks {

class SystemException : public Exception
{
public:
    template<typename... Args>
    explicit SystemException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    template<typename... Args>
    explicit SystemException(int erno, const std::string& format,
            Args... args) :
        Exception(format + " -> %s", args..., strerror(erno))
    {}

    ~SystemException() override = default;
};

}
