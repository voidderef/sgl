#pragma once

#include <string>

#include "ks/ThreadLoop.h"

#include "io/IODevice.h"

namespace io {
namespace iidxio {

class IIDXIO : public io::IODevice
{
public:
    IIDXIO();

    ~IIDXIO() override = default;

    void Open(const ks::Settings& settings) override;

    void Close() override;

    void Update() override;

    void ProcessInputStates(InputState& inputStates) override;

    void ProcessOutputStates(const OutputState& outputStates) override;

private:
    bool m_initialized;

    std::string m_16SegText;
};

}
}
