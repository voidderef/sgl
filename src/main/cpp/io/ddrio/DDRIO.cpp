#include "DDRIO.h"

extern "C" {
#include "bt/api/ddrio.h"
}

#include "io/bt/BemanitoolsGlue.h"

#include "DDRIODefs.h"
#include "DDRIOException.h"

#include "ks/Thread.h"

namespace io {
namespace ddrio {

DDRIO::DDRIO() :
    IODevice(DEVICE_IDENT),
    m_initialized(false),
    m_padBuffer(0),
    m_extioLights(0),
    m_p3ioLights(0)
{
    ddr_io_set_loggers(bt::BemanitoolsGlue::LogFormatterMisc, bt::BemanitoolsGlue::LogFormatterInfo,
        bt::BemanitoolsGlue::LogFormatterWarning, bt::BemanitoolsGlue::LogFormatterFatal);
}

void DDRIO::Open(const ks::Settings& settings)
{
    if (!ddr_io_init(bt::BemanitoolsGlue::ThreadCreate, bt::BemanitoolsGlue::ThreadJoin,
            bt::BemanitoolsGlue::ThreadDestroy)) {
        throw DDRIOException("Initializing ddrio failed");
    }

    m_initialized = true;
}

void DDRIO::Close()
{
    if (m_initialized) {
        ddr_io_fini();

        m_initialized = false;
    }
}

void DDRIO::Update()
{
    if (m_initialized) {
        ddr_io_set_lights_extio(m_extioLights);
        ddr_io_set_lights_p3io(m_p3ioLights);

        m_padBuffer = ddr_io_read_pad();
    }
}

void DDRIO::ProcessInputStates(InputState& inputStates)
{
    if (m_initialized) {
        inputStates.SetDigital(e_DI_P1PadUp, static_cast<bool>(m_padBuffer & (1 << DDR_P1_UP)));
        inputStates.SetDigital(e_DI_P1PadDown, static_cast<bool>(m_padBuffer & (1 << DDR_P1_DOWN)));
        inputStates.SetDigital(e_DI_P1PadLeft, static_cast<bool>(m_padBuffer & (1 << DDR_P1_LEFT)));
        inputStates.SetDigital(e_DI_P1PadRight, static_cast<bool>(m_padBuffer & (1 << DDR_P1_RIGHT)));
     
        inputStates.SetDigital(e_DI_P2PadUp, static_cast<bool>(m_padBuffer & (1 << DDR_P2_UP)));
        inputStates.SetDigital(e_DI_P2PadDown, static_cast<bool>(m_padBuffer & (1 << DDR_P2_DOWN)));
        inputStates.SetDigital(e_DI_P2PadLeft, static_cast<bool>(m_padBuffer & (1 << DDR_P2_LEFT)));
        inputStates.SetDigital(e_DI_P2PadRight, static_cast<bool>(m_padBuffer & (1 << DDR_P2_RIGHT)));
      
        inputStates.SetDigital(e_DI_P1SelectLeft, static_cast<bool>(m_padBuffer & (1 << DDR_P1_MENU_LEFT)));
        inputStates.SetDigital(e_DI_P1SelectRight, static_cast<bool>(m_padBuffer & (1 << DDR_P1_MENU_RIGHT)));
        inputStates.SetDigital(e_DI_P1Start, static_cast<bool>(m_padBuffer & (1 << DDR_P1_START)));
        
        inputStates.SetDigital(e_DI_P2SelectLeft, static_cast<bool>(m_padBuffer & (1 << DDR_P2_MENU_LEFT)));
        inputStates.SetDigital(e_DI_P2SelectRight, static_cast<bool>(m_padBuffer & (1 << DDR_P2_MENU_RIGHT)));
        inputStates.SetDigital(e_DI_P2Start, static_cast<bool>(m_padBuffer & (1 << DDR_P2_START)));

        inputStates.SetDigital(e_DI_Test, static_cast<bool>(m_padBuffer & (1 << DDR_TEST)));
        inputStates.SetDigital(e_DI_Service, static_cast<bool>(m_padBuffer & (1 << DDR_SERVICE)));
    }
}

void DDRIO::ProcessOutputStates(const OutputState& outputStates)
{
    if (m_initialized) {
        uint32_t extioLights;
        uint32_t p3ioLights;

        extioLights = 0;
        p3ioLights = 0;

        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P1PadUp)) << LIGHT_P1_UP;
        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P1PadDown)) << LIGHT_P1_DOWN;
        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P1PadLeft)) << LIGHT_P1_LEFT;
        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P1PadRight)) << LIGHT_P1_RIGHT;

        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P2PadUp)) << LIGHT_P2_UP;
        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P2PadDown)) << LIGHT_P2_DOWN;
        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P2PadLeft)) << LIGHT_P2_LEFT;
        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P2PadRight)) << LIGHT_P2_RIGHT;

        p3ioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P1CabinetButton)) << LIGHT_P1_MENU;
        p3ioLights |= ((uint8_t) outputStates.GetDigital(e_DO_P2CabinetButton)) << LIGHT_P2_MENU;

        p3ioLights |= ((uint8_t) outputStates.GetDigital(e_DO_LampTopLeft)) << LIGHT_P1_UPPER_LAMP;
        p3ioLights |= ((uint8_t) outputStates.GetDigital(e_DO_LampBottomLeft)) << LIGHT_P1_LOWER_LAMP;
        p3ioLights |= ((uint8_t) outputStates.GetDigital(e_DO_LampTopRight)) << LIGHT_P2_UPPER_LAMP;
        p3ioLights |= ((uint8_t) outputStates.GetDigital(e_DO_LampBottomRight)) << LIGHT_P2_LOWER_LAMP;

        extioLights |= ((uint8_t) outputStates.GetDigital(e_DO_BassNeons)) << LIGHT_NEONS;

        m_extioLights = extioLights;
        m_p3ioLights = p3ioLights;
    }
}

}
}