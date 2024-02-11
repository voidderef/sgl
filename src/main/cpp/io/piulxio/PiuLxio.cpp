#include "PiuLxio.h"
#include "ks/Logger.hpp"
#include <cstring>

#include "PiulxioDefs.h"

#define LXIO_VID        0x0D2F
#define LXIO_PID        0x1020
#define LXIO_CONFIG     0x01
#define LXIO_IFACE      0x00

#define USB_REQ_TIMEOUT         10000
#define LXIO_ENDPOINT_INPUT     0x81
#define LXIO_ENDPOINT_OUTPUT    0x02
#define LXIO_MSG_SIZE           16


namespace io {
namespace piulxio {

PiuLxio::PiuLxio() :
    IODevice(DEVICE_IDENT),
    m_usb(new Usb(LXIO_VID, LXIO_PID, LXIO_CONFIG, LXIO_IFACE)),
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

PiuLxio::~PiuLxio()
{
    delete m_inBuffer;
    delete m_outBuffer;
    delete m_inData;
    delete m_outData;
    delete m_usb;
}

void PiuLxio::Open(const ks::Settings& settings)
{
    m_usb->Open();
}

void PiuLxio::Close()
{
    m_usb->Close();
}

void PiuLxio::Update()
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

    m_outBuffer->m_p1lu_menu = static_cast<uint8_t>(m_outData->m_p1lu_menu);
    m_outBuffer->m_p1ru_menu = static_cast<uint8_t>(m_outData->m_p1ru_menu);
    m_outBuffer->m_p1cn_menu = static_cast<uint8_t>(m_outData->m_p1cn_menu);
    m_outBuffer->m_p1ld_menu = static_cast<uint8_t>(m_outData->m_p1ld_menu);
    m_outBuffer->m_p1rd_menu = static_cast<uint8_t>(m_outData->m_p1rd_menu);

    m_outBuffer->m_p2lu_menu = static_cast<uint8_t>(m_outData->m_p2lu_menu);
    m_outBuffer->m_p2ru_menu = static_cast<uint8_t>(m_outData->m_p2ru_menu);
    m_outBuffer->m_p2cn_menu = static_cast<uint8_t>(m_outData->m_p2cn_menu);
    m_outBuffer->m_p2ld_menu = static_cast<uint8_t>(m_outData->m_p2ld_menu);
    m_outBuffer->m_p2rd_menu = static_cast<uint8_t>(m_outData->m_p2rd_menu);

    m_outBuffer->m_bass = static_cast<uint8_t>(m_outData->m_bass);
    m_outBuffer->m_haloR2 = static_cast<uint8_t>(m_outData->m_haloR2);
    m_outBuffer->m_haloR1 = static_cast<uint8_t>(m_outData->m_haloR1);
    m_outBuffer->m_haloL2 = static_cast<uint8_t>(m_outData->m_haloL2);
    m_outBuffer->m_haloL1 = static_cast<uint8_t>(m_outData->m_haloL1);

    m_outBuffer->m_coinCounter = static_cast<uint8_t>(m_outData->m_coinCounter);
    m_outBuffer->m_coinCounter2 = static_cast<uint8_t>(m_outData->m_coinCounter2);

    // write outputs
    m_usb->IntTransfer(LXIO_ENDPOINT_OUTPUT, reinterpret_cast<uint8_t *>(m_outBuffer), LXIO_MSG_SIZE, USB_REQ_TIMEOUT);

    auto* inBuffer = reinterpret_cast<uint8_t *>(m_inBuffer);

    // get inputs
    m_usb->IntTransfer(LXIO_ENDPOINT_INPUT, inBuffer, LXIO_MSG_SIZE, USB_REQ_TIMEOUT);

    // invert pull ups
    for (uint8_t j = 0; j < LXIO_MSG_SIZE; j++) {
        inBuffer[j] ^= 0xFF;
    }

    // Player 1: cycle though  4 sensors
    for (uint8_t j = 0; j < 4; j++) {
        m_inData->m_p1lu.m_sens[j] =    inBuffer[j] & ( 1<<0 );
        m_inData->m_p1lu.m_panel |=     inBuffer[j] & ( 1<<0 );
        m_inData->m_p1ru.m_sens[j] =    inBuffer[j] & ( 1<<1 );
        m_inData->m_p1ru.m_panel |=     inBuffer[j] & ( 1<<1 );
        m_inData->m_p1cn.m_sens[j] =    inBuffer[j] & ( 1<<2 );
        m_inData->m_p1cn.m_panel |=     inBuffer[j] & ( 1<<2 );
        m_inData->m_p1ld.m_sens[j] =    inBuffer[j] & ( 1<<3 );
        m_inData->m_p1ld.m_panel |=     inBuffer[j] & ( 1<<3 );
        m_inData->m_p1rd.m_sens[j] =    inBuffer[j] & ( 1<<4 );
        m_inData->m_p1rd.m_panel |=     inBuffer[j] & ( 1<<4 );
    }

