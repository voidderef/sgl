#include "TextOutputAnimation.h"

#include "ks/StringUtils.h"

namespace sgl {

TextOutputAnimation::TextOutputAnimation(size_t displaySize,
        uint32_t cycleTimeMs) :
    m_displaySize(displaySize),
    m_cycleTimeMs(cycleTimeMs)
{
    m_displayText.resize(m_displaySize);
}

void TextOutputAnimation::SetText(const std::string& text)
{
    m_text = text;
    ks::StringUtils::ToUpper(m_text);
}

void TextOutputAnimation::Start()
{
    m_pos = 0;
    m_cycleTimer.Start();
}

void TextOutputAnimation::Update(io::OutputState& state, uint16_t id)
{
    if (m_cycleTimer.GetMs() >= m_cycleTimeMs) {
        m_cycleTimer.Start();

        m_pos = (m_pos + 1) % m_text.size();
    }

    for (size_t i = 0; i < m_displaySize; i++) {
        m_displayText[i] = m_text[(i + m_pos) % m_text.size()];
    }

    state.SetText(id, m_displayText);
}

}