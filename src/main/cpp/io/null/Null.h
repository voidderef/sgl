#pragma once

#include "io/IODevice.h"

#include "NullDefs.h"

namespace io {
namespace null {

class Null : public io::IODevice
{
public:
    Null() :
        IODevice(DEVICE_IDENT)
    {};

    ~Null() override = default;

    void Open(const ks::Settings& settings) override {

    }

    void Close() override {

    }

    void Update() override {

    }
};

}
}
