#pragma once

#include "io/IODevice.h"
#include "io/util/Usb.h"

namespace io {
namespace piumk6 {

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
        uint8_t m_p1lu : 1;
        uint8_t m_p1ru : 1;
        uint8_t m_p1cn : 1;
        uint8_t m_p1ld : 1;
        uint8_t m_p1rd : 1;
        uint8_t m_blank0 : 3;

        uint8_t m_blank1 : 1;
        uint8_t m_test : 1;
        uint8_t m_coin : 1;
        uint8_t m_blank2 : 3;
        uint8_t m_service : 1;
        uint8_t m_clear : 1;

        uint8_t m_p2lu : 1;
        uint8_t m_p2ru : 1;
        uint8_t m_p2cn : 1;
        uint8_t m_p2ld : 1;
        uint8_t m_p2rd : 1;
        uint8_t m_blank3 : 3;

        uint8_t m_blank4;

        uint8_t m_blank5;
        uint8_t m_blank6;
        uint8_t m_blank7;
        uint8_t m_blank8;
    } __attribute__((__packed__));

    struct OutBuffer {
        uint8_t m_p1SensorGroup : 2;
        uint8_t m_p1lu : 1;
        uint8_t m_p1ru : 1;
        uint8_t m_p1cn : 1;
        uint8_t m_p1ld : 1;
        uint8_t m_p1rd : 1;
        uint8_t m_blank0 : 1;

        uint8_t m_blank1 : 2;
        uint8_t m_bass : 1;
        uint8_t m_coinCounter2 : 1;
        uint8_t m_coinCounter : 1;
        uint8_t m_blank2 : 3;

        uint8_t m_p2SensorGroup : 2;
        uint8_t m_p2lu : 1;
        uint8_t m_p2ru : 1;
        uint8_t m_p2cn : 1;
        uint8_t m_p2ld : 1;
        uint8_t m_p2rd : 1;
        uint8_t m_haloR2 : 1;

        uint8_t m_haloR1 : 1;
        uint8_t m_haloL2 : 1;
        uint8_t m_haloL1 : 1;
        uint8_t m_blank3 : 5;

        uint8_t m_blank4;
        uint8_t m_blank5;
        uint8_t m_blank6;
        uint8_t m_blank7;
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
        SensorData m_p1lu;
        SensorData m_p1ru;
        SensorData m_p1cn;
        SensorData m_p1ld;
        SensorData m_p1rd;

        SensorData m_p2lu;
        SensorData m_p2ru;
        SensorData m_p2cn;
        SensorData m_p2ld;
        SensorData m_p2rd;

        bool m_test {};
        bool m_service {};
        bool m_clear {};
        bool m_coin {};

        InData() {
            Clear();
        }

        void Clear() {
            m_p1lu.Clear();
            m_p1ru.Clear();
            m_p1cn.Clear();
            m_p1ld.Clear();
            m_p1rd.Clear();

            m_p2lu.Clear();
            m_p2ru.Clear();
            m_p2cn.Clear();
            m_p2ld.Clear();
            m_p2rd.Clear();

            m_test = false;
            m_service = false;
            m_clear = false;
            m_coin = false;
        }
    };

    struct OutData {
        // don't include sensor groups here

        bool m_p1lu {};
        bool m_p1ru {};
        bool m_p1cn {};
        bool m_p1ld {};
        bool m_p1rd {};

        bool m_p2lu {};
        bool m_p2ru {};
        bool m_p2cn {};
        bool m_p2ld {};
        bool m_p2rd {};

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
            m_p1lu = false;
            m_p1ru = false;
            m_p1cn = false;
            m_p1ld = false;
            m_p1rd = false;

            m_p2lu = false;
            m_p2ru = false;
            m_p2cn = false;
            m_p2ld = false;
            m_p2rd = false;

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
