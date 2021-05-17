#include "PiuMk6Itg.h"

#include <cstring>

#include "Piumk6ItgDefs.h"

#define PIUIO_VID			0x0547
#define PIUIO_PID		    0x1002

#define PIUIO_CONFIG  0x01
#define PIUIO_IFACE   0x00

#define PIUIO_USB_CTRL_TYPE_IN 	0xC0
#define PIUIO_USB_CTRL_TYPE_OUT	0x40
#define PIUIO_USB_CTRL_REQUEST	0xAE
#define PIUIO_USB_REQ_TIMEOUT	10000

#define PIUIO_BUFFER_SIZE 8

namespace io {
namespace piumk6itg {

PiuMk6Itg::PiuMk6Itg() :
    IODevice(DEVICE_IDENT),
    m_usb(new Usb(PIUIO_VID, PIUIO_PID, PIUIO_CONFIG, PIUIO_IFACE)),
    m_inBuffer(new InBuffer()),
    m_outBuffer(new OutBuffer()),
    m_inData(new InData()),
    m_outData(new OutData())
{
    // Pull up active on inputs
    memset(m_inBuffer, 0xFF, sizeof(InBuffer));
    memset(m_outBuffer, 0x00, sizeof(OutBuffer));

    memset(m_inData, 0x00, sizeof(InData));
    memset(m_outData, 0x00, sizeof(OutData));
}

PiuMk6Itg::~PiuMk6Itg()
{
    delete m_inBuffer;
    delete m_outBuffer;
    delete m_inData;
    delete m_outData;
    delete m_usb;
}

void PiuMk6Itg::Open(const ks::Settings& settings)
{
    m_usb->Open();
}

void PiuMk6Itg::Close()
{
    m_usb->Close();
}

void PiuMk6Itg::Update()
{
    m_inData->Clear();

    // apply output states once
    m_outBuffer->m_p1Left = static_cast<uint8_t>(m_outData->m_p1Left);
    m_outBuffer->m_p1Right = static_cast<uint8_t>(m_outData->m_p1Right);
    m_outBuffer->m_p1Up = static_cast<uint8_t>(m_outData->m_p1Up);
    m_outBuffer->m_p1Down = static_cast<uint8_t>(m_outData->m_p1Down);

    m_outBuffer->m_p2Left = static_cast<uint8_t>(m_outData->m_p2Left);
    m_outBuffer->m_p2Right = static_cast<uint8_t>(m_outData->m_p2Right);
    m_outBuffer->m_p2Up = static_cast<uint8_t>(m_outData->m_p2Up);
    m_outBuffer->m_p2Down = static_cast<uint8_t>(m_outData->m_p2Down);

    m_outBuffer->m_bass = static_cast<uint8_t>(m_outData->m_bass);
    m_outBuffer->m_haloR2 = static_cast<uint8_t>(m_outData->m_haloR2);
    m_outBuffer->m_haloR1 = static_cast<uint8_t>(m_outData->m_haloR1);
    m_outBuffer->m_haloL2 = static_cast<uint8_t>(m_outData->m_haloL2);
    m_outBuffer->m_haloL1 = static_cast<uint8_t>(m_outData->m_haloL1);

    m_outBuffer->m_coinCounter = static_cast<uint8_t>(m_outData->m_coinCounter);
    m_outBuffer->m_coinCounter2 = static_cast<uint8_t>(m_outData->m_coinCounter2);

    // cycle all four sensor groups in a single update call
    for (uint8_t i = 0; i < 4; i++) {
        auto* inBuffer = reinterpret_cast<uint8_t *>(m_inBuffer);

        m_outBuffer->m_p1SensorGroup = i;
        m_outBuffer->m_p2SensorGroup = i;

        // write outputs
        m_usb->ControlTransfer(PIUIO_USB_CTRL_TYPE_OUT, PIUIO_USB_CTRL_REQUEST, 0, 0,
            reinterpret_cast<uint8_t *>(m_outBuffer), sizeof(m_outBuffer), PIUIO_USB_REQ_TIMEOUT);

        // get inputs
        m_usb->ControlTransfer(PIUIO_USB_CTRL_TYPE_IN, PIUIO_USB_CTRL_REQUEST, 0, 0, inBuffer, sizeof(m_inBuffer),
            PIUIO_USB_REQ_TIMEOUT);

        // invert pull ups
        for (uint8_t j = 0; j < 4; j++) {
            inBuffer[j] ^= 0xFF;
        }

        // store input states
        m_inData->m_p1Left.m_sens[i] = m_inBuffer->m_p1Left;
        m_inData->m_p1Left.m_panel |= m_inBuffer->m_p1Left;
        m_inData->m_p1Right.m_sens[i] = m_inBuffer->m_p1Right;
        m_inData->m_p1Right.m_panel |= m_inBuffer->m_p1Right;
        m_inData->m_p1Up.m_sens[i] = m_inBuffer->m_p1Up;
        m_inData->m_p1Up.m_panel |= m_inBuffer->m_p1Up;
        m_inData->m_p1Down.m_sens[i] = m_inBuffer->m_p1Down;
        m_inData->m_p1Down.m_panel |= m_inBuffer->m_p1Down;

        m_inData->m_p1CabinetLeft |= m_inBuffer->m_p1CabinetLeft;
        m_inData->m_p1CabinetRight |= m_inBuffer->m_p1CabinetRight;
        m_inData->m_p1CabinetStart |= m_inBuffer->m_p1CabinetStart;
        m_inData->m_p1CabinetBack |= m_inBuffer->m_p1CabinetBack;

        m_inData->m_p2Left.m_sens[i] = m_inBuffer->m_p2Left;
        m_inData->m_p2Left.m_panel |= m_inBuffer->m_p2Left;
        m_inData->m_p2Right.m_sens[i] = m_inBuffer->m_p2Right;
        m_inData->m_p2Right.m_panel |= m_inBuffer->m_p2Right;
        m_inData->m_p2Up.m_sens[i] = m_inBuffer->m_p2Up;
        m_inData->m_p2Up.m_panel |= m_inBuffer->m_p2Up;
        m_inData->m_p2Down.m_sens[i] = m_inBuffer->m_p2Down;
        m_inData->m_p2Down.m_panel |= m_inBuffer->m_p2Down;

        m_inData->m_p2CabinetLeft |= m_inBuffer->m_p2CabinetLeft;
        m_inData->m_p2CabinetRight |= m_inBuffer->m_p2CabinetRight;
        m_inData->m_p2CabinetStart |= m_inBuffer->m_p2CabinetStart;
        m_inData->m_p2CabinetBack |= m_inBuffer->m_p2CabinetBack;

        m_inData->m_test |= m_inBuffer->m_test;
        m_inData->m_service |= m_inBuffer->m_service;
        m_inData->m_clear |= m_inBuffer->m_clear;
        m_inData->m_coin |= m_inBuffer->m_coin;
    }
}

void PiuMk6Itg::ProcessInputStates(InputState& inputStates)
{
    inputStates.SetDigital(e_DI_P1_Left, m_inData->m_p1Left.m_panel);
    inputStates.SetDigital(e_DI_P1_Left_Sens0, m_inData->m_p1Left.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_Left_Sens1, m_inData->m_p1Left.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_Left_Sens2, m_inData->m_p1Left.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_Left_Sens3, m_inData->m_p1Left.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_Right, m_inData->m_p1Right.m_panel);
    inputStates.SetDigital(e_DI_P1_Right_Sens0, m_inData->m_p1Right.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_Right_Sens1, m_inData->m_p1Right.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_Right_Sens2, m_inData->m_p1Right.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_Right_Sens3, m_inData->m_p1Right.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_Up, m_inData->m_p1Up.m_panel);
    inputStates.SetDigital(e_DI_P1_Up_Sens0, m_inData->m_p1Up.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_Up_Sens1, m_inData->m_p1Up.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_Up_Sens2, m_inData->m_p1Up.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_Up_Sens3, m_inData->m_p1Up.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_Down, m_inData->m_p1Down.m_panel);
    inputStates.SetDigital(e_DI_P1_Down_Sens0, m_inData->m_p1Down.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_Down_Sens1, m_inData->m_p1Down.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_Down_Sens2, m_inData->m_p1Down.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_Down_Sens3, m_inData->m_p1Down.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_Cabinet_Left, m_inData->m_p1CabinetLeft);
    inputStates.SetDigital(e_DI_P1_Cabinet_Right, m_inData->m_p1CabinetRight);
    inputStates.SetDigital(e_DI_P1_Cabinet_Start, m_inData->m_p1CabinetStart);
    inputStates.SetDigital(e_DI_P1_Cabinet_Back, m_inData->m_p1CabinetBack);

    inputStates.SetDigital(e_DI_P2_Left, m_inData->m_p2Left.m_panel);
    inputStates.SetDigital(e_DI_P2_Left_Sens0, m_inData->m_p2Left.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_Left_Sens1, m_inData->m_p2Left.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_Left_Sens2, m_inData->m_p2Left.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_Left_Sens3, m_inData->m_p2Left.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_Right, m_inData->m_p2Right.m_panel);
    inputStates.SetDigital(e_DI_P2_Right_Sens0, m_inData->m_p2Right.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_Right_Sens1, m_inData->m_p2Right.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_Right_Sens2, m_inData->m_p2Right.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_Right_Sens3, m_inData->m_p2Right.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_Up, m_inData->m_p2Up.m_panel);
    inputStates.SetDigital(e_DI_P2_Up_Sens0, m_inData->m_p2Up.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_Up_Sens1, m_inData->m_p2Up.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_Up_Sens2, m_inData->m_p2Up.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_Up_Sens3, m_inData->m_p2Up.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_Down, m_inData->m_p2Down.m_panel);
    inputStates.SetDigital(e_DI_P2_Down_Sens0, m_inData->m_p2Down.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_Down_Sens1, m_inData->m_p2Down.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_Down_Sens2, m_inData->m_p2Down.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_Down_Sens3, m_inData->m_p2Down.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_Cabinet_Left, m_inData->m_p2CabinetLeft);
    inputStates.SetDigital(e_DI_P2_Cabinet_Right, m_inData->m_p2CabinetRight);
    inputStates.SetDigital(e_DI_P2_Cabinet_Start, m_inData->m_p2CabinetStart);
    inputStates.SetDigital(e_DI_P2_Cabinet_Back, m_inData->m_p2CabinetBack);

    inputStates.SetDigital(e_DI_OP_Test, m_inData->m_test);
    inputStates.SetDigital(e_DI_OP_Service, m_inData->m_service);
    inputStates.SetDigital(e_DI_OP_Clear, m_inData->m_clear);
    inputStates.SetDigital(e_DI_OP_Coin, m_inData->m_coin);
}

void PiuMk6Itg::ProcessOutputStates(const OutputState& outputStates)
{
    m_outData->m_p1Left = outputStates.GetDigital(DO_P1_Left);
    m_outData->m_p1Right = outputStates.GetDigital(DO_P1_Right);
    m_outData->m_p1Up = outputStates.GetDigital(DO_P1_Up);
    m_outData->m_p1Down = outputStates.GetDigital(DO_P1_Down);

    m_outData->m_p2Left = outputStates.GetDigital(DO_P2_Left);
    m_outData->m_p2Right = outputStates.GetDigital(DO_P2_Right);
    m_outData->m_p2Up = outputStates.GetDigital(DO_P2_Up);
    m_outData->m_p2Down = outputStates.GetDigital(DO_P2_Down);

    m_outData->m_bass = outputStates.GetDigital(DO_Bass);
    m_outData->m_haloR2 = outputStates.GetDigital(DO_HaloR2);
    m_outData->m_haloR1 = outputStates.GetDigital(DO_HaloR1);
    m_outData->m_haloL2 = outputStates.GetDigital(DO_HaloL2);
    m_outData->m_haloL1 = outputStates.GetDigital(DO_HaloL1);

    m_outData->m_coinCounter = outputStates.GetDigital(DO_CoinCounter);
    m_outData->m_coinCounter2 = outputStates.GetDigital(DO_CoinCounter2);
}

}
}