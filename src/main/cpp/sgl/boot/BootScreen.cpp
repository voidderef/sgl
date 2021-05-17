#include "BootScreen.h"

#include "sgl/attract/AttractScreen.h"
#include "sgl/error/ErrorScreen.h"

namespace sgl {

const std::string BootScreen::MS_NAME = "boot";
const std::string BootScreen::ms_settingTitle = "screen" + ks::Settings::ms_delim + "boot" +
    ks::Settings::ms_delim + "title";
const std::string BootScreen::ms_settingMinDurationMs = "screen" + ks::Settings::ms_delim + "boot" +
    ks::Settings::ms_delim + "min_duration_ms";

BootScreen::BootScreen() :
    Screen(MS_NAME),
    m_bootPhase(e_BootPhaseRenderTitle),
    m_games(nullptr),
    m_texBootTitle(TextureManager::INVALID_HANDLE_ID),
    m_spriteBootTitle(SpriteRenderer::INVALID_HANDLE_ID),
    m_minBootDurationMs(0)
{

}

bool BootScreen::Init()
{
    auto title = _GetEngineProxy()->GetSettings().GetValue<const std::string&>(ms_settingTitle);

    if (_GetEngineProxy()->GetDataManager().GetRefFsTex()->Exists(title)) {
        m_texBootTitle = _GetEngineProxy()->GetTextureManager().CreateTextureFromFile(
            _GetEngineProxy()->GetDataManager().GetRefFsTex()->GetAbsolutePath(title));
        m_spriteBootTitle = _GetEngineProxy()->GetSpriteRenderer().CreateSprite(m_texBootTitle);

        // set sprite size to screen size to either stretch or shrink it
        _GetEngineProxy()->GetSpriteRenderer().SpriteSetSize(m_spriteBootTitle,
            _GetEngineProxy()->GetRenderer().GetResolutionX(),
            _GetEngineProxy()->GetRenderer().GetResolutionY());

        _GetEngineProxy()->GetSpriteRenderer().SpriteSetActive(m_spriteBootTitle, true);
    } else {
        KS_LOG_WARN("No boot title image found, boot screen blank");
    }

    m_minBootDurationMs = _GetEngineProxy()->GetSettings().GetValue<uint32_t>(ms_settingMinDurationMs);

    m_bootPhase = e_BootPhaseRenderTitle;

    return true;
}

void BootScreen::Cleanup()
{
    if (m_spriteBootTitle != SpriteRenderer::INVALID_HANDLE_ID) {
        _GetEngineProxy()->GetSpriteRenderer().DestroySprite(m_spriteBootTitle);
    }

    if (m_texBootTitle != TextureManager::INVALID_HANDLE_ID) {
        _GetEngineProxy()->GetTextureManager().DestroyTexture(m_texBootTitle);
    }
}

void BootScreen::Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState)
{
    if (!m_timerBootDuration.IsRunning()) {
        m_timerBootDuration.Start();
    }

    switch (m_bootPhase) {
        case e_BootPhaseRenderTitle: {
            // allow title to render before loading (which blocks the whole main/render thread)
            m_bootPhase = e_BootPhaseLoad;

            break;
        }

        case e_BootPhaseLoad: {
            KS_LOG_DEBUG("Initializing IO manager...");

            if (!_GetEngineProxy()->GetIoManager().Init(_GetEngineProxy()->GetSettings())) {
                m_errorMessage = "Initializing IO manager failed";
                KS_LOG_ERROR(m_errorMessage);
                m_bootPhase = e_BootPhaseFailed;
                break;
            }

            KS_LOG_DEBUG("Loading games...");

            try {
                m_games = new Games(&_GetEngineProxy()->GetDataManager(), &_GetEngineProxy()->GetLuaState());
            } catch (ks::Exception& e) {
                m_errorMessage = "Loading games list failed, see log";
                KS_LOG_ERROR(m_errorMessage);
                KS_LOG_ERROR(e.what());
                m_bootPhase = e_BootPhaseFailed;
                break;
            }

            try {
                for (auto& it : m_games->GetEntries()) {
                    it->LoadAttractBindings(&_GetEngineProxy()->GetLuaState());
                }
            } catch (ks::Exception& e) {
                m_errorMessage = "Loading attract lua bindings failed, see log";
                KS_LOG_ERROR(m_errorMessage);
                KS_LOG_ERROR(e.what());
                m_bootPhase = e_BootPhaseFailed;
                break;
            }

            KS_LOG_DEBUG("Finished loading attract data");

            m_bootPhase = e_BootPhaseCleanup;

            break;
        }

        case e_BootPhaseCleanup: {
            _GetEngineProxy()->GetWorldState().Add("games", m_games);

            double minBootDurExpiredMs = m_timerBootDuration.GetMs();

            if (m_minBootDurationMs - minBootDurExpiredMs > 0) {
                KS_LOG_DEBUG("Finished loading data (%f ms), waiting %f more ms before next screen",
                    minBootDurExpiredMs, m_minBootDurationMs - minBootDurExpiredMs);
            }

            m_bootPhase = e_BootPhaseWaitMinBootTime;

            break;
        }

        case e_BootPhaseWaitMinBootTime: {
            if (m_timerBootDuration.GetMs() > m_minBootDurationMs) {
                KS_LOG_DEBUG("Min boot duration expired, next screen");
                _SetNextScreen(AttractScreen::MS_NAME);
            }

            break;
        }

        case e_BootPhaseFailed: {
            auto errorCtx = new ErrorCtx(m_errorMessage);
            _GetEngineProxy()->GetWorldState().Add("errorMessage", errorCtx);

            _SetNextScreen(ErrorScreen::MS_NAME);

            break;
        }
    }
}

}