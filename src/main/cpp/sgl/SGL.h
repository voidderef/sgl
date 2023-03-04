#pragma once

#include "io/IOManager.h"
#include "io/sdl2/EventProxy.h"

#include "ks/Settings.h"

#include "sgl/lua/LuaState.h"

#include "DataManager.h"
#include "EngineProxy.h"
#include "FontManager.h"
#include "Renderer.h"
#include "SaveState.h"
#include "Screen.h"
#include "SFXManager.h"
#include "SoundRenderer.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "VideoManager.h"
#include "WorldState.h"

namespace sgl {

class SGL
{
public:
    SGL(int argc, char** argv);
    ~SGL() = default;

    int Main();

private:
    bool __Init();
    void __Update();
    void __Shutdown();

private:
    int m_argc;
    char** m_argv;

private:
    enum ScreenState {
        e_ScreenStateInit = 0,
        e_ScreenStateUpdate = 1,
        e_ScreenStateNext = 2,
        e_ScreenStateExitApplication = 3,
    };

private:
    ks::Settings* m_settings;

    DataManager* m_dataManager;
    io::IOManager* m_ioManager;
    io::sdl2::EventProxy* m_eventProxy;

    SoundRenderer* m_soundRenderer;
    SFXManager* m_sfxManager;
    Renderer* m_renderer;
    FontManager* m_fontManager;
    TextureManager* m_textureManager;
    SpriteRenderer* m_spriteRenderer;
    VideoManager* m_videoManager;

    LuaState* m_luaState;

    WorldState* m_worldState;
    SaveState* m_saveState;
    GameLauncher* m_gameLauncher;

    std::string m_nextScreenName;
    ScreenState m_screenState;
    Screen* m_currentScreen;

    EngineProxy* m_proxy;

    io::InputState m_dummyInputState;
    io::OutputState m_dummyOutputState;

private:
    void __LogSGLHeader();
    bool __LoadSettings();
    void __InitData();
    bool __InitSfx();
    bool __InitGfx();
    void __InitIO();
    void __InitSDL2Joystick();
    void __InitLua();
    void __InitScreenAndWorldState();

    void __ShutdownScreenAndWorldState();
    void __ShutdownLua();
    void __ShutdownIO();
    void __ShutdownSfx();
    void __ShutdownGfx();
    void __ShutdownData();

    Screen* __CreateScreenInstance(const std::string& name);
    io::IOStateProcessorManager* __LoadIOProcessorsFromScripts();
    bool __ProcessSDLEvents();
};

}
