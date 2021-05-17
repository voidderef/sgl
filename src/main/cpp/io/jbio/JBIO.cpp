//
// Created by on 8/5/19.
//

#include "JBIO.h"

extern "C" {
#include "bt/api/jbio.h"
}

#include "io/bt/BemanitoolsGlue.h"

#include "JBIODefs.h"
#include "JBIOException.h"

#include "ks/Thread.h"

namespace io {
namespace jbio {

JBIO::JBIO() :
    IODevice(DEVICE_IDENT),
    m_initialized(false)
{
    jb_io_set_loggers(bt::BemanitoolsGlue::LogFormatterMisc, bt::BemanitoolsGlue::LogFormatterInfo,
        bt::BemanitoolsGlue::LogFormatterWarning, bt::BemanitoolsGlue::LogFormatterFatal);
}

void JBIO::Open(const ks::Settings& settings)
{
    if (!jb_io_init(bt::BemanitoolsGlue::ThreadCreate, bt::BemanitoolsGlue::ThreadJoin,
            bt::BemanitoolsGlue::ThreadDestroy)) {
        throw JBIOException("Initializing jbio failed");
    }

    m_initialized = true;
}

void JBIO::Close()
{
    if (m_initialized) {
        jb_io_fini();

        m_initialized = false;
    }
}

void JBIO::Update()
{
    if (m_initialized) {
        if (!jb_io_write_outputs()) {
            throw new JBIOException("Writing outputs failed");
        }

        if (!jb_io_read_inputs()) {
            throw new JBIOException("Reading inputs failed");
        }
    }
}

void JBIO::ProcessInputStates(InputState& inputStates)
{
    if (m_initialized) {
        uint16_t panels = jb_io_get_panel_inputs();
        uint8_t sys = jb_io_get_sys_inputs();

        inputStates.SetDigital(e_DI_Panel1, static_cast<bool>(panels & (1 << JB_IO_PANEL_01)));
        inputStates.SetDigital(e_DI_Panel2, static_cast<bool>(panels & (1 << JB_IO_PANEL_02)));
        inputStates.SetDigital(e_DI_Panel3, static_cast<bool>(panels & (1 << JB_IO_PANEL_03)));
        inputStates.SetDigital(e_DI_Panel4, static_cast<bool>(panels & (1 << JB_IO_PANEL_04)));
        inputStates.SetDigital(e_DI_Panel5, static_cast<bool>(panels & (1 << JB_IO_PANEL_05)));
        inputStates.SetDigital(e_DI_Panel6, static_cast<bool>(panels & (1 << JB_IO_PANEL_06)));
        inputStates.SetDigital(e_DI_Panel7, static_cast<bool>(panels & (1 << JB_IO_PANEL_07)));
        inputStates.SetDigital(e_DI_Panel8, static_cast<bool>(panels & (1 << JB_IO_PANEL_08)));
        inputStates.SetDigital(e_DI_Panel9, static_cast<bool>(panels & (1 << JB_IO_PANEL_09)));
        inputStates.SetDigital(e_DI_Panel10, static_cast<bool>(panels & (1 << JB_IO_PANEL_10)));
        inputStates.SetDigital(e_DI_Panel11, static_cast<bool>(panels & (1 << JB_IO_PANEL_11)));
        inputStates.SetDigital(e_DI_Panel12, static_cast<bool>(panels & (1 << JB_IO_PANEL_12)));
        inputStates.SetDigital(e_DI_Panel13, static_cast<bool>(panels & (1 << JB_IO_PANEL_13)));
        inputStates.SetDigital(e_DI_Panel14, static_cast<bool>(panels & (1 << JB_IO_PANEL_14)));
        inputStates.SetDigital(e_DI_Panel15, static_cast<bool>(panels & (1 << JB_IO_PANEL_15)));
        inputStates.SetDigital(e_DI_Panel16, static_cast<bool>(panels & (1 << JB_IO_PANEL_16)));

        inputStates.SetDigital(e_DI_PanelAny, static_cast<bool>(panels > 0));

        inputStates.SetDigital(e_DI_Test, static_cast<bool>(sys & (1 << JB_IO_SYS_TEST)));
        inputStates.SetDigital(e_DI_Service, static_cast<bool>(sys & (1 << JB_IO_SYS_SERVICE)));
        inputStates.SetDigital(e_DI_Coin, static_cast<bool>(sys & (1 << JB_IO_SYS_COIN)));
    }
}

void JBIO::ProcessOutputStates(const OutputState& outputStates)
{
    if (m_initialized) {
        jb_io_set_rgb_led(JB_IO_RGB_LED_FRONT, outputStates.GetAnalogU8(e_AO_FrontLedR),
            outputStates.GetAnalogU8(e_AO_FrontLedG), outputStates.GetAnalogU8(e_AO_FrontLedB));

        jb_io_set_rgb_led(JB_IO_RGB_LED_TOP, outputStates.GetAnalogU8(e_AO_TopLedR),
            outputStates.GetAnalogU8(e_AO_TopLedG), outputStates.GetAnalogU8(e_AO_TopLedB));

        jb_io_set_rgb_led(JB_IO_RGB_LED_LEFT, outputStates.GetAnalogU8(e_AO_LeftLedR),
            outputStates.GetAnalogU8(e_AO_LeftLedG), outputStates.GetAnalogU8(e_AO_LeftLedB));

        jb_io_set_rgb_led(JB_IO_RGB_LED_RIGHT, outputStates.GetAnalogU8(e_AO_RightLedR),
            outputStates.GetAnalogU8(e_AO_RightLedG), outputStates.GetAnalogU8(e_AO_RightLedB));

        jb_io_set_rgb_led(JB_IO_RGB_LED_TITLE, outputStates.GetAnalogU8(e_AO_TitleLedR),
            outputStates.GetAnalogU8(e_AO_TitleLedG), outputStates.GetAnalogU8(e_AO_TitleLedB));

        jb_io_set_rgb_led(JB_IO_RGB_LED_WOOFER, outputStates.GetAnalogU8(e_AO_WooferLedR),
            outputStates.GetAnalogU8(e_AO_WooferLedG), outputStates.GetAnalogU8(e_AO_WooferLedB));
    }
}

}
}