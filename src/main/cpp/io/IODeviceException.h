#pragma once

#include "ks/Exception.h"

#include "io/IODevice.h"

namespace io {

class IODeviceException : public ks::Exception
{
public:
    template<typename... Args>
    explicit IODeviceException(const IODevice& device, const std::string& format, Args... args) :
        Exception("[%s] " + format, device.GetIdentifier(), args...)
    {}

    ~IODeviceException() override = default;
};

}
