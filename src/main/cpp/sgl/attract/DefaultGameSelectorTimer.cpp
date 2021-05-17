#include "sgl/attract/DefaultGameSelectorTimer.h"
#include "sgl/FontManager.h"
#include "sgl/SpriteRenderer.h"
#include "sgl/TextureManager.h"

namespace sgl {

const std::string DefaultGameSelectorTimer::ms_settingFont = "screen" + ks::Settings::ms_delim + "attract" +
    ks::Settings::ms_delim + "font";

DefaultGameSelectorTimer::DefaultGameSelectorTimer(uint32_t defaultGameId, uint32_t timeoutDurationSec) :
    m_font(FontManager::INVALID_HANDLE_ID),
    m_tex(TextureManager::INVALID_HANDLE_ID),
    m_sprite(SpriteRenderer::INVALID_HANDLE_ID),
    m_prevRemainingTime(-1),
    m_defaultGameId(defaultGameId),
    m_timeoutDurationSec(timeoutDurationSec),
    m_inputTriggered(false)
{

}

void DefaultGameSelectorTimer::Init(EngineProxy* refEngineProxy)
{
    if (m_timeoutDurationSec > 0) {
        m_font = refEngineProxy->GetFontManager().CreateFontFromTTFFile(
                refEngineProxy->GetDataManager().GetRefFsFont()->GetAbsolutePath(
                        refEngineProxy->GetSettings().GetValue<const std::string &>(ms_settingFont)), 20);

        __UpdateRenderTimer(refEngineProxy, true);
    }
}

void DefaultGameSelectorTimer::Update(EngineProxy* refEngineProxy)
{
    if (m_timeoutDurationSec > 0) {
        if (!m_inputTriggered && !m_timer.IsRunning()) {
            m_timer.Start();
        }

        __UpdateRenderTimer(refEngineProxy, !m_inputTriggered);
    }
}

void DefaultGameSelectorTimer::Cleanup(EngineProxy* refEngineProxy)
{
    if (m_font != FontManager::INVALID_HANDLE_ID) {
        refEngineProxy->GetFontManager().DestroyFont(m_font);
    }

    if (m_tex != TextureManager::INVALID_HANDLE_ID) {
        refEngineProxy->GetTextureManager().DestroyTexture(m_tex);
    }

    if (m_sprite != SpriteRenderer::INVALID_HANDLE_ID) {
        refEngineProxy->GetSpriteRenderer().DestroySprite(m_sprite);
    }
}

void DefaultGameSelectorTimer::InputTriggered()
{
    if (m_timeoutDurationSec > 0) {
        m_inputTriggered = true;
        m_timer.Stop();
    }
}

bool DefaultGameSelectorTimer::IsTimeout()
{
    return m_timeoutDurationSec > 0 && !m_inputTriggered && (uint32_t) m_timer.GetSec() > m_timeoutDurationSec;
}

void DefaultGameSelectorTimer::__UpdateRenderTimer(EngineProxy* refEngineProxy, bool visible)
{
    if (visible) {
        int32_t timeRemainSec = m_timeoutDurationSec - (uint32_t) m_timer.GetSec();

        if (timeRemainSec < 0) {
            timeRemainSec = 0;
        }

        // Only update when text needs to change
        if (m_prevRemainingTime != timeRemainSec) {
            m_prevRemainingTime = timeRemainSec;

            if (m_tex != TextureManager::INVALID_HANDLE_ID) {
                refEngineProxy->GetTextureManager().DestroyTexture(m_tex);
            }

            m_tex = refEngineProxy->GetTextureManager().CreateTextureFromText(
                    std::to_string(timeRemainSec), 0x00, 0xFF, 0x00, 0x00, m_font);

            if (m_sprite != SpriteRenderer::INVALID_HANDLE_ID) {
                refEngineProxy->GetSpriteRenderer().DestroySprite(m_sprite);
            }

            m_sprite = refEngineProxy->GetSpriteRenderer().CreateSprite(m_tex);

            refEngineProxy->GetSpriteRenderer().SpriteSetPos(m_sprite, 2, 2);
            refEngineProxy->GetSpriteRenderer().SpriteSetLayer(m_sprite, 0xFF);
            refEngineProxy->GetSpriteRenderer().SpriteSetActive(m_sprite, true);
        }
    } else {
        refEngineProxy->GetSpriteRenderer().SpriteSetActive(m_sprite, false);
    }
}

}