#pragma once

#include "ks/Object.h"

#include "FontManager.h"
#include "Renderer.h"
#include "VideoDecoder.h"

namespace sgl {

// forward declaration for friendship
class SpriteRenderer;

class TextureManager : public ks::Object
{
public:
    friend class SpriteRenderer;

    static const uint16_t INVALID_HANDLE_ID = 0xFFFF;

    TextureManager(Renderer* refRenderer, FontManager* refFontManager);

    ~TextureManager() override;

    uint16_t CreateStreamingTexture(uint32_t width, uint32_t height);

    uint16_t CreateTextureFromFile(const std::string& path);

    uint16_t CreateTextureFromText(const std::string& text, uint8_t r, uint8_t g, uint8_t b, uint8_t a,
        uint16_t fontId);

    void DestroyTexture(uint16_t& id);

    void UpdateTextureNoise(uint16_t id);

    void UpdateTextureVideo(uint16_t id, VideoDecoder* refVideoDecoder);

private:
    Renderer* m_refRenderer;
    FontManager* m_refFontManager;

    std::vector<SDL_Texture*> m_texture;
    std::vector<SDL_Rect> m_rectTexture;

    uint16_t m_handleCount;
    std::vector<uint16_t> m_freeHandles;

    uint16_t __Alloc();
    void __Free(uint16_t id);
};

}
