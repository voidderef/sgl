#pragma once

namespace io {
namespace iidxio {

static constexpr const char* DEVICE_IDENT = "iidxio";

enum AnalogInputs
{
    e_AI_TurnTableP1 = 0,
    e_AI_TurnTableP2 = 1,
    e_AI_Slider1 = 2,
    e_AI_Slider2 = 3,
    e_AI_Slider3 = 4,
    e_AI_Slider4 = 5,
    e_AI_Slider5 = 6,
};

enum DigitalInputs
{
    e_DI_P1Key1 = 0,
    e_DI_P1Key2 = 1,
    e_DI_P1Key3 = 2,
    e_DI_P1Key4 = 3,
    e_DI_P1Key5 = 4,
    e_DI_P1Key6 = 5,
    e_DI_P1Key7 = 6,

    e_DI_P2Key1 = 7,
    e_DI_P2Key2 = 8,
    e_DI_P2Key3 = 9,
    e_DI_P2Key4 = 10,
    e_DI_P2Key5 = 11,
    e_DI_P2Key6 = 12,
    e_DI_P2Key7 = 13,

    e_DI_Key14Any = 14,
    e_DI_KeyP1Any = 15,
    e_DI_KeyP2Any = 16,

    e_DI_P1Start = 17,
    e_DI_P2Start = 18,
    e_DI_VEFX = 19,
    e_DI_Effector = 20,
    e_DI_Test = 21,
    e_DI_Service = 22,
};

enum DigitalOutputs
{
    e_DO_P1Key1 = 0,
    e_DO_P1Key2 = 1,
    e_DO_P1Key3 = 2,
    e_DO_P1Key4 = 3,
    e_DO_P1Key5 = 4,
    e_DO_P1Key6 = 5,
    e_DO_P1Key7 = 6,

    e_DO_P2Key1 = 7,
    e_DO_P2Key2 = 8,
    e_DO_P2Key3 = 9,
    e_DO_P2Key4 = 10,
    e_DO_P2Key5 = 11,
    e_DO_P2Key6 = 12,
    e_DO_P2Key7 = 13,

    e_DO_P1Start = 14,
    e_DO_P2Start = 15,
    e_DO_VEFX = 16,
    e_DO_Effector = 17,

    e_DO_HaloLeftRed = 18,
    e_DO_HaloLeftYellow = 19,
    e_DO_HaloLeftGreen = 20,
    e_DO_HaloLeftBlue = 21,
    e_DO_HaloRightBlue = 22,
    e_DO_HaloRightGreen = 23,
    e_DO_HaloRightYellow = 24,
    e_DO_HaloRightRed = 25,

    e_DO_Neons = 26,
};

enum TextOutput
{
    e_TO_16Seg = 0
};

}
}
