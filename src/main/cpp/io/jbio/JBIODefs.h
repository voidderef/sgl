#pragma once

namespace io {
namespace jbio {

static constexpr const char* DEVICE_IDENT = "jbio";

enum DigitalInputs
{
    e_DI_Panel1 = 0,
    e_DI_Panel2 = 1,
    e_DI_Panel3 = 2,
    e_DI_Panel4 = 3,
    e_DI_Panel5 = 4,
    e_DI_Panel6 = 5,
    e_DI_Panel7 = 6,
    e_DI_Panel8 = 7,
    e_DI_Panel9 = 8,
    e_DI_Panel10 = 9,
    e_DI_Panel11 = 10,
    e_DI_Panel12 = 11,
    e_DI_Panel13 = 12,
    e_DI_Panel14 = 13,
    e_DI_Panel15 = 14,
    e_DI_Panel16 = 15,

    e_DI_PanelAny = 16,

    e_DI_Test = 17,
    e_DI_Service = 18,
    e_DI_Coin = 19,
};

enum AnalogOutputs
{
    e_AO_FrontLedR = 0,
    e_AO_FrontLedG = 1,
    e_AO_FrontLedB = 2,

    e_AO_TopLedR = 3,
    e_AO_TopLedG = 4,
    e_AO_TopLedB = 5,

    e_AO_LeftLedR = 6,
    e_AO_LeftLedG = 7,
    e_AO_LeftLedB = 8,

    e_AO_RightLedR = 9,
    e_AO_RightLedG = 10,
    e_AO_RightLedB = 11,

    e_AO_TitleLedR = 12,
    e_AO_TitleLedG = 13,
    e_AO_TitleLedB = 14,

    e_AO_WooferLedR = 15,
    e_AO_WooferLedG = 16,
    e_AO_WooferLedB = 17,
};

}
}
