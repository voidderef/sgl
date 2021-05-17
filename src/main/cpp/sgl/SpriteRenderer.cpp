#include "SpriteRenderer.h"

#include "SpriteException.h"

namespace sgl {

SpriteRenderer::SpriteRenderer(Renderer* refRenderer,
        TextureManager* refTextureManager) :
    m_refRenderer(refRenderer),
    m_textureManager(refTextureManager),
    m_handleCount(0)
{

}

SpriteRenderer::~SpriteRenderer()
{
    for (uint32_t i = 0; i < m_handleCount; i++) {
        if (m_textureId[i] != TextureManager::INVALID_HANDLE_ID) {
            KS_LOG_WARN("Sprite %d referencing texture %d not destroyed, cleanup", i, m_textureId[i]);
        }
    }
}

uint16_t SpriteRenderer::CreateSprite(uint16_t textureId)
{
    if (textureId == TextureManager::INVALID_HANDLE_ID) {
        throw TextureException("Invalid texture id: %d", textureId);
    }

    uint16_t id = __Alloc();

    m_active[id] = false;

    m_textureId[id] = textureId;

    m_rectSprite[id].x = 0;
    m_rectSprite[id].y = 0;
    m_rectSprite[id].w = m_textureManager->m_rectTexture[textureId].w;
    m_rectSprite[id].h = m_textureManager->m_rectTexture[textureId].h;

    m_alpha[id] = 0xFF;
    m_layer[id] = 0;

    KS_LOG_DEBUG("Sprite %d %dx%d of texture %d create", id, m_rectSprite[id].w, m_rectSprite[id].h, textureId);

    return id;
}

void SpriteRenderer::DestroySprite(uint16_t& id)
{
    KS_LOG_DEBUG("Destroy %d", id);

    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    __Free(id);
    id = INVALID_HANDLE_ID;
}

void SpriteRenderer::SpriteSetTexture(uint16_t id, uint16_t textureId)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_textureId[id] = textureId;
}

void SpriteRenderer::SpriteSetActive(uint16_t id, bool active)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_active[id] = active;
}

void SpriteRenderer::SpriteSetPos(uint16_t id, uint32_t x, uint32_t y)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_rectSprite[id].x = x;
    m_rectSprite[id].y = y;
}

void SpriteRenderer::SpriteSetX(uint16_t id, uint32_t val)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_rectSprite[id].x = val;
}

void SpriteRenderer::SpriteSetY(uint16_t id, uint32_t val)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_rectSprite[id].y = val;
}

void SpriteRenderer::SpriteSetSize(uint16_t id, uint32_t w, uint32_t h)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_rectSprite[id].w = w;
    m_rectSprite[id].h = h;
}

void SpriteRenderer::SpriteSetWidth(uint16_t id, uint32_t val)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_rectSprite[id].w = val;
}

void SpriteRenderer::SpriteSetHeight(uint16_t id, uint32_t val)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_rectSprite[id].h = val;
}

void SpriteRenderer::SpriteSetAlpha(uint16_t id, uint8_t val)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_alpha[id] = val;
}

void SpriteRenderer::SpriteSetLayer(uint16_t id, uint8_t val)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid sprite id: %d", id);
    }

    m_layer[id] = val;
}

void SpriteRenderer::Update()
{
    for (size_t layer = 0; layer <= 0xFF; layer++) {
        for (size_t i = 0; i < m_handleCount; i++) {
            if (m_layer[i] == layer && m_active[i]) {
                SDL_SetTextureBlendMode(m_textureManager->m_texture[m_textureId[i]], SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(m_textureManager->m_texture[m_textureId[i]], m_alpha[i]);

                SDL_RenderCopy(m_refRenderer->GetRenderer(),
                               m_textureManager->m_texture[m_textureId[i]],
                               &m_textureManager->m_rectTexture[m_textureId[i]],
                               &m_rectSprite[i]);
            }
        }
    }
}

uint16_t SpriteRenderer::__Alloc()
{
    uint16_t id;

    if (m_handleCount + 1 == 0xFFFF) {
        throw TextureException("Out of handles");
    }

    if (m_freeHandles.empty()) {
        id = m_handleCount++;

        m_active.resize(m_handleCount, false);
        m_textureId.resize(m_handleCount);
        m_rectSprite.resize(m_handleCount);
        m_alpha.resize(m_handleCount);
        m_layer.resize(m_handleCount);
    } else {
        id = m_freeHandles.back();
        m_freeHandles.pop_back();
    }

    return id;
}

void SpriteRenderer::__Free(uint16_t id)
{
    m_active[id] = false;
    m_textureId[id] = TextureManager::INVALID_HANDLE_ID;

    m_freeHandles.push_back(id);
}

}