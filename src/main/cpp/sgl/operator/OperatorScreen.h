#pragma once

#include "Screen.h"

#include "Menu.h"

namespace sgl {

class OperatorScreen : public Screen
{
public:
    enum InputStateDigital {
        e_ISD_SelectNext = 0,
        e_ISD_SelectPrev = 1,
        e_ISD_Selected = 2,
        e_ISD_NoiseBack = 3,
        e_ISD_ExitApplication = 4,
    };

public:
    static const std::string MS_NAME;

    OperatorScreen();

    ~OperatorScreen() override = default;

    bool Init() override;

    void Cleanup() override;

    void Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState) override;

private:
    static const std::string ms_settingsFont;

private:
    enum State {
        e_StateMenuIdle = 0,
        e_StateNoise = 1,
        e_StateGoBack = 2,
        e_StateExitApplication = 3,
    };

    State m_state;

private:
    uint16_t m_textureNoise;
    uint16_t m_spriteNoise;

private:
    Menu* m_menu;

    ssize_t __CallbackNoise();
    ssize_t __CallbackReboot();
    ssize_t __CallbackShutdown();
};

}
