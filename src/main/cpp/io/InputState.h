#pragma once

#include <cstdint>
#include <vector>

namespace io {

class InputState
{
public:
    static const uint16_t ms_maxDigitalStates = 256;
    static const uint16_t ms_maxAnalogStates = 128;

    InputState() {
        m_digital.resize(ms_maxDigitalStates);
        m_analog.resize(ms_maxAnalogStates);
    };

    ~InputState() = default;

    void SetDigital(uint16_t id, bool val) {
        if (id >= ms_maxDigitalStates) {
            return;
        }

        m_digital[id] = val;
    }

    bool GetDigital(uint16_t id) const {
        if (id >= ms_maxDigitalStates) {
            return false;
        }

        return m_digital[id];
    }

    void SetAnalog(uint16_t id, float val) {
        if (id >= ms_maxAnalogStates) {
            return;
        }

        m_analog[id] = val;
    }

    float GetAnalog(uint16_t id) const {
        if (id >= ms_maxAnalogStates) {
            return 0.0f;
        }

        return m_analog[id];
    }

    void Clear() {
        for (auto&& i : m_digital) {
            i = false;
        }

        for (auto& it : m_analog) {
            it = 0.0f;
        }
    }

    void Merge(const InputState& other) {
        for (uint16_t i = 0; i < ms_maxDigitalStates; i++) {
            m_digital[i] = m_digital[i] || other.m_digital[i];
        }

        // TODO how to "merge" analog states?
    }

public:
    static bool IsPushed(uint8_t id, const InputState& cur, const InputState& prev) {
        return cur.GetDigital(id) && !prev.GetDigital(id);
    }

    static bool IsReleased(uint8_t id, const InputState& cur, const InputState& prev) {
        return !cur.GetDigital(id) && prev.GetDigital(id);
    }

    static bool IsHeld(uint8_t id, const InputState& cur, const InputState& prev) {
        return cur.GetDigital(id) && prev.GetDigital(id);
    }

    static bool IsIncremented(uint8_t id, float delta, const InputState& cur, const InputState& prev) {
        float prevVal = prev.GetAnalog(id);
        float curVal = cur.GetAnalog(id);

        // detect overflow
        return (prevVal - curVal > 0.5f) || (curVal - prevVal >= delta);
    }

    static bool IsDecremented(uint8_t id, float delta, const InputState& cur, const InputState& prev) {
        float prevVal = prev.GetAnalog(id);
        float curVal = cur.GetAnalog(id);

        // detect underflow
        return (curVal - prevVal > 0.5f) || (prevVal - curVal >= delta);
    }

private:
    std::vector<bool> m_digital;
    std::vector<float> m_analog;
};

}
