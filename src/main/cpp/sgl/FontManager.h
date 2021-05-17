#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL_ttf.h>

#include "ks/Object.h"

#include "Renderer.h"

namespace sgl {

// forward declaration for friendship
class TextureManager;

class FontManager : public ks::Object
{
public:
    friend class TextureManager;

    static const uint16_t INVALID_HANDLE_ID = 0xFFFF;

    explicit FontManager(Renderer* refRenderer);

    ~FontManager() override;

    uint16_t CreateFontFromTTFFile(const std::string& path, uint16_t size);

    void DestroyFont(uint16_t& id);

private:
    Renderer* m_refRenderer;

    std::vector<TTF_Font*> m_fonts;

    uint16_t m_handleCount;
    std::vector<uint16_t> m_freeHandles;

    uint16_t __Alloc();
    void __Free(uint16_t id);

private:
    struct CacheEntry {
        std::string m_key;
        uint16_t m_refCount;
        uint16_t m_id;
    };

    std::unordered_map<std::string, size_t> m_cacheMappingPath;
    std::vector<CacheEntry> m_cache;

    uint16_t __GetFromCache(const std::string& path, uint16_t size);
    void __AddToCache(const std::string& path, uint16_t size, uint16_t id);
    bool __RemoveFromCache(uint16_t id);
};

}
