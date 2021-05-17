#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "ks/Object.h"

#include "Renderer.h"
#include "TextureManager.h"

namespace sgl {

class SpriteRenderer : public ks::Object
{
public:
    static const uint16_t INVALID_HANDLE_ID = 0xFFFF;

    SpriteRenderer(Renderer* refRenderer, TextureManager* refTextureManager);

    ~SpriteRenderer() override;

    uint16_t CreateSprite(uint16_t textureId);

    void DestroySprite(uint16_t& id);

    void SpriteSetTexture(uint16_t id, uint16_t textureId);

    void SpriteSetActive(uint16_t id, bool active);

    void SpriteSetPos(uint16_t id, uint32_t x, uint32_t y);

    void SpriteSetX(uint16_t id, uint32_t val);

    void SpriteSetY(uint16_t id, uint32_t val);

    void SpriteSetSize(uint16_t id, uint32_t w, uint32_t h);

    void SpriteSetWidth(uint16_t id, uint32_t val);

    void SpriteSetHeight(uint16_t id, uint32_t val);

    void SpriteSetAlpha(uint16_t id, uint8_t val);

    void SpriteSetLayer(uint16_t id, uint8_t val);

    void Update();

private:
    Renderer* m_refRenderer;
    TextureManager* m_textureManager;

private:
    std::vector<bool> m_active;
    std::vector<uint16_t> m_textureId;
    std::vector<SDL_Rect> m_rectSprite;
    std::vector<uint8_t> m_alpha;
    std::vector<uint8_t> m_layer;

    uint16_t m_handleCount;
    std::vector<uint16_t> m_freeHandles;

    uint16_t __Alloc();
    void __Free(uint16_t id);
};

}
