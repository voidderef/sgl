#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace io {

class OutputState
{
public:
    static const uint16_t ms_maxDigitalStates = 256;
    static const uint16_t ms_maxAnalogFloatStates = 128;
    static const uint16_t ms_maxAnalogU8States = 128;
    static const uint16_t ms_maxTextStates = 16;

    OutputState() {
        m_digital.resize(ms_maxDigitalStates);
        m_analogFloat.resize(ms_maxAnalogFloatStates);
        m_analogU8.resize(ms_maxAnalogU8States);
        m_text.resize(ms_maxTextStates);
    };

    ~OutputState() = default;

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

    void SetAnalogFloat(uint16_t id, float val) {
        if (id >= ms_maxAnalogFloatStates) {
            return;
        }

        m_analogFloat[id] = val;
    }

    float GetAnalogFloat(uint16_t id) const {
        if (id >= ms_maxAnalogFloatStates) {
            return 0.0f;
        }

        return m_analogFloat[id];
    }

    void SetAnalogU8(uint16_t id, uint8_t val) {
        if (id >= ms_maxAnalogU8States) {
            return;
        }

        m_analogU8[id] = val;
    }

    uint8_t GetAnalogU8(uint16_t id) const {
        if (id >= ms_maxAnalogU8States) {
            return 0;
        }

        return m_analogU8[id];
    }

    void SetText(uint16_t id, const std::string& text) {
        if (id >= ms_maxTextStates) {
            return;
        }

        m_text[id] = text;
    }

    const std::string& GetText(uint16_t id) const {
        if (id >= ms_maxTextStates) {
            static const std::string emptyString;

            return emptyString;
        }

        return m_text[id];
    }

    void Clear() {
        for (auto&& i : m_digital) {
            i = false;
        }

        for (auto& it : m_analogFloat) {
            it = 0.0f;
        }

        for (auto& it : m_analogU8) {
            it = 0;
        }

        for (auto& it : m_text) {
            it = "";
        }
    }

private:
    std::vector<bool> m_digital;
    std::vector<float> m_analogFloat;
    std::vector<uint8_t> m_analogU8;
    std::vector<std::string> m_text;
};

}
