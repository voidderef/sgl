#pragma once

#include "ks/Object.h"

#include "SoundRenderer.h"
#include "TextureManager.h"
#include "VideoDecoder.h"

namespace sgl {

class VideoManager : public ks::Object
{
public:
    static const uint16_t INVALID_HANDLE_ID = 0xFFFF;

    VideoManager(TextureManager* refTextureManager, SoundRenderer* refSoundRenderer);

    ~VideoManager() override;

    uint16_t CreateVideo(const std::string& path, bool withSound = true);

    void DestroyVideo(uint16_t& id);

    uint16_t GetVideoTexture(uint16_t id) const;

    uint32_t GetWidth(uint16_t id) const;

    uint32_t GetHeight(uint16_t id) const;

    void SetVolume(uint16_t id, float volume);

    void SetLoopStart(uint16_t id, uint32_t framePos);

    void SetLoop(uint16_t id, int32_t loop);

    void Play(uint16_t id);

    void Pause(uint16_t id, bool pause);

    void Stop(uint16_t id);

    void Reset(uint16_t id);

    bool IsEof(uint16_t id);

    bool IsRunning(uint16_t id);

    void Update();

private:
    TextureManager* m_refTextureManager;
    SoundRenderer* m_refSoundRenderer;

    std::vector<uint16_t> m_videoTexture;
    std::vector<uint16_t> m_audioStreams;
    std::vector<VideoDecoder*> m_decoders;

    uint16_t m_handleCount;
    std::vector<uint16_t> m_freeHandles;

    uint16_t __Alloc();
    void __Free(uint16_t id);
};

}
