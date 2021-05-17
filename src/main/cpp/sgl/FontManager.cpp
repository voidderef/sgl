#include "FontManager.h"

#include "ks/Logger.hpp"

#include "FontException.h"

namespace sgl {

FontManager::FontManager(Renderer* refRenderer) :
    m_refRenderer(refRenderer),
    m_handleCount(0),
    m_cacheMappingPath(),
    m_cache()
{
    TTF_Init();
}

FontManager::~FontManager()
{
    for (uint32_t i = 0; i < m_handleCount; i++) {
        if (m_fonts[i] != nullptr) {
            KS_LOG_WARN("Font %d not destroyed, cleanup", i);
            TTF_CloseFont(m_fonts[i]);
        }
    }
}

uint16_t FontManager::CreateFontFromTTFFile(const std::string& path, uint16_t size)
{
    uint16_t id;
    TTF_Font* font;

    id = __GetFromCache(path, size);

    if (id != INVALID_HANDLE_ID) {
        return id;
    }

    font = TTF_OpenFont(path.c_str(), size);

    if (!font) {
        throw FontException("Opening font '%s' failed: %s", path, TTF_GetError());
    }

    id = __Alloc();

    KS_LOG_DEBUG("Font %d of file %s, size %d loaded", id, path, size);

    m_fonts[id] = font;

    __AddToCache(path, size, id);

    return id;
}

void FontManager::DestroyFont(uint16_t& id)
{
    // check if cached font reference count is 0 before deleting
    if (__RemoveFromCache(id)) {
        KS_LOG_DEBUG("Destroy %d", id);
        __Free(id);
    }

    id = INVALID_HANDLE_ID;
}

uint16_t FontManager::__Alloc()
{
    uint16_t id;

    if (m_handleCount + 1 == INVALID_HANDLE_ID) {
        throw FontException("Out of handles");
    }

    if (m_freeHandles.empty()) {
        id = m_handleCount++;

        m_fonts.resize(m_handleCount, nullptr);
    } else {
        id = m_freeHandles.back();
        m_freeHandles.pop_back();
    }

    return id;
}

void FontManager::__Free(uint16_t id)
{
    TTF_CloseFont(m_fonts[id]);
    m_fonts[id] = nullptr;

    m_freeHandles.push_back(id);
}

uint16_t FontManager::__GetFromCache(const std::string& path, uint16_t size)
{
    // check cache to avoid loading one font multiple times
    auto it = m_cacheMappingPath.find(path + "-" + std::to_string(size));

    if (it != m_cacheMappingPath.end()) {
        m_cache[it->second].m_refCount++;
        return m_cache[it->second].m_id;
    }

    return INVALID_HANDLE_ID;
}

void FontManager::__AddToCache(const std::string& path, uint16_t size, uint16_t id)
{
    if (m_cache.size() <= id) {
        m_cache.resize(id + 1);
    }

    CacheEntry entry {};
    entry.m_key = path + "-" + std::to_string(size);
    entry.m_refCount = 0;
    entry.m_id = id;

    m_cache[id] = entry;
    m_cacheMappingPath.insert(std::make_pair(entry.m_key, entry.m_id));
}

bool FontManager::__RemoveFromCache(uint16_t id)
{
    if (--m_cache[id].m_refCount == 0) {
        m_cacheMappingPath.erase(m_cache[id].m_key);
        m_cache[id].m_key.clear();
        m_cache[id].m_id = INVALID_HANDLE_ID;
        return true;
    }

    return false;
}

}