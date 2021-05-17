#pragma once

#include "Exception.h"

namespace ks {

class FileException : public Exception
{
public:
    template<typename... Args>
    explicit FileException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    template<typename... Args>
    explicit FileException(int erno, const std::string& format,
            Args... args) :
        Exception(format + " -> %s", args..., strerror(erno))
    {}

    ~FileException() override = default;
};

}
