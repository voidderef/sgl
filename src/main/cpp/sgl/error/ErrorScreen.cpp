#include "ErrorScreen.h"

namespace sgl {

const std::string ErrorScreen::MS_NAME = "error";
const std::string ErrorScreen::ms_settingFont = "screen" + ks::Settings::ms_delim + "error" + ks::Settings::ms_delim +
    "font";

ErrorScreen::ErrorScreen() :
    Screen(MS_NAME),
    m_fontError(FontManager::INVALID_HANDLE_ID),
    m_texError1(TextureManager::INVALID_HANDLE_ID),
    m_texError2(TextureManager::INVALID_HANDLE_ID),
    m_spriteError1(SpriteRenderer::INVALID_HANDLE_ID),
    m_spriteError2(SpriteRenderer::INVALID_HANDLE_ID)
{

}

bool ErrorScreen::Init()
{
    m_errorCtx = _GetEngineProxy()->GetWorldState().GetAndRemove<ErrorCtx>("errorMessage");

    m_fontError = _GetEngineProxy()->GetFontManager().CreateFontFromTTFFile(
        _GetEngineProxy()->GetDataManager().GetRefFsFont()->GetAbsolutePath(
            _GetEngineProxy()->GetSettings().GetValue<const std::string&>(ms_settingFont)), 32);

    m_texError1 = _GetEngineProxy()->GetTextureManager().CreateTextureFromText(
        "Boot failed", 0xFF, 0x00, 0x00, 0x00, m_fontError);
    m_texError2 = _GetEngineProxy()->GetTextureManager().CreateTextureFromText(
        "> " + m_errorCtx->GetMessage(), 0xFF, 0x00, 0x00, 0x00, m_fontError);

    m_spriteError1 = _GetEngineProxy()->GetSpriteRenderer().CreateSprite(m_texError1);
    m_spriteError2 = _GetEngineProxy()->GetSpriteRenderer().CreateSprite(m_texError2);

    _GetEngineProxy()->GetSpriteRenderer().SpriteSetPos(m_spriteError1, 20, 20);
    _GetEngineProxy()->GetSpriteRenderer().SpriteSetActive(m_spriteError1, true);

    _GetEngineProxy()->GetSpriteRenderer().SpriteSetPos(m_spriteError2, 20, 52);
    _GetEngineProxy()->GetSpriteRenderer().SpriteSetActive(m_spriteError2, true);

    return true;
}

void ErrorScreen::Cleanup()
{
    _GetEngineProxy()->GetSpriteRenderer().DestroySprite(m_spriteError1);
    _GetEngineProxy()->GetSpriteRenderer().DestroySprite(m_spriteError2);

    _GetEngineProxy()->GetTextureManager().DestroyTexture(m_texError1);
    _GetEngineProxy()->GetTextureManager().DestroyTexture(m_texError2);

    _GetEngineProxy()->GetFontManager().DestroyFont(m_fontError);

    delete m_errorCtx;
}

void ErrorScreen::Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState)
{
    // die on this screen
}

}