#include "SGL.h"

#include "ks/Defs.h"
#include "ks/KitchenSink.h"

#include "sgl/attract/AttractScreen.h"
#include "sgl/boot/BootScreen.h"
#include "sgl/error/ErrorScreen.h"
#include "sgl/operator/OperatorScreen.h"

#include "sgl/lua/LuaAsset.h"
#include "sgl/lua/LuaFont.h"
#include "sgl/lua/LuaISP.h"
#include "sgl/lua/LuaOSP.h"
#include "sgl/lua/LuaRenderer.h"
#include "sgl/lua/LuaSettingsLoader.h"
#include "sgl/lua/LuaSound.h"
#include "sgl/lua/LuaSprite.h"
#include "sgl/lua/LuaTime.h"
#include "sgl/lua/LuaTexture.h"
#include "sgl/lua/LuaVideo.h"

#include "SGLSettings.h"

namespace sgl {

SGL::SGL(int argc, char** argv) :
    m_argc(argc),
    m_argv(argv),
    m_settings(nullptr),
    m_dataManager(nullptr),
    m_ioManager(nullptr),
    m_eventProxy(nullptr),
    m_soundRenderer(nullptr),
    m_sfxManager(nullptr),
    m_renderer(nullptr),
    m_fontManager(nullptr),
    m_textureManager(nullptr),
    m_spriteRenderer(nullptr),
    m_videoManager(nullptr),
    m_luaState(nullptr),
    m_saveState(nullptr),
    m_gameLauncher(nullptr),
    m_nextScreenName(""),
    m_screenState(e_ScreenStateInit),
    m_currentScreen(nullptr),
    m_worldState(nullptr),
    m_proxy(nullptr),
    m_dummyInputState(),
    m_dummyOutputState()
{

}

int SGL::Main()
{
    if (!__Init()) {
        return -1;
    }

    __Update();
    __Shutdown();

    return 0;
}

bool SGL::__Init()
{
    if (m_argc < 2) {
        printf("Usage: %s <settings.lua>\n", m_argv[0]);
        return false;
    }

    ks::KitchenSink::Init("sgl.log");

    __LogSGLHeader();

    if (!__LoadSettings()) {
        return false;
    }

    __InitData();

    if (!__InitSfx()) {
        return false;
    }

    if (!__InitGfx()) {
        return false;
    }

    __InitIO();
    __InitLua();
    __InitScreenAndWorldState();

    return true;
}

void SGL::__Update()
{
    const double frameTimeSec = 1.0 / 60.0;
    bool loop = true;
    ks::Timer timer;

    timer.Start();

    KS_LOG_DEBUG("Starting render loop");

    while (loop) {
        switch (m_screenState) {
            case e_ScreenStateInit:
            {
                KS_LOG_DEBUG("Next screen '%s' creating instance...", m_nextScreenName);

                m_currentScreen = __CreateScreenInstance(m_nextScreenName);

                if (!m_currentScreen) {
                    loop = false;
                    break;
                }

                KS_LOG_DEBUG("Initializing screen %s", m_currentScreen->GetName());

                if (!m_currentScreen->Init()) {
                    KS_LOG_ERROR("Initializing screen %s failed", m_currentScreen->GetName());
                }

                if (m_ioManager->IsInitialized()) {
                    m_ioManager->SetIOProcessors(m_currentScreen->GetName());
                }

                m_screenState = e_ScreenStateUpdate;

                break;
            }

            case e_ScreenStateUpdate:
            {
                double deltaSec = timer.GetSec();

                if (deltaSec <= frameTimeSec) {
                    double sleep = (frameTimeSec - deltaSec) * 1000 * 1000 * 1000;
                    std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(sleep));
                    deltaSec = frameTimeSec;
                } else {
                    // TODO frameskipping?
                    KS_LOG_WARN("Frame took too long: %f sec", deltaSec);
                }

                timer.Start();

                if (!__ProcessSDLEvents()) {
                    loop = false;
                    break;
                }

                // Don't process any IO when not initialized, i.e. during boot
                // screen
                if (m_ioManager->IsInitialized()) {
                    m_ioManager->StateProcess();

                    m_currentScreen->Update(deltaSec, m_ioManager->GetInputStates(), m_ioManager->GetOutputStates());
                } else {
                    m_dummyInputState.Clear();
                    m_currentScreen->Update(deltaSec, m_dummyInputState, m_dummyOutputState);
                    m_dummyOutputState.Clear();
                }

                // Rendering
                m_videoManager->Update();

                m_renderer->Begin();
                m_spriteRenderer->Update();
                m_renderer->End();

                // screen events

                if (m_currentScreen->ExitApplication()) {
                    m_screenState = e_ScreenStateExitApplication;
                }

                if (!m_currentScreen->NextScreen().empty()) {
                    m_screenState = e_ScreenStateNext;
                }

                break;
            }

            case e_ScreenStateNext :
            {
                // push the output states of the last frame, otherwise they get lost on screen transition
                m_ioManager->StateProcess();
                m_ioManager->FlushOutputStates();

                m_nextScreenName = m_currentScreen->NextScreen();

                KS_LOG_DEBUG("Transition to next screen '%s' from current screen '%s'", m_nextScreenName,
                    m_currentScreen->GetName());

                KS_LOG_DEBUG("Screen '%s' cleanup...", m_currentScreen->GetName());
                m_currentScreen->Cleanup();

                delete m_currentScreen;
                m_currentScreen = nullptr;

                m_screenState = e_ScreenStateInit;

                break;
            }

            case e_ScreenStateExitApplication:
            {
                // push the output states of the last frame, otherwise they get lost when exiting the application
                m_ioManager->StateProcess();
                m_ioManager->FlushOutputStates();

                KS_LOG_DEBUG("Screen '%s' signals exit application", m_currentScreen->GetName());

                KS_LOG_DEBUG("Screen '%s' cleanup...", m_currentScreen->GetName());
                m_currentScreen->Cleanup();

                delete m_currentScreen;
                m_currentScreen = nullptr;

                loop = false;
                break;
            }

            default:
            {
                KS_LOG_ERROR("Unhandled screen state %d", m_screenState);
                loop = false;
                break;
            }
        }
    }
}