    // Player 2: cycle though  4 sensors
    for (uint8_t j = 0; j < 4; j++) {
        m_inData->m_p2lu.m_sens[j] =    inBuffer[4 + j] & ( 1<<0 );
        m_inData->m_p2lu.m_panel |=     inBuffer[4 + j] & ( 1<<0 );
        m_inData->m_p2ru.m_sens[j] =    inBuffer[4 + j] & ( 1<<1 );
        m_inData->m_p2ru.m_panel |=     inBuffer[4 + j] & ( 1<<1 );
        m_inData->m_p2cn.m_sens[j] =    inBuffer[4 + j] & (1<<2 );
        m_inData->m_p2cn.m_panel |=     inBuffer[4 + j] & (1<<2 );
        m_inData->m_p2ld.m_sens[j] =    inBuffer[4 + j] & (1<<3 );
        m_inData->m_p2ld.m_panel |=     inBuffer[4 + j] & ( 1<<3 );
        m_inData->m_p2rd.m_sens[j] =    inBuffer[4 + j] & ( 1<<4 );
        m_inData->m_p2rd.m_panel |=     inBuffer[4 + j] & ( 1<<4 );
    }


    // menu buttons p1
    m_inData->m_p1lu_menu = inBuffer[10] & ( 1<<0 );
    m_inData->m_p1ru_menu = inBuffer[10] & ( 1<<1 );
    m_inData->m_p1cn_menu = inBuffer[10] & ( 1<<2 );
    m_inData->m_p1ld_menu = inBuffer[10] & ( 1<<3 );
    m_inData->m_p1rd_menu = inBuffer[10] & ( 1<<4 );

    // menu buttons p2
    m_inData->m_p2lu_menu = inBuffer[11] & ( 1<<0 );
    m_inData->m_p2ru_menu = inBuffer[11] & ( 1<<1 );
    m_inData->m_p2cn_menu = inBuffer[11] & ( 1<<2 );
    m_inData->m_p2ld_menu = inBuffer[11] & ( 1<<3 );
    m_inData->m_p2rd_menu = inBuffer[11] & ( 1<<4 );


    //Operator Buttons
    m_inData->m_test =      inBuffer[8] & ( 1<<1 );
    m_inData->m_service =   inBuffer[8] & ( 1<<6 );
    m_inData->m_clear =     inBuffer[8] & ( 1<<7 );
    m_inData->m_coin =      inBuffer[8] & ( 1<<2 );

}

void PiuLxio::ProcessInputStates(InputState& inputStates)
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

    inputStates.SetDigital(e_DI_P1_LU_Menu, m_inData->m_p1lu_menu);
    inputStates.SetDigital(e_DI_P1_RU_Menu, m_inData->m_p1ru_menu);
    inputStates.SetDigital(e_DI_P1_CN_Menu, m_inData->m_p1cn_menu);
    inputStates.SetDigital(e_DI_P1_LD_Menu, m_inData->m_p1ld_menu);
    inputStates.SetDigital(e_DI_P1_RD_Menu, m_inData->m_p1rd_menu);

    inputStates.SetDigital(e_DI_P2_LU_Menu, m_inData->m_p2lu_menu);
    inputStates.SetDigital(e_DI_P2_RU_Menu, m_inData->m_p2ru_menu);
    inputStates.SetDigital(e_DI_P2_CN_Menu, m_inData->m_p2cn_menu);
    inputStates.SetDigital(e_DI_P2_LD_Menu, m_inData->m_p2ld_menu);
    inputStates.SetDigital(e_DI_P2_RD_Menu, m_inData->m_p2rd_menu);

    inputStates.SetDigital(e_DI_OP_Test, m_inData->m_test);
    inputStates.SetDigital(e_DI_OP_Service, m_inData->m_service);
    inputStates.SetDigital(e_DI_OP_Clear, m_inData->m_clear);
    inputStates.SetDigital(e_DI_OP_Coin, m_inData->m_coin);
}

void PiuLxio::ProcessOutputStates(const OutputState& outputStates)
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

    m_outData->m_p1lu_menu = outputStates.GetDigital(DO_P1_LU_Menu);
    m_outData->m_p1ru_menu = outputStates.GetDigital(DO_P1_RU_Menu);
    m_outData->m_p1cn_menu = outputStates.GetDigital(DO_P1_CN_Menu);
    m_outData->m_p1ld_menu = outputStates.GetDigital(DO_P1_LD_Menu);
    m_outData->m_p1rd_menu = outputStates.GetDigital(DO_P1_RD_Menu);

    m_outData->m_p2lu_menu = outputStates.GetDigital(DO_P2_LU_Menu);
    m_outData->m_p2ru_menu = outputStates.GetDigital(DO_P2_RU_Menu);
    m_outData->m_p2cn_menu = outputStates.GetDigital(DO_P2_CN_Menu);
    m_outData->m_p2ld_menu = outputStates.GetDigital(DO_P2_LD_Menu);
    m_outData->m_p2rd_menu = outputStates.GetDigital(DO_P2_RD_Menu);

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