#pragma once

#include <string>

#include "ks/Timer.h"

#include "sgl/EngineProxy.h"

namespace sgl {

class DefaultGameSelectorTimer
{
public:
    DefaultGameSelectorTimer(uint32_t defaultGameId, uint32_t timeoutDurationSec);
    ~DefaultGameSelectorTimer() = default;

    void Init(EngineProxy* refEngineProxy);

    void Update(EngineProxy* refEngineProxy);

    void Cleanup(EngineProxy* refEngineProxy);

    void InputTriggered();

    bool IsTimeout();

    uint32_t getDefaultGameId() {
        return m_defaultGameId;
    }

private:
    static const std::string ms_settingFont;

private:
    uint16_t m_font;
    uint16_t m_tex;
    uint16_t m_sprite;
    uint32_t m_prevRemainingTime;

    uint32_t m_defaultGameId;
    uint32_t m_timeoutDurationSec;
    ks::Timer m_timer;
    bool m_inputTriggered;

    void __UpdateRenderTimer(EngineProxy* refEngineProxy, bool visible);
};

}

