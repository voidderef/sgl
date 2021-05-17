#pragma once

#include <SDL2/SDL.h>

namespace sgl {

class Renderer
{
public:
    Renderer(uint32_t resolutionX, uint32_t resolutionY, bool fullscreen);

    ~Renderer() = default;

    void* GetNativeRenderWindowHandle() {
        return m_nativeRenderWindowHandle;
    }

    uint32_t GetResolutionX() {
        return m_resolutionX;
    }

    uint32_t GetResolutionY() {
        return m_resolutionY;
    }

    bool Init();

    void Begin();

    void End();

    void Shutdown();

    SDL_Renderer* GetRenderer() const {
        return m_renderer;
    }

private:
    uint32_t m_resolutionX;
    uint32_t m_resolutionY;
    bool m_fullscreen;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    void* m_nativeRenderWindowHandle;
};

}
