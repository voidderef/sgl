#pragma once

namespace io {
namespace piubtn {

static constexpr const char* DEVICE_IDENT = "piubtn";

enum DigitalInputs
{
    e_DI_P1_Left = 0,
    e_DI_P1_Right = 1,
    e_DI_P1_Start = 2,
    e_DI_P1_Select = 3,

    e_DI_P2_Left = 4,
    e_DI_P2_Right = 5,
    e_DI_P2_Start = 6,
    e_DI_P2_Select = 7,
};

enum DigitalOutputs
{
    e_DO_P1_Left = 0,
    e_DO_P1_Right = 1,
    e_DO_P1_Start = 2,
    e_DO_P1_Select = 3,

    e_DO_P2_Left = 4,
    e_DO_P2_Right = 5,
    e_DO_P2_Start = 6,
    e_DO_P2_Select = 7,
};

}
}