void SGL::__Shutdown()
{
    KS_LOG_INFO("Shutdown");

    __ShutdownScreenAndWorldState();
    __ShutdownLua();
    __ShutdownIO();
    __ShutdownSfx();
    __ShutdownGfx();
    __ShutdownData();

    ks::KitchenSink::Shutdown();
}

void SGL::__LogSGLHeader()
{
    KS_LOG_INFO("\n"
        "  ____   ____ _     \n"
        " / ___| / ___| |    \n"
        " \\___ \\| |  _| |    \n"
        "  ___) | |_| | |___ \n"
        " |____/ \\____|_____|");

    KS_LOG_INFO("SGL build " __DATE__ " " __TIME__ " gitrev %s", STRINGIFY(GITREV));
}

bool SGL::__LoadSettings()
{
    KS_LOG_INFO("Loading settings");

    m_settings = new ks::Settings();

    try {
        LuaSettingsLoader loader(m_argv[1]);
        loader.Load(*m_settings);
    } catch (ks::Exception& e) {
        delete m_settings;
        KS_LOG_ERROR("Loading settings failed: %s", e.what());
        return false;
    }

    return true;
}

void SGL::__InitData()
{
    KS_LOG_INFO("Data path: %s", m_settings->GetValue<const std::string&>(SGLSettings::ms_systemDataPath));

    m_dataManager = new DataManager(new ks::FileSystem(m_settings->GetValue<const std::string&>(
        SGLSettings::ms_systemDataPath)));

    m_saveState = new SaveState(new ks::FileSystem(m_settings->GetValue<const std::string&>(
        SGLSettings::ms_systemSavePath), "save"));
    m_saveState->Load();

    m_gameLauncher = new GameLauncher(new ks::FileSystem(m_settings->GetValue<const std::string&>(
        SGLSettings::ms_systemTmpPath), "tmp"));
    m_gameLauncher->Init();
}

