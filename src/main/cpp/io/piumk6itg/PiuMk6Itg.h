#pragma once

#include "io/IODevice.h"
#include "io/util/Usb.h"

namespace io {
namespace piumk6itg {

class PiuMk6Itg : public io::IODevice
{
public:
    PiuMk6Itg();

    ~PiuMk6Itg() override;

    void Open(const ks::Settings& settings) override;

    void Close() override;

    void Update() override;

    void ProcessInputStates(InputState& inputStates) override;

    void ProcessOutputStates(const OutputState& outputStates) override;

private:
    struct InBuffer {
        uint8_t m_p1Up : 1;
        uint8_t m_p1Down : 1;
        uint8_t m_p1Left : 1;
        uint8_t m_p1Right : 1;
        uint8_t m_p1CabinetStart : 1;
        uint8_t m_p1CabinetBack : 1;
        uint8_t m_p1CabinetLeft : 1;
        uint8_t m_p1CabinetRight : 1;

        uint8_t m_blank1 : 1;
        uint8_t m_test : 1;
        uint8_t m_coin : 1;
        uint8_t m_blank2 : 3;
        uint8_t m_service : 1;
        uint8_t m_clear : 1;

        uint8_t m_p2Up : 1;
        uint8_t m_p2Down : 1;
        uint8_t m_p2Left : 1;
        uint8_t m_p2Right : 1;
        uint8_t m_p2CabinetStart : 1;
        uint8_t m_p2CabinetBack : 1;
        uint8_t m_p2CabinetLeft : 1;
        uint8_t m_p2CabinetRight : 1;

        uint8_t m_blank4;

        uint8_t m_blank5;
        uint8_t m_blank6;
        uint8_t m_blank7;
        uint8_t m_blank8;
    } __attribute__((__packed__));

    struct OutBuffer {
        uint8_t m_p1SensorGroup : 2;
        uint8_t m_p1Up : 1;
        uint8_t m_p1Down : 1;
        uint8_t m_p1Left : 1;
        uint8_t m_p1Right : 1;
        uint8_t m_blank0 : 2;

        uint8_t m_blank1 : 2;
        uint8_t m_bass : 1;
        uint8_t m_coinCounter2 : 1;
        uint8_t m_coinCounter : 1;
        uint8_t m_blank2 : 3;

        uint8_t m_p2SensorGroup : 2;
        uint8_t m_p2Up : 1;
        uint8_t m_p2Down : 1;
        uint8_t m_p2Left : 1;
        uint8_t m_p2Right : 1;
        uint8_t m_blank3 : 1;
        uint8_t m_haloL1 : 1;

        uint8_t m_haloR1 : 1;
        uint8_t m_haloL2 : 1;
        uint8_t m_haloR2 : 1;
        uint8_t m_blank4 : 5;

        uint8_t m_blank5;
        uint8_t m_blank6;
        uint8_t m_blank7;
        uint8_t m_blank8;
    } __attribute__((__packed__));

    struct SensorData {
        bool m_sens[4] {};

        // merges all sensores into single state
        bool m_panel {};

        SensorData() {
            Clear();
        }

        void Clear() {
            m_sens[0] = false;
            m_sens[1] = false;
            m_sens[2] = false;
            m_sens[3] = false;
            m_panel = false;
        }
    };

    struct InData {
        SensorData m_p1Left;
        SensorData m_p1Right;
        SensorData m_p1Up;
        SensorData m_p1Down;

        bool m_p1CabinetLeft {};
        bool m_p1CabinetRight {};
        bool m_p1CabinetStart {};
        bool m_p1CabinetBack {};

        SensorData m_p2Left;
        SensorData m_p2Right;
        SensorData m_p2Up;
        SensorData m_p2Down;

        bool m_p2CabinetLeft {};
        bool m_p2CabinetRight {};
        bool m_p2CabinetStart {};
        bool m_p2CabinetBack {};

        bool m_test {};
        bool m_service {};
        bool m_clear {};
        bool m_coin {};

        InData() {
            Clear();
        }

        void Clear() {
            m_p1Left.Clear();
            m_p1Right.Clear();
            m_p1Up.Clear();
            m_p1Down.Clear();

            m_p1CabinetLeft = false;
            m_p1CabinetRight = false;
            m_p1CabinetStart = false;
            m_p1CabinetBack = false;

            m_p2Left.Clear();
            m_p2Right.Clear();
            m_p2Up.Clear();
            m_p2Down.Clear();

            m_p2CabinetLeft = false;
            m_p2CabinetRight = false;
            m_p2CabinetStart = false;
            m_p2CabinetBack = false;

            m_test = false;
            m_service = false;
            m_clear = false;
            m_coin = false;
        }
    };

    struct OutData {
        // don't include sensor groups here

        bool m_p1Left {};
        bool m_p1Right {};
        bool m_p1Up {};
        bool m_p1Down {};

        bool m_p2Left {};
        bool m_p2Right {};
        bool m_p2Up {};
        bool m_p2Down {};

        bool m_bass {};

        bool m_haloR2 {};
        bool m_haloR1 {};
        bool m_haloL2 {};
        bool m_haloL1 {};

        bool m_coinCounter {};
        bool m_coinCounter2 {};

        OutData() {
            Clear();
        }

        void Clear() {
            m_p1Left = false;
            m_p1Right = false;
            m_p1Up = false;
            m_p1Down = false;

            m_p2Left = false;
            m_p2Right = false;
            m_p2Up = false;
            m_p2Down = false;

            m_bass = false;

            m_haloR2 = false;
            m_haloR1 = false;
            m_haloL2 = false;
            m_haloL1 = false;

            m_coinCounter = false;
            m_coinCounter2 = false;
        }
    };

private:
    Usb* m_usb;

    InBuffer* m_inBuffer;
    OutBuffer* m_outBuffer;

    InData* m_inData;
    OutData* m_outData;
};

}
}
