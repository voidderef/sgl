#pragma once

#include "ks/Exception.h"

namespace io {
namespace iidxio {

class IIDXIOException : public ks::Exception
{
public:
    template<typename... Args>
    explicit IIDXIOException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~IIDXIOException() override = default;
};

}
}