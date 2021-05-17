#pragma once

#include "ks/Timer.h"

#include "io/OutputState.h"

namespace sgl {

class OutputAnimation
{
public:
    explicit OutputAnimation(uint32_t cycleTimeMs);

    ~OutputAnimation() = default;

    uint64_t GetCycleTimeMs() const {
        return m_cycleTimeMs;
    }

    void AddEventDigital(uint16_t id, uint32_t timestampMs, uint32_t durationMs);

    void Start();

    void Update(io::OutputState& state);

private:
    struct EventDigital {
        uint32_t m_timestampMs;
        uint32_t m_durationMs;
        uint16_t m_id;
    };

private:
    const uint32_t m_cycleTimeMs;

    uint64_t m_cycleCounter;
    ks::Timer m_cycleTimer;

    std::vector<EventDigital> m_digitalEvents;
};

}
