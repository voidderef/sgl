#pragma once

#include "ks/Timer.h"

#include "io/OutputState.h"

namespace sgl {

class TextOutputAnimation
{
public:
    explicit TextOutputAnimation(size_t displaySize, uint32_t cycleTimeMs);

    void SetText(const std::string& text);

    void Start();

    void Update(io::OutputState& state, uint16_t id);

private:
    const size_t m_displaySize;
    const uint32_t m_cycleTimeMs;

    ks::Timer m_cycleTimer;

    std::string m_text;
    size_t m_pos;

    std::string m_displayText;
};

}