bool SGL::__InitGfx()
{
    m_renderer = new Renderer(m_settings->GetValue<uint16_t>(SGLSettings::ms_gfxResWidth),
        m_settings->GetValue<uint16_t>(SGLSettings::ms_gfxResHeight),
        m_settings->GetValue<bool>(SGLSettings::ms_gfxFullscreen));

    if (!m_renderer->Init()) {
        KS_LOG_ERROR("Initializing renderer failed");
        return false;
    }

    m_fontManager = new FontManager(m_renderer);
    m_textureManager = new TextureManager(m_renderer, m_fontManager);
    m_spriteRenderer = new SpriteRenderer(m_renderer, m_textureManager);
    m_videoManager = new VideoManager(m_textureManager, m_soundRenderer);

    return true;
}

bool SGL::__InitSfx()
{
    m_soundRenderer = new SoundRenderer();

    m_soundRenderer->ApplySettings(*m_settings);
    m_soundRenderer->Init();

    m_sfxManager = new SFXManager(m_dataManager->GetRefFsSfx(), m_soundRenderer,
        m_settings->GetValue<float>(SGLSettings::ms_soundSfxVolume));

    return true;
}

void SGL::__InitIO()
{
    io::IOManagerCtx* ioManagerCtx = new io::IOManagerCtx();
    ioManagerCtx->Put("window", m_renderer->GetNativeRenderWindowHandle());

    m_eventProxy = new io::sdl2::EventProxy();
    ioManagerCtx->Put("sdl2-event-proxy", m_eventProxy);

    m_ioManager = new io::IOManager(ioManagerCtx, __LoadIOProcessorsFromScripts());
    // Init done in boot loading thread

    __InitSDL2Joystick();
}

void SGL::__InitSDL2Joystick()
{
    // Ensures that the SDL2 event loop receives joystick events for all available joysticks (rather hacky)

    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
        KS_LOG_ERROR("Initializing SDL joystick failed: %s", SDL_GetError());
    }

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        // Memory leak returned joystick devices not freed (on purpose)
        SDL_JoystickOpen(i);
    }
}

void SGL::__InitLua()
{
    m_luaState = new LuaState();
    m_luaState->LoadStdLibs();

    LuaAsset::ExposeToState(m_luaState, m_dataManager);
    LuaFont::ExposeToState(m_luaState, m_fontManager);
    LuaRenderer::ExposeToState(m_luaState, m_renderer);
    LuaSound::ExposeToState(m_luaState, m_soundRenderer);
    LuaSprite::ExposeToState(m_luaState, m_spriteRenderer);
    LuaTexture::ExposeToState(m_luaState, m_textureManager);
    LuaTime::ExposeToState(m_luaState);
    LuaVideo::ExposeToState(m_luaState, m_videoManager);
}

void SGL::__InitScreenAndWorldState()
{
    // Set initial screen states and first screen
    m_nextScreenName = BootScreen::MS_NAME;
    m_screenState = e_ScreenStateInit;
    m_currentScreen = nullptr;

    m_worldState = new WorldState("root");

    m_proxy = new EngineProxy(m_settings, m_dataManager, m_ioManager, m_soundRenderer, m_sfxManager, m_renderer,
        m_fontManager, m_textureManager, m_spriteRenderer, m_videoManager, m_worldState, m_saveState, m_luaState,
        m_gameLauncher);
}

void SGL::__ShutdownScreenAndWorldState()
{
    delete m_proxy;
    m_proxy = nullptr;

    delete m_worldState;
    m_worldState = nullptr;
}

void SGL::__ShutdownLua()
{
    delete m_luaState;
    m_luaState = nullptr;
}

