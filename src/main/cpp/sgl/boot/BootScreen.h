#pragma once

#include <atomic>

#include "io/IOManager.h"

#include "ks/Thread.h"
#include "ks/Timer.h"

#include "Games.h"
#include "Screen.h"

namespace sgl {

class BootScreen : public Screen
{
public:
    static const std::string MS_NAME;

    BootScreen();

    ~BootScreen() override = default;

    bool Init() override;

    void Cleanup() override;

    void Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState) override;

private:
    enum BootPhase {
        e_BootPhaseRenderTitle = 0,
        e_BootPhaseLoad = 1,
        e_BootPhaseCleanup = 2,
        e_BootPhaseWaitMinBootTime = 3,
        e_BootPhaseFailed = 4,
    };

    BootPhase m_bootPhase;

private:
    static const std::string ms_settingTitle;
    static const std::string ms_settingMinDurationMs;

private:
    Games* m_games;

    uint16_t m_texBootTitle;
    uint16_t m_spriteBootTitle;

    uint32_t m_minBootDurationMs;
    ks::Timer m_timerBootDuration;

    std::string m_errorMessage;
};

}
