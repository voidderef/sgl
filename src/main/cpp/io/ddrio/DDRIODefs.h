#pragma once

namespace io {
namespace ddrio {

static constexpr const char* DEVICE_IDENT = "ddrio";

enum DigitalInputs
{
    e_DI_P1PadUp = 0,
    e_DI_P1PadDown = 1,
    e_DI_P1PadLeft = 2,
    e_DI_P1PadRight = 3,

    e_DI_P2PadUp = 4,
    e_DI_P2PadDown = 5,
    e_DI_P2PadLeft = 6,
    e_DI_P2PadRight = 7,

    e_DI_P1SelectLeft = 8,
    e_DI_P1SelectRight = 9,
    e_DI_P1Start = 10,

    e_DI_P2SelectLeft = 11,
    e_DI_P2SelectRight = 12,
    e_DI_P2Start = 13,

    e_DI_Test = 14,
    e_DI_Service = 15,
};

enum DigitalOutputs
{
    e_DO_P1PadUp = 0,
    e_DO_P1PadDown = 1,
    e_DO_P1PadLeft = 2,
    e_DO_P1PadRight = 3,

    e_DO_P2PadUp = 4,
    e_DO_P2PadDown = 5,
    e_DO_P2PadLeft = 6,
    e_DO_P2PadRight = 7,

    e_DO_P1CabinetButton = 8,
    e_DO_P2CabinetButton = 9,

    e_DO_LampTopLeft = 10,
    e_DO_LampBottomLeft = 11,
    e_DO_LampTopRight = 12,
    e_DO_LampBottomRight = 13,
    
    e_DO_BassNeons = 14,
};

}
}
