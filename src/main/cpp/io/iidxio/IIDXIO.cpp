//
// Created by on 8/5/19.
//

#include "IIDXIO.h"

extern "C" {
#include "bt/api/iidxio.h"
}

#include "io/bt/BemanitoolsGlue.h"

#include "IIDXIODefs.h"
#include "IIDXIOException.h"

#include "ks/Thread.h"

namespace io {
namespace iidxio {

IIDXIO::IIDXIO() :
    IODevice(DEVICE_IDENT),
    m_initialized(false),
    m_16SegText()
{
    iidx_io_set_loggers(bt::BemanitoolsGlue::LogFormatterMisc, bt::BemanitoolsGlue::LogFormatterInfo,
        bt::BemanitoolsGlue::LogFormatterWarning, bt::BemanitoolsGlue::LogFormatterFatal);
}

void IIDXIO::Open(const ks::Settings& settings)
{
    if (!iidx_io_init(bt::BemanitoolsGlue::ThreadCreate, bt::BemanitoolsGlue::ThreadJoin,
            bt::BemanitoolsGlue::ThreadDestroy)) {
        throw IIDXIOException("Initializing iidxio failed");
    }

    m_initialized = true;
}

void IIDXIO::Close()
{
    if (m_initialized) {
        iidx_io_fini();

        m_initialized = false;
    }
}

void IIDXIO::Update()
{
    if (m_initialized) {
        // EP order like this, firmware gets stuck otherwise

        if (!iidx_io_ep1_send()) {
            throw new IIDXIOException("Sending data to ep1 failed");
        }

        if (!iidx_io_ep2_recv()) {
            throw new IIDXIOException("Receiving data on ep2 failed");
        }

        if (!iidx_io_ep3_write_16seg(m_16SegText.c_str())) {
            throw new IIDXIOException("Sending data to ep3 failed");
        }

        // Slow down polling to avoid crashing the firmware
        ks::Thread::SleepMs(1);
    }
}

void IIDXIO::ProcessInputStates(InputState& inputStates)
{
    if (m_initialized) {
        inputStates.SetAnalog(e_AI_TurnTableP1, iidx_io_ep2_get_turntable(0) / 255.0f);
        inputStates.SetAnalog(e_AI_TurnTableP2, iidx_io_ep2_get_turntable(1) / 255.0f);

        inputStates.SetAnalog(e_AI_Slider1, iidx_io_ep2_get_slider(0) / 15.0f);
        inputStates.SetAnalog(e_AI_Slider2, iidx_io_ep2_get_slider(1) / 15.0f);
        inputStates.SetAnalog(e_AI_Slider3, iidx_io_ep2_get_slider(2) / 15.0f);
        inputStates.SetAnalog(e_AI_Slider4, iidx_io_ep2_get_slider(3) / 15.0f);
        inputStates.SetAnalog(e_AI_Slider5, iidx_io_ep2_get_slider(4) / 15.0f);

        uint8_t keysSys = iidx_io_ep2_get_sys();
        uint8_t keysPanel = iidx_io_ep2_get_panel();
        uint16_t keys14 = iidx_io_ep2_get_keys();

        inputStates.SetDigital(e_DI_P1Key1, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P1_1)));
        inputStates.SetDigital(e_DI_P1Key2, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P1_2)));
        inputStates.SetDigital(e_DI_P1Key3, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P1_3)));
        inputStates.SetDigital(e_DI_P1Key4, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P1_4)));
        inputStates.SetDigital(e_DI_P1Key5, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P1_5)));
        inputStates.SetDigital(e_DI_P1Key6, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P1_6)));
        inputStates.SetDigital(e_DI_P1Key7, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P1_7)));

        inputStates.SetDigital(e_DI_P2Key1, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P2_1)));
        inputStates.SetDigital(e_DI_P2Key2, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P2_2)));
        inputStates.SetDigital(e_DI_P2Key3, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P2_3)));
        inputStates.SetDigital(e_DI_P2Key4, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P2_4)));
        inputStates.SetDigital(e_DI_P2Key5, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P2_5)));
        inputStates.SetDigital(e_DI_P2Key6, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P2_6)));
        inputStates.SetDigital(e_DI_P2Key7, static_cast<bool>(keys14 & (1 << IIDX_IO_KEY_P2_7)));

        inputStates.SetDigital(e_DI_Key14Any, keys14 > 0);
        inputStates.SetDigital(e_DI_KeyP1Any, static_cast<bool>(keys14 & 0x7F));
        inputStates.SetDigital(e_DI_KeyP2Any, static_cast<bool>((keys14 >> 0x7F) & 0x7F));

        inputStates.SetDigital(e_DI_P1Start, static_cast<bool>(keysPanel & (1 << IIDX_IO_PANEL_P1_START)));
        inputStates.SetDigital(e_DI_P2Start, static_cast<bool>(keysPanel & (1 << IIDX_IO_PANEL_P2_START)));
        inputStates.SetDigital(e_DI_VEFX, static_cast<bool>(keysPanel & (1 << IIDX_IO_PANEL_VEFX)));
        inputStates.SetDigital(e_DI_Effector, static_cast<bool>(keysPanel & (1 << IIDX_IO_PANEL_EFFECT)));
        inputStates.SetDigital(e_DI_Test, static_cast<bool>(keysSys & (1 << IIDX_IO_SYS_TEST)));
        inputStates.SetDigital(e_DI_Service, static_cast<bool>(keysSys & (1 << IIDX_IO_SYS_SERVICE)));
    }
}

void IIDXIO::ProcessOutputStates(const OutputState& outputStates)
{
    if (m_initialized) {
        uint16_t deckLights = 0;
        uint8_t panelLights = 0;
        uint8_t topLamps = 0;
        bool neons = false;

        for (uint8_t i = e_DO_P1Key1; i <= e_DO_P2Key7; i++) {
            deckLights |= ((uint8_t) outputStates.GetDigital(i)) << i;
        }

        panelLights |= (uint8_t) outputStates.GetDigital(e_DO_P1Start);
        panelLights |= ((uint8_t) outputStates.GetDigital(e_DO_P2Start)) << 1;
        panelLights |= ((uint8_t) outputStates.GetDigital(e_DO_VEFX)) << 2;
        panelLights |= ((uint8_t) outputStates.GetDigital(e_DO_Effector)) << 3;

        // from left to right
        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloLeftRed)) << 3;
        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloLeftYellow)) << 2;
        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloLeftGreen)) << 1;
        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloLeftBlue)) << 0;

        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloRightBlue)) << 4;
        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloRightGreen)) << 5;
        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloRightYellow)) << 6;
        topLamps |= ((uint8_t) outputStates.GetDigital(e_DO_HaloRightRed)) << 7;

        neons = (uint8_t) outputStates.GetDigital(e_DO_Neons);

        // text sent in Update
        m_16SegText = outputStates.GetText(e_TO_16Seg);

        iidx_io_ep1_set_deck_lights(deckLights);
        iidx_io_ep1_set_panel_lights(panelLights);
        iidx_io_ep1_set_top_lamps(topLamps);
        iidx_io_ep1_set_top_neons(neons);
    }
}

}
}