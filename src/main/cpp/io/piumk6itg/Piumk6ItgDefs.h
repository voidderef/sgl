#pragma once

namespace io {
namespace piumk6itg {

static constexpr const char* DEVICE_IDENT = "piumk6itg";

enum DigitalInputs
{
    // Panels P1 (if any sensor pressed)
    e_DI_P1_Left = 0,
    e_DI_P1_Right = 5,
    e_DI_P1_Up = 10,
    e_DI_P1_Down = 15,

    e_DI_P1_Cabinet_Left = 20,
    e_DI_P1_Cabinet_Right = 21,
    e_DI_P1_Cabinet_Start = 22,
    e_DI_P1_Cabinet_Back = 23,

    // Panels P2 (if any sensor pressed)
    e_DI_P2_Left = 25,
    e_DI_P2_Right = 30,
    e_DI_P2_Up = 35,
    e_DI_P2_Down = 40,

    e_DI_P2_Cabinet_Left = 45,
    e_DI_P2_Cabinet_Right = 46,
    e_DI_P2_Cabinet_Start = 47,
    e_DI_P2_Cabinet_Back = 48,

    // Operator inputs
    e_DI_OP_Test = 50,
    e_DI_OP_Service = 51,
    e_DI_OP_Clear = 52,
    e_DI_OP_Coin = 53,

    // Single sensor states P1
    e_DI_P1_Left_Sens0 = 1,
    e_DI_P1_Left_Sens1 = 2,
    e_DI_P1_Left_Sens2 = 3,
    e_DI_P1_Left_Sens3 = 4,

    e_DI_P1_Right_Sens0 = 6,
    e_DI_P1_Right_Sens1 = 7,
    e_DI_P1_Right_Sens2 = 8,
    e_DI_P1_Right_Sens3 = 9,

    e_DI_P1_Up_Sens0 = 11,
    e_DI_P1_Up_Sens1 = 12,
    e_DI_P1_Up_Sens2 = 13,
    e_DI_P1_Up_Sens3 = 14,

    e_DI_P1_Down_Sens0 = 16,
    e_DI_P1_Down_Sens1 = 17,
    e_DI_P1_Down_Sens2 = 18,
    e_DI_P1_Down_Sens3 = 19,

    // Single sensor states P2
    e_DI_P2_Left_Sens0 = 26,
    e_DI_P2_Left_Sens1 = 27,
    e_DI_P2_Left_Sens2 = 28,
    e_DI_P2_Left_Sens3 = 29,

    e_DI_P2_Right_Sens0 = 31,
    e_DI_P2_Right_Sens1 = 32,
    e_DI_P2_Right_Sens2 = 33,
    e_DI_P2_Right_Sens3 = 34,

    e_DI_P2_Up_Sens0 = 36,
    e_DI_P2_Up_Sens1 = 37,
    e_DI_P2_Up_Sens2 = 38,
    e_DI_P2_Up_Sens3 = 39,

    e_DI_P2_Down_Sens0 = 41,
    e_DI_P2_Down_Sens1 = 42,
    e_DI_P2_Down_Sens2 = 43,
    e_DI_P2_Down_Sens3 = 44,
};

enum DigitalOutputs
{
    DO_P1_Left = 0,
    DO_P1_Right = 1,
    DO_P1_Up = 2,
    DO_P1_Down = 3,

    DO_P2_Left = 4,
    DO_P2_Right = 5,
    DO_P2_Up = 6,
    DO_P2_Down = 7,

    DO_Bass = 8,
    DO_HaloR2 = 9,
    DO_HaloR1 = 10,
    DO_HaloL2 = 11,
    DO_HaloL1 = 12,

    DO_CoinCounter = 13,
    DO_CoinCounter2 = 14
};

}
}
