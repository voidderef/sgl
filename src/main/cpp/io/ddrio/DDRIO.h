#pragma once

#include <string>

#include "ks/ThreadLoop.h"

#include "io/IODevice.h"

namespace io {
namespace ddrio {

class DDRIO : public io::IODevice
{
public:
    DDRIO();

    ~DDRIO() override = default;

    void Open(const ks::Settings& settings) override;

    void Close() override;

    void Update() override;

    void ProcessInputStates(InputState& inputStates) override;

    void ProcessOutputStates(const OutputState& outputStates) override;

private:
    bool m_initialized;
    uint32_t m_padBuffer;
    uint32_t m_extioLights;
    uint32_t m_p3ioLights;
};

}
}
