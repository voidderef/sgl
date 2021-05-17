#include "OperatorScreen.h"

#include "ks/System.h"

#include "sgl/attract/AttractScreen.h"

#include "MenuPageText.h"
#include "MenuItemText.h"

namespace sgl {

const std::string OperatorScreen::MS_NAME = "operator";
const std::string OperatorScreen::ms_settingsFont = "screen" + ks::Settings::ms_delim + "operator" +
    ks::Settings::ms_delim + "font";

OperatorScreen::OperatorScreen() :
    Screen(MS_NAME),
    m_state(e_StateMenuIdle),
    m_textureNoise(TextureManager::INVALID_HANDLE_ID),
    m_spriteNoise(SpriteRenderer::INVALID_HANDLE_ID),
    m_menu(nullptr)
{

}

bool OperatorScreen::Init()
{
    auto fontName = _GetEngineProxy()->GetSettings().GetValue<const std::string&>(ms_settingsFont);
    const uint16_t fontSize = 32;

    auto systemRebootSubmenu = new MenuPageText(_GetEngineProxy(), fontName, fontSize, "System reboot?", {}, {
        new MenuItemText(_GetEngineProxy(), fontName, fontSize, "No", MenuItem::DefaultCallbacks::MenuPageBack),
        new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Yes",
            std::bind(&OperatorScreen::__CallbackReboot, this))
    });

    auto systemShutdownSubmenu = new MenuPageText(_GetEngineProxy(), fontName, fontSize, "System shutdown?", {}, {
        new MenuItemText(_GetEngineProxy(), fontName, fontSize, "No", MenuItem::DefaultCallbacks::MenuPageBack),
        new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Yes",
            std::bind(&OperatorScreen::__CallbackShutdown, this))
    });

    auto mainMenu = new MenuPageText(_GetEngineProxy(), fontName, fontSize, "Operator Menu",
        {systemRebootSubmenu, systemShutdownSubmenu}, {
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "I/O Test", MenuItem::DefaultCallbacks::Dummy),
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Reduce Screen Burn In",
                std::bind(&OperatorScreen::__CallbackNoise, this)),
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Loader Information",
                MenuItem::DefaultCallbacks::Dummy),
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "System Information",
                MenuItem::DefaultCallbacks::Dummy),
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Update", MenuItem::DefaultCallbacks::Dummy),
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Reboot",
                MenuItem::DefaultCallbacks::MenuPageChild<1>),
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Shutdown",
                MenuItem::DefaultCallbacks::MenuPageChild<2>),
            new MenuItemText(_GetEngineProxy(), fontName, fontSize, "Back", MenuItem::DefaultCallbacks::MenuPageBack)
    });

    m_menu = new Menu("Operator", mainMenu);

    m_textureNoise = _GetEngineProxy()->GetTextureManager().CreateStreamingTexture(
        _GetEngineProxy()->GetRenderer().GetResolutionX(), _GetEngineProxy()->GetRenderer().GetResolutionY());
    m_spriteNoise = _GetEngineProxy()->GetSpriteRenderer().CreateSprite(m_textureNoise);

    return true;
}

void OperatorScreen::Cleanup()
{
    _GetEngineProxy()->GetSpriteRenderer().DestroySprite(m_spriteNoise);
    _GetEngineProxy()->GetTextureManager().DestroyTexture(m_textureNoise);

    delete m_menu;
    m_menu = nullptr;
}

void OperatorScreen::Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState)
{
    switch (m_state) {
        case e_StateMenuIdle:
            if (inputState.GetDigital(e_ISD_SelectNext)) {
                m_menu->SelectNextItem();
            } else if (inputState.GetDigital(e_ISD_SelectPrev)) {
                m_menu->SelectPreviousItem();
            } else if (inputState.GetDigital(e_ISD_Selected)) {
                if (!m_menu->ConfirmCurrentItem()) {
                    m_state = e_StateGoBack;
                }
            } else if (inputState.GetDigital(e_ISD_ExitApplication)) {
                m_state = e_StateExitApplication;
            }

            break;

        case e_StateNoise:
            if (inputState.GetDigital(e_ISD_NoiseBack)) {
                m_menu->Activate(true);
                _GetEngineProxy()->GetSpriteRenderer().SpriteSetActive(m_spriteNoise, false);
                m_state = e_StateMenuIdle;
            }

            _GetEngineProxy()->GetTextureManager().UpdateTextureNoise(m_textureNoise);

            break;

        case e_StateGoBack:
            _SetNextScreen(AttractScreen::MS_NAME);

            break;

        case e_StateExitApplication:
            _ExitApplication();

            KS_LOG_DEBUG("User quit");

            break;

        default:
            break;
    }
}

ssize_t OperatorScreen::__CallbackNoise()
{
    m_menu->Activate(false);
    _GetEngineProxy()->GetSpriteRenderer().SpriteSetActive(m_spriteNoise, true);
    m_state = e_StateNoise;

    return 0;
}

ssize_t OperatorScreen::__CallbackReboot()
{
    ks::System::Reboot();

    return 0;
}

ssize_t OperatorScreen::__CallbackShutdown()
{
    ks::System::Shutdown();

    return 0;
}

}