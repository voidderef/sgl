#include "TextureManager.h"

extern "C" {
#include <SDL2/SDL_image.h>
}

#include "ks/FileSystem.h"
#include "ks/Random.h"
#include "ks/StringUtils.h"

#include "TextureException.h"

namespace sgl {

TextureManager::TextureManager(Renderer* refRenderer,
        FontManager* refFontManager) :
    m_refRenderer(refRenderer),
    m_refFontManager(refFontManager),
    m_handleCount(0)
{
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

TextureManager::~TextureManager()
{
    for (uint32_t i = 0; i < m_handleCount; i++) {
        if (m_texture[i] != nullptr) {
            KS_LOG_WARN("Texture %d not destroyed, cleanup", i);
            SDL_DestroyTexture(m_texture[i]);
        }
    }
}

uint16_t TextureManager::CreateStreamingTexture(uint32_t width, uint32_t height)
{
    uint16_t id = __Alloc();

    m_texture[id] = SDL_CreateTexture(m_refRenderer->GetRenderer(),
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);

    if (!m_texture[id]) {
        throw TextureException("Creating new texture " + std::to_string(width) + "x" + std::to_string(height) +
            " failed");
    }

    m_rectTexture[id].x = 0;
    m_rectTexture[id].y = 0;
    m_rectTexture[id].w = width;
    m_rectTexture[id].h = height;

    KS_LOG_DEBUG("Texture %d %dx%d created", id, m_rectTexture[id].w, m_rectTexture[id].h);

    return id;
}

uint16_t TextureManager::CreateTextureFromFile(const std::string& path)
{
    uint16_t id;
    const std::string extension = ks::FileSystem::GetFileExtension(path);
    ks::StringUtils::ToLower(extension);

    SDL_Surface* surface;

    if (extension == "bmp") {
        id = __Alloc();

        surface = SDL_LoadBMP(path.c_str());
    } else if (extension == "png" || extension == "jpg") {
        id = __Alloc();

        surface = IMG_Load(path.c_str());
    } else {
        throw TextureException("Unsupported file format: " + extension);
    }

    if (!surface) {
        throw TextureException("Creating surface from " + extension + " file '" + path + "' failed: " +
            std::string(SDL_GetError()));
    }

    m_rectTexture[id].x = 0;
    m_rectTexture[id].y = 0;
    m_rectTexture[id].w = (uint32_t) surface->w;
    m_rectTexture[id].h = (uint32_t) surface->h;

    m_texture[id] = SDL_CreateTextureFromSurface(m_refRenderer->GetRenderer(),
        surface);

    if (!m_texture[id]) {
        throw TextureException("Creating texture from surface of bmp file '" + path + "' failed: " +
            std::string(SDL_GetError()));
    }

    SDL_FreeSurface(surface);

    KS_LOG_DEBUG("Texture %d %dx%d of file %s loaded", id, m_rectTexture[id].w, m_rectTexture[id].h, path);

    return id;
}

uint16_t TextureManager::CreateTextureFromText(const std::string& text,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint16_t fontId)
{
    if (text.empty()) {
        throw TextureException("Text is empty");
    }

    if (!m_refFontManager->m_fonts[fontId]) {
        throw TextureException("Invalid font id: " + std::to_string(fontId));
    }

    uint16_t id = __Alloc();

    SDL_Surface* surface = TTF_RenderText_Solid(
        m_refFontManager->m_fonts[fontId], text.c_str(), {r, g, b, a});

    m_texture[id] = SDL_CreateTextureFromSurface(m_refRenderer->GetRenderer(),
        surface);

    if (!m_texture[id]) {
        SDL_FreeSurface(surface);
        __Free(id);

        throw TextureException("Creating new texture from surface for text failed" + std::string(SDL_GetError()));
    }

    SDL_FreeSurface(surface);

    m_rectTexture[id].x = 0;
    m_rectTexture[id].y = 0;
    m_rectTexture[id].w = surface->w;
    m_rectTexture[id].h = surface->h;

    return id;
}

void TextureManager::DestroyTexture(uint16_t& id)
{
    KS_LOG_DEBUG("Destroy %d", id);

    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid texture id: " + std::to_string(id));
    }

    __Free(id);
    id = INVALID_HANDLE_ID;
}

void TextureManager::UpdateTextureNoise(uint16_t id)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid texture id: " + std::to_string(id));
    }

    auto texDim = (uint32_t) (m_rectTexture[id].w * m_rectTexture[id].h);

    void* data;
    int pitch;
    uint8_t* buffer;

    if (SDL_LockTexture(m_texture[id], nullptr, &data, &pitch) < 0) {
        throw TextureException("Locking texture, id " + std::to_string(id) +" failed: " + std::string(SDL_GetError()));
    }

    buffer = (uint8_t*) data;

    for (uint32_t i = 0; i < texDim; i++) {
        uint8_t val = ks::Random::U8();

        // RGBA format
        buffer[i * 4] = val;
        buffer[i * 4 + 1] = val;
        buffer[i * 4 + 2] = val;
        buffer[i * 4 + 3] = 0xFF;
    }

    SDL_UnlockTexture(m_texture[id]);
}

void TextureManager::UpdateTextureVideo(uint16_t id, VideoDecoder* refVideoDecoder)
{
    if (id == INVALID_HANDLE_ID) {
        throw TextureException("Invalid texture id: " + std::to_string(id));
    }

    void* data;
    int pitch;

    if (SDL_LockTexture(m_texture[id], nullptr, &data, &pitch) < 0) {
        throw TextureException("Locking texture, id " + std::to_string(id) + " failed: " + std::string(SDL_GetError()));
    }

    // this call already scales the video frame to the correct window size
    bool pollFrames = true;

    while (pollFrames && !refVideoDecoder->IsEof()) {
        VideoDecoder::VideoFrameState state = refVideoDecoder->GetNextVideoFrame(data, (uint32_t) m_rectTexture[id].w,
            (uint32_t) m_rectTexture[id].h, VideoDecoder::e_PixelFormatRGBA, 60.0);

        switch (state) {
            case VideoDecoder::e_VideoFrameStateNA:
            case VideoDecoder::e_VideoFrameStateOnTime:
                pollFrames = false;
                break;

            case VideoDecoder::e_VideoFrameStateQueueDelay:
                ks::Thread::SleepUs(100);
                KS_LOG_DEBUG("Video decoding queue delay");
                break;

            case VideoDecoder::e_VideoFrameStateSkip:
                KS_LOG_DEBUG("Video decoder frame skip");
                // continue polling frames until all frames to skip are skipped
                break;

            default:
                throw TextureException("Invalid video decoding state");
        }
    }

    SDL_UnlockTexture(m_texture[id]);
}

uint16_t TextureManager::__Alloc()
{
    uint16_t id;

    if (m_handleCount + 1 == INVALID_HANDLE_ID) {
        throw TextureException("Out of handles");
    }

    if (m_freeHandles.empty()) {
        id = m_handleCount++;

        m_texture.resize(m_handleCount, nullptr);
        m_rectTexture.resize(m_handleCount);
    } else {
        id = m_freeHandles.back();
        m_freeHandles.pop_back();
    }

    return id;
}

void TextureManager::__Free(uint16_t id)
{
    SDL_DestroyTexture(m_texture[id]);
    m_texture[id] = nullptr;

    m_freeHandles.push_back(id);
}

}