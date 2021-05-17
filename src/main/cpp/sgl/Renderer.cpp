#include "Renderer.h"

#include <SDL2/SDL_syswm.h>

// avoid conflicts with other libs using identical names/macros
#ifdef None
#undef None
#endif
#ifdef True
#undef True
#endif
#ifdef False
#undef False
#endif

#include "ks/Logger.hpp"

namespace sgl {

Renderer::Renderer(uint32_t resolutionX, uint32_t resolutionY, bool fullscreen) :
    m_resolutionX(resolutionX),
    m_resolutionY(resolutionY),
    m_fullscreen(fullscreen),
    m_window(nullptr),
    m_renderer(nullptr),
    m_nativeRenderWindowHandle(nullptr)
{

}

bool Renderer::Init()
{
    SDL_SysWMinfo wmInfo {};

    KS_LOG_INFO("Initialize %dx%d, fullscreen %d", m_resolutionX, m_resolutionY, m_fullscreen);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        KS_LOG_ERROR("Initializing SDL video failed: %s", SDL_GetError());
        goto fail;
    }

    m_window = SDL_CreateWindow("SGL", 0, 0, m_resolutionX, m_resolutionY, 0);

    if (!m_window) {
        KS_LOG_ERROR("Creating window failed: %s", SDL_GetError());
        goto fail;
    }

    if (m_fullscreen) {
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer) {
        KS_LOG_ERROR("Creating renderer failed: %s", SDL_GetError());
        goto fail_window;
    }

    SDL_ShowCursor(SDL_DISABLE);

    // get native window handle
    SDL_GetVersion(&wmInfo.version);

    if (SDL_GetWindowWMInfo(m_window, &wmInfo) == SDL_FALSE) {
        KS_LOG_ERROR("Accessing native window information failed: %s", SDL_GetError());
        goto fail_render;
    }

#ifdef SGL_PLATFORM_LINUX
    m_nativeRenderWindowHandle = reinterpret_cast<void*>(wmInfo.info.x11.window);
#elif defined(SGL_PLATFORM_WIN32)
    m_nativeRenderWindowHandle = reinterpret_cast<void*>(wmInfo.info.win.window);
#else
#error "Non supported platform"
#endif

    return true;

fail_render:
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;

fail_window:
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

fail:
    return false;
}

void Renderer::Begin()
{
    SDL_RenderClear(m_renderer);
}

void Renderer::End()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::Shutdown()
{
    KS_LOG_INFO("Shutdown");

    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;

    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

}