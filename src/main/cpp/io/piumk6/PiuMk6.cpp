#include "PiuMk6.h"

#include <cstring>

#include "Piumk6Defs.h"

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
namespace piumk6 {

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
    m_outBuffer->m_p1lu = static_cast<uint8_t>(m_outData->m_p1lu);
    m_outBuffer->m_p1ru = static_cast<uint8_t>(m_outData->m_p1ru);
    m_outBuffer->m_p1cn = static_cast<uint8_t>(m_outData->m_p1cn);
    m_outBuffer->m_p1ld = static_cast<uint8_t>(m_outData->m_p1ld);
    m_outBuffer->m_p1rd = static_cast<uint8_t>(m_outData->m_p1rd);

    m_outBuffer->m_p2lu = static_cast<uint8_t>(m_outData->m_p2lu);
    m_outBuffer->m_p2ru = static_cast<uint8_t>(m_outData->m_p2ru);
    m_outBuffer->m_p2cn = static_cast<uint8_t>(m_outData->m_p2cn);
    m_outBuffer->m_p2ld = static_cast<uint8_t>(m_outData->m_p2ld);
    m_outBuffer->m_p2rd = static_cast<uint8_t>(m_outData->m_p2rd);

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
        m_inData->m_p1lu.m_sens[i] = m_inBuffer->m_p1lu;
        m_inData->m_p1lu.m_panel |= m_inBuffer->m_p1lu;
        m_inData->m_p1ru.m_sens[i] = m_inBuffer->m_p1ru;
        m_inData->m_p1ru.m_panel |= m_inBuffer->m_p1ru;
        m_inData->m_p1cn.m_sens[i] = m_inBuffer->m_p1cn;
        m_inData->m_p1cn.m_panel |= m_inBuffer->m_p1cn;
        m_inData->m_p1ld.m_sens[i] = m_inBuffer->m_p1ld;
        m_inData->m_p1ld.m_panel |= m_inBuffer->m_p1ld;
        m_inData->m_p1rd.m_sens[i] = m_inBuffer->m_p1rd;
        m_inData->m_p1rd.m_panel |= m_inBuffer->m_p1rd;

        m_inData->m_p2lu.m_sens[i] = m_inBuffer->m_p2lu;
        m_inData->m_p2lu.m_panel |= m_inBuffer->m_p2lu;
        m_inData->m_p2ru.m_sens[i] = m_inBuffer->m_p2ru;
        m_inData->m_p2ru.m_panel |= m_inBuffer->m_p2ru;
        m_inData->m_p2cn.m_sens[i] = m_inBuffer->m_p2cn;
        m_inData->m_p2cn.m_panel |= m_inBuffer->m_p2cn;
        m_inData->m_p2ld.m_sens[i] = m_inBuffer->m_p2ld;
        m_inData->m_p2ld.m_panel |= m_inBuffer->m_p2ld;
        m_inData->m_p2rd.m_sens[i] = m_inBuffer->m_p2rd;
        m_inData->m_p2rd.m_panel |= m_inBuffer->m_p2rd;

        m_inData->m_test = m_inBuffer->m_test;
        m_inData->m_service = m_inBuffer->m_service;
        m_inData->m_clear = m_inBuffer->m_clear;
        m_inData->m_coin = m_inBuffer->m_coin;
    }
}

void PiuMk6Itg::ProcessInputStates(InputState& inputStates)
{
    inputStates.SetDigital(e_DI_P1_LU, m_inData->m_p1lu.m_panel);
    inputStates.SetDigital(e_DI_P1_LU_Sens0, m_inData->m_p1lu.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_LU_Sens1, m_inData->m_p1lu.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_LU_Sens2, m_inData->m_p1lu.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_LU_Sens3, m_inData->m_p1lu.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_RU, m_inData->m_p1ru.m_panel);
    inputStates.SetDigital(e_DI_P1_RU_Sens0, m_inData->m_p1ru.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_RU_Sens1, m_inData->m_p1ru.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_RU_Sens2, m_inData->m_p1ru.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_RU_Sens3, m_inData->m_p1ru.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_CN, m_inData->m_p1cn.m_panel);
    inputStates.SetDigital(e_DI_P1_CN_Sens0, m_inData->m_p1cn.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_CN_Sens1, m_inData->m_p1cn.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_CN_Sens2, m_inData->m_p1cn.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_CN_Sens3, m_inData->m_p1cn.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_LD, m_inData->m_p1ld.m_panel);
    inputStates.SetDigital(e_DI_P1_LD_Sens0, m_inData->m_p1ld.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_LD_Sens1, m_inData->m_p1ld.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_LD_Sens2, m_inData->m_p1ld.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_LD_Sens3, m_inData->m_p1ld.m_sens[3]);

    inputStates.SetDigital(e_DI_P1_RD, m_inData->m_p1rd.m_panel);
    inputStates.SetDigital(e_DI_P1_RD_Sens0, m_inData->m_p1rd.m_sens[0]);
    inputStates.SetDigital(e_DI_P1_RD_Sens1, m_inData->m_p1rd.m_sens[1]);
    inputStates.SetDigital(e_DI_P1_RD_Sens2, m_inData->m_p1rd.m_sens[2]);
    inputStates.SetDigital(e_DI_P1_RD_Sens3, m_inData->m_p1rd.m_sens[3]);


    inputStates.SetDigital(e_DI_P2_LU, m_inData->m_p2lu.m_panel);
    inputStates.SetDigital(e_DI_P2_LU_Sens0, m_inData->m_p2lu.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_LU_Sens1, m_inData->m_p2lu.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_LU_Sens2, m_inData->m_p2lu.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_LU_Sens3, m_inData->m_p2lu.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_RU, m_inData->m_p2ru.m_panel);
    inputStates.SetDigital(e_DI_P2_RU_Sens0, m_inData->m_p2ru.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_RU_Sens1, m_inData->m_p2ru.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_RU_Sens2, m_inData->m_p2ru.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_RU_Sens3, m_inData->m_p2ru.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_CN, m_inData->m_p2cn.m_panel);
    inputStates.SetDigital(e_DI_P2_CN_Sens0, m_inData->m_p2cn.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_CN_Sens1, m_inData->m_p2cn.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_CN_Sens2, m_inData->m_p2cn.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_CN_Sens3, m_inData->m_p2cn.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_LD, m_inData->m_p2ld.m_panel);
    inputStates.SetDigital(e_DI_P2_LD_Sens0, m_inData->m_p2ld.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_LD_Sens1, m_inData->m_p2ld.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_LD_Sens2, m_inData->m_p2ld.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_LD_Sens3, m_inData->m_p2ld.m_sens[3]);

    inputStates.SetDigital(e_DI_P2_RD, m_inData->m_p2rd.m_panel);
    inputStates.SetDigital(e_DI_P2_RD_Sens0, m_inData->m_p2rd.m_sens[0]);
    inputStates.SetDigital(e_DI_P2_RD_Sens1, m_inData->m_p2rd.m_sens[1]);
    inputStates.SetDigital(e_DI_P2_RD_Sens2, m_inData->m_p2rd.m_sens[2]);
    inputStates.SetDigital(e_DI_P2_RD_Sens3, m_inData->m_p2rd.m_sens[3]);

    inputStates.SetDigital(e_DI_OP_Test, m_inData->m_test);
    inputStates.SetDigital(e_DI_OP_Service, m_inData->m_service);
    inputStates.SetDigital(e_DI_OP_Clear, m_inData->m_clear);
    inputStates.SetDigital(e_DI_OP_Coin, m_inData->m_coin);
}

void PiuMk6Itg::ProcessOutputStates(const OutputState& outputStates)
{
    m_outData->m_p1lu = outputStates.GetDigital(DO_P1_LU);
    m_outData->m_p1ru = outputStates.GetDigital(DO_P1_RU);
    m_outData->m_p1cn = outputStates.GetDigital(DO_P1_CN);
    m_outData->m_p1ld = outputStates.GetDigital(DO_P1_LD);
    m_outData->m_p1rd = outputStates.GetDigital(DO_P1_RD);

    m_outData->m_p2lu = outputStates.GetDigital(DO_P2_LU);
    m_outData->m_p2ru = outputStates.GetDigital(DO_P2_RU);
    m_outData->m_p2cn = outputStates.GetDigital(DO_P2_CN);
    m_outData->m_p2ld = outputStates.GetDigital(DO_P2_LD);
    m_outData->m_p2rd = outputStates.GetDigital(DO_P2_RD);

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