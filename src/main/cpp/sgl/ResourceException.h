#pragma once

#include "ks/Exception.h"
#include "ks/FileSystem.h"

namespace sgl {

class ResourceException : public ks::Exception
{
public:
    template<typename... Args>
    explicit ResourceException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    template<typename... Args>
    explicit ResourceException(const std::string& path, const std::string& format, Args... args) :
        Exception("[%s] " + format, path, args...)
    {}

    template<typename... Args>
    ResourceException(const ks::FileSystem* fs, const std::string& format, Args... args) :
        Exception("[%s] " + format, fs->GetRootPath(), args...)
    {}

    ~ResourceException() override = default;
};

}
