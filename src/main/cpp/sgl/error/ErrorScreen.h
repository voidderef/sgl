#pragma once

#include "ErrorCtx.h"
#include "Screen.h"

namespace sgl {

class ErrorScreen : public Screen
{
public:
    static const std::string MS_NAME;

    ErrorScreen();
    ~ErrorScreen() override = default;

    bool Init() override;

    void Cleanup() override;

    void Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState) override;

private:
    static const std::string ms_settingFont;

private:
    uint16_t m_fontError;
    uint16_t m_texError1;
    uint16_t m_texError2;
    uint16_t m_spriteError1;
    uint16_t m_spriteError2;

    ErrorCtx* m_errorCtx;
};

}
