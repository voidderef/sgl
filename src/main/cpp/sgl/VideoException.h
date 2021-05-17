#pragma once

#include "ks/Exception.h"

namespace sgl {

class VideoException : public ks::Exception
{
public:
    template<typename... Args>
    explicit VideoException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    ~VideoException() override = default;
};

}