void SGL::__ShutdownIO()
{
    m_ioManager->Shutdown();
    delete m_ioManager;
    m_ioManager = nullptr;

    delete m_eventProxy;
    m_eventProxy = nullptr;
}

void SGL::__ShutdownSfx()
{
    delete m_sfxManager;
    m_sfxManager = nullptr;

    m_soundRenderer->Shutdown();
    delete m_soundRenderer;
    m_soundRenderer = nullptr;
}

void SGL::__ShutdownGfx()
{
    delete m_spriteRenderer;
    m_spriteRenderer = nullptr;

    delete m_textureManager;
    m_textureManager = nullptr;

    delete m_fontManager;
    m_fontManager = nullptr;

    m_renderer->Shutdown();
    delete m_renderer;
    m_renderer = nullptr;
}

void SGL::__ShutdownData()
{
    // write back
    m_saveState->Save();
    delete m_saveState;
    m_saveState = nullptr;

    delete m_dataManager;
    m_dataManager = nullptr;

    delete m_settings;
    m_settings = nullptr;
}

Screen* SGL::__CreateScreenInstance(const std::string& name)
{
    Screen* screen = nullptr;

    if (name == AttractScreen::MS_NAME) {
        screen = new AttractScreen();
    } else if (name == BootScreen::MS_NAME) {
        screen = new BootScreen();
    } else if (name == ErrorScreen::MS_NAME) {
        screen = new ErrorScreen();
    } else if (name == OperatorScreen::MS_NAME) {
        screen = new OperatorScreen();
    } else {
        KS_LOG_ERROR("Can't create screen instance of unknown screen name '%s'", name);
    }

    if (screen) {
        screen->SetEngineProxy(m_proxy);
    }

    return screen;
}

io::IOStateProcessorManager* SGL::__LoadIOProcessorsFromScripts()
{
    auto* ioProcessManager = new io::IOStateProcessorManager();

    try {
        auto ispScripts = m_settings->GetValue<const std::vector<std::string>&>(SGLSettings::ms_ispScripts);

        for (auto& it : ispScripts) {
            auto binding = new LuaISPBindings(m_dataManager->GetRefFsLua()->GetAbsolutePath(it));
            binding->Load();
            ioProcessManager->Add(new LuaISP(binding));
        }
    } catch (ks::SettingsException& e) {
        // ignore if no tools are defined
    }

    try {
        auto ospScripts = m_settings->GetValue<const std::vector<std::string>&>(SGLSettings::ms_ospScripts);

        for (auto& it : ospScripts) {
            auto binding = new LuaOSPBindings(m_dataManager->GetRefFsLua()->GetAbsolutePath(it));
            binding->Load();
            ioProcessManager->Add(new LuaOSP(binding));
        }
    } catch (ks::SettingsException& e) {
        // ignore if no tools are defined
    }

    return ioProcessManager;
}

bool SGL::__ProcessSDLEvents()
{
    SDL_Event e {};

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
            {
                KS_LOG_DEBUG("Quitting render loop");

                return false;
            }

            case SDL_KEYUP:
                // Fallthrough
            case SDL_KEYDOWN:
            {
                m_eventProxy->ProcessKeyboardEvent(e.key);
                break;
            }

            case SDL_JOYBUTTONDOWN:
                // Fallthrough
            case SDL_JOYBUTTONUP:
            {
                m_eventProxy->ProcessJoystickButtonEvent(e.jbutton);
                break;
            }

            case SDL_WINDOWEVENT:
            {
                switch (e.window.event) {
                    case SDL_WINDOWEVENT_EXPOSED:
                        // repaint
                        SDL_RenderPresent(m_renderer->GetRenderer());
                        break;

                    case SDL_WINDOWEVENT_CLOSE:
                        KS_LOG_DEBUG("SDL_WINDOWEVENT_CLOSE");
                        return false;

                    default:
                        break;
                }

                break;
            }

            default:
                // printf("Unhandled SDL event: %d\n", e.type);
                break;
        }
    }

    return true;
}

}