#include "OutputAnimation.h"

#include <algorithm>

namespace sgl {

OutputAnimation::OutputAnimation(uint32_t cycleTimeMs) :
    m_cycleTimeMs(cycleTimeMs),
    m_cycleCounter(0)
{

}

void OutputAnimation::AddEventDigital(uint16_t id, uint32_t timestampMs,
        uint32_t durationMs)
{
    EventDigital step {};
    step.m_timestampMs = timestampMs;
    step.m_durationMs = durationMs;
    step.m_id = id;

    m_digitalEvents.push_back(step);

    std::sort(m_digitalEvents.begin(), m_digitalEvents.end(),
        [](const EventDigital& a, const EventDigital& b) -> bool {
            return a.m_timestampMs > b.m_timestampMs;
        });
}

void OutputAnimation::Start()
{
    m_cycleCounter = 0;
    m_cycleTimer.Start();
}

void OutputAnimation::Update(io::OutputState& state)
{
    double timeMs = ((uint32_t) m_cycleTimer.GetMs()) % m_cycleTimeMs;

    // naive method for now: iterate all events and apply them if intersecting
    // with the current timestamp
    for (auto event : m_digitalEvents) {
        if (timeMs >= event.m_timestampMs && timeMs < event.m_timestampMs + event.m_durationMs) {
            state.SetDigital(event.m_id, true);
        }
    }

    m_cycleCounter++;
}

}