#pragma once

#include "io/IODevice.h"
#include "io/util/Usb.h"

namespace io {
namespace piubtn {

class PiuBtn : public io::IODevice
{
public:
    PiuBtn();

    ~PiuBtn() override;

    void Open(const ks::Settings& settings) override;

    void Close() override;

    void Update() override;

    void ProcessInputStates(InputState& inputStates) override;

    void ProcessOutputStates(const OutputState& outputStates) override;

private:
    struct InBuffer {
        uint8_t m_p1Select : 1;
        uint8_t m_p1Left : 1;
        uint8_t m_p1Right : 1;
        uint8_t m_p1Start : 1;

        uint8_t m_p2Select : 1;
        uint8_t m_p2Left : 1;
        uint8_t m_p2Right : 1;
        uint8_t m_p2Start : 1;

        uint8_t padding[7];
    };

    struct OutBuffer {
        uint8_t m_p1Start : 1;
        uint8_t m_p1Right : 1;
        uint8_t m_p1Left : 1;
        uint8_t m_p1Back : 1;

        uint8_t m_p2Start : 1;
        uint8_t m_p2Right : 1;
        uint8_t m_p2Left : 1;
        uint8_t m_p2Back : 1;

        uint8_t padding[7];
    };

private:
    Usb* m_usb;

    InBuffer* m_inBuffer;
    OutBuffer* m_outBuffer;
};

}
}
