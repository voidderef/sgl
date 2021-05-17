#pragma once

#include "Exception.h"

namespace ks {

class FileSystemException : public Exception
{
public:
    template<typename... Args>
    explicit FileSystemException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    template<typename... Args>
    explicit FileSystemException(int erno, const std::string& format, Args... args) :
        Exception(format + " -> %s", args..., strerror(erno))
    {}

    ~FileSystemException() override = default;
};

}
