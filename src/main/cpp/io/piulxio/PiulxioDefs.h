#pragma once

namespace io {
namespace piulxio {

static constexpr const char* DEVICE_IDENT = "piulxio";

enum DigitalInputs
{
    // Panels P1 (if any sensor pressed)
    e_DI_P1_LU = 0,
    e_DI_P1_RU = 5,
    e_DI_P1_CN = 10,
    e_DI_P1_LD = 15,
    e_DI_P1_RD = 20,

    // Panels P2 (if any sensor pressed)
    e_DI_P2_LU = 25,
    e_DI_P2_RU = 30,
    e_DI_P2_CN = 35,
    e_DI_P2_LD = 40,
    e_DI_P2_RD = 45,

    // Operator inputs
    e_DI_OP_Test = 50,
    e_DI_OP_Service = 51,
    e_DI_OP_Clear = 52,
    e_DI_OP_Coin = 53,

    // Single sensor states P1
    e_DI_P1_LU_Sens0 = 1,
    e_DI_P1_LU_Sens1 = 2,
    e_DI_P1_LU_Sens2 = 3,
    e_DI_P1_LU_Sens3 = 4,

    e_DI_P1_RU_Sens0 = 6,
    e_DI_P1_RU_Sens1 = 7,
    e_DI_P1_RU_Sens2 = 8,
    e_DI_P1_RU_Sens3 = 9,

    e_DI_P1_CN_Sens0 = 11,
    e_DI_P1_CN_Sens1 = 12,
    e_DI_P1_CN_Sens2 = 13,
    e_DI_P1_CN_Sens3 = 14,

    e_DI_P1_LD_Sens0 = 16,
    e_DI_P1_LD_Sens1 = 17,
    e_DI_P1_LD_Sens2 = 18,
    e_DI_P1_LD_Sens3 = 19,

    e_DI_P1_RD_Sens0 = 21,
    e_DI_P1_RD_Sens1 = 22,
    e_DI_P1_RD_Sens2 = 23,
    e_DI_P1_RD_Sens3 = 24,

    // Single sensor states P2
    e_DI_P2_LU_Sens0 = 26,
    e_DI_P2_LU_Sens1 = 27,
    e_DI_P2_LU_Sens2 = 28,
    e_DI_P2_LU_Sens3 = 29,

    e_DI_P2_RU_Sens0 = 31,
    e_DI_P2_RU_Sens1 = 32,
    e_DI_P2_RU_Sens2 = 33,
    e_DI_P2_RU_Sens3 = 34,

    e_DI_P2_CN_Sens0 = 36,
    e_DI_P2_CN_Sens1 = 37,
    e_DI_P2_CN_Sens2 = 38,
    e_DI_P2_CN_Sens3 = 39,

    e_DI_P2_LD_Sens0 = 41,
    e_DI_P2_LD_Sens1 = 42,
    e_DI_P2_LD_Sens2 = 43,
    e_DI_P2_LD_Sens3 = 44,

    e_DI_P2_RD_Sens0 = 46,
    e_DI_P2_RD_Sens1 = 47,
    e_DI_P2_RD_Sens2 = 48,
    e_DI_P2_RD_Sens3 = 49,

    //P1 Menu Buttons
    e_DI_P1_LU_Menu = 54,
    e_DI_P1_RU_Menu = 55,
    e_DI_P1_CN_Menu = 56,
    e_DI_P1_LD_Menu = 57,
    e_DI_P1_RD_Menu = 58,

    //P2 Menu Buttons
    e_DI_P2_LU_Menu = 59,
    e_DI_P2_RU_Menu = 60,
    e_DI_P2_CN_Menu = 61,
    e_DI_P2_LD_Menu = 62,
    e_DI_P2_RD_Menu = 63

};

enum DigitalOutputs
{
    DO_P1_LU = 0,
    DO_P1_RU = 1,
    DO_P1_CN = 2,
    DO_P1_LD = 3,
    DO_P1_RD = 4,

    DO_P2_LU = 5,
    DO_P2_RU = 6,
    DO_P2_CN = 7,
    DO_P2_LD = 8,
    DO_P2_RD = 9,
    

    DO_Bass = 10,
    DO_HaloR2 = 11,
    DO_HaloR1 = 12,
    DO_HaloL2 = 13,
    DO_HaloL1 = 14,

    DO_CoinCounter = 15,
    DO_CoinCounter2 = 16,

    DO_P1_LU_Menu = 17,
    DO_P1_RU_Menu = 18,
    DO_P1_CN_Menu = 19,
    DO_P1_LD_Menu = 20,
    DO_P1_RD_Menu = 21,

    DO_P2_LU_Menu = 22,
    DO_P2_RU_Menu = 23,
    DO_P2_CN_Menu = 24,
    DO_P2_LD_Menu = 25,
    DO_P2_RD_Menu = 26
};

}
}
