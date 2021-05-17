#include "PiuBtn.h"

#include <cstring>

#include "PiuBtnDefs.h"

#define PIUBTN_VID			0x0D2F
#define PIUBTN_PID		    0x1010

#define PIUBTN_CONFIG  0x01
#define PIUBTN_IFACE   0x00

#define PIUBTN_USB_CTRL_TYPE_IN 	0xC0
#define PIUBTN_USB_CTRL_TYPE_OUT	0x40
#define PIUBTN_USB_CTRL_REQUEST	0xAE
#define PIUBTN_USB_REQ_TIMEOUT	10000

#define PIUBTN_BUFFER_SIZE 8

namespace io {
namespace piubtn {

PiuBtn::PiuBtn() :
    IODevice(DEVICE_IDENT),
    m_usb(new Usb(PIUBTN_VID, PIUBTN_PID, PIUBTN_CONFIG, PIUBTN_IFACE)),
    m_inBuffer(new InBuffer()),
    m_outBuffer(new OutBuffer())
{
    // Pull up active on inputs
    memset(m_inBuffer, 0xFF, sizeof(InBuffer));
    memset(m_outBuffer, 0x00, sizeof(OutBuffer));
}

PiuBtn::~PiuBtn()
{
    delete m_usb;
}

void PiuBtn::Open(const ks::Settings& settings)
{
    m_usb->Open();
}

void PiuBtn::Close()
{
    m_usb->Close();
}

void PiuBtn::Update()
{
    auto* inBuffer = reinterpret_cast<uint8_t *>(m_inBuffer);

    // write outputs
    m_usb->ControlTransfer(PIUBTN_USB_CTRL_TYPE_OUT, PIUBTN_USB_CTRL_REQUEST, 0, 0,
        reinterpret_cast<uint8_t *>(m_outBuffer), sizeof(m_outBuffer), PIUBTN_USB_REQ_TIMEOUT);

    // get inputs
    m_usb->ControlTransfer(PIUBTN_USB_CTRL_TYPE_IN, PIUBTN_USB_CTRL_REQUEST, 0, 0, inBuffer, sizeof(m_inBuffer),
        PIUBTN_USB_REQ_TIMEOUT);

    // invert pull ups
    for (uint8_t j = 0; j < PIUBTN_BUFFER_SIZE; j++) {
        inBuffer[j] ^= 0xFF;
    }
}

void PiuBtn::ProcessInputStates(InputState& inputStates)
{
    inputStates.SetDigital(e_DI_P1_Left, m_inBuffer->m_p1Left);
    inputStates.SetDigital(e_DI_P1_Right, m_inBuffer->m_p1Right);
    inputStates.SetDigital(e_DI_P1_Start, m_inBuffer->m_p1Start);
    inputStates.SetDigital(e_DI_P1_Select, m_inBuffer->m_p1Select);

    inputStates.SetDigital(e_DI_P2_Left, m_inBuffer->m_p2Left);
    inputStates.SetDigital(e_DI_P2_Right, m_inBuffer->m_p2Right);
    inputStates.SetDigital(e_DI_P2_Start, m_inBuffer->m_p2Start);
    inputStates.SetDigital(e_DI_P2_Select, m_inBuffer->m_p2Select);
}

void PiuBtn::ProcessOutputStates(const OutputState& outputStates)
{
    m_outBuffer->m_p1Left = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P1_Left));
    m_outBuffer->m_p1Right = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P1_Right));
    m_outBuffer->m_p1Start = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P1_Start));
    m_outBuffer->m_p1Back = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P1_Select));

    m_outBuffer->m_p2Left = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P2_Left));
    m_outBuffer->m_p2Right = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P2_Right));
    m_outBuffer->m_p2Start = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P2_Start));
    m_outBuffer->m_p2Back = static_cast<uint8_t>(outputStates.GetDigital(e_DO_P2_Select));
}

}
}