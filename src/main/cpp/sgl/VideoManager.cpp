#include "VideoManager.h"

#include "VideoException.h"

namespace sgl {

VideoManager::VideoManager(TextureManager* refTextureManager, SoundRenderer* refSoundRenderer) :
    m_refTextureManager(refTextureManager),
    m_refSoundRenderer(refSoundRenderer),
    m_handleCount(0)
{

}

VideoManager::~VideoManager()
{
    for (uint32_t i = 0; i < m_handleCount; i++) {
        if (m_decoders[i]) {
            KS_LOG_WARN("Video %d not destroyed, memory leak", i);
        }
    }
}

uint16_t VideoManager::CreateVideo(const std::string& path, bool withSound)
{
    uint16_t id = __Alloc();

    m_decoders[id] = new VideoDecoder(path, withSound);

    m_videoTexture[id] = m_refTextureManager->CreateStreamingTexture(m_decoders[id]->GetVideoWidth(),
        m_decoders[id]->GetVideoHeight());

    if (withSound && m_decoders[id]->WithAudio()) {
        m_audioStreams[id] = m_refSoundRenderer->CreateVideoSoundStream(m_decoders[id]);
        m_refSoundRenderer->Play(m_audioStreams[id]);
    } else {
        m_audioStreams[id] = SoundRenderer::INVALID_HANDLE_ID;
    }

    KS_LOG_DEBUG("Created video %s, with sound %d (%d): decoder %s, texture %d", path, withSound, id, *m_decoders[id],
        m_videoTexture[id]);

    return id;
}

void VideoManager::DestroyVideo(uint16_t& id)
{
    KS_LOG_DEBUG("Destroying video %d: decoder %s, texture %d", id, *m_decoders[id], m_videoTexture[id]);

    m_decoders[id]->Stop();
    delete m_decoders[id];
    m_decoders[id] = nullptr;

    if (m_audioStreams[id] != SoundRenderer::INVALID_HANDLE_ID) {
        m_refSoundRenderer->Stop(m_audioStreams[id]);
        m_refSoundRenderer->DestorySound(m_audioStreams[id]);
    }

    m_refTextureManager->DestroyTexture(m_videoTexture[id]);

    __Free(id);
    id = VideoManager::INVALID_HANDLE_ID;
}

uint16_t VideoManager::GetVideoTexture(uint16_t id) const
{
    return m_videoTexture[id];
}

uint32_t VideoManager::GetWidth(uint16_t id) const
{
    return m_decoders[id]->GetVideoWidth();
}

uint32_t VideoManager::GetHeight(uint16_t id) const
{
    return m_decoders[id]->GetVideoHeight();
}

void VideoManager::SetVolume(uint16_t id, float volume)
{
    m_refSoundRenderer->SetVolume(m_audioStreams[id], volume);
}

void VideoManager::SetLoopStart(uint16_t id, uint32_t framePos)
{
    m_decoders[id]->SetLoopStartPos(framePos);
}

void VideoManager::SetLoop(uint16_t id, int32_t loop)
{
    m_decoders[id]->SetLoop(loop);
}

void VideoManager::Play(uint16_t id)
{
    m_decoders[id]->Start();
}

void VideoManager::Pause(uint16_t id, bool pause)
{
    m_decoders[id]->Pause(pause);
}

void VideoManager::Stop(uint16_t id)
{
    m_decoders[id]->Stop();
}

void VideoManager::Reset(uint16_t id)
{
    m_decoders[id]->Reset();
}

bool VideoManager::IsEof(uint16_t id)
{
    return m_decoders[id]->IsEof();
}

bool VideoManager::IsRunning(uint16_t id)
{
    return m_decoders[id]->IsRunning();
}

void VideoManager::Update()
{
    for (uint16_t i = 0; i < m_handleCount; i++) {
        if (m_decoders[i] && m_decoders[i]->IsRunning()) {
            m_refTextureManager->UpdateTextureVideo(m_videoTexture[i], m_decoders[i]);
        }
    }
}

uint16_t VideoManager::__Alloc()
{
    uint16_t id;

    if (m_handleCount + 1 == INVALID_HANDLE_ID) {
        throw VideoException("Out of handles");
    }

    if (m_freeHandles.empty()) {
        id = m_handleCount++;

        uint16_t tmp = TextureManager::INVALID_HANDLE_ID;
        uint16_t tmp2 = SoundRenderer::INVALID_HANDLE_ID;

        m_audioStreams.resize(m_handleCount, tmp2);
        m_videoTexture.resize(m_handleCount, tmp);
        m_decoders.resize(m_handleCount, nullptr);
    } else {
        id = m_freeHandles.back();
        m_freeHandles.pop_back();
    }

    return id;
}

void VideoManager::__Free(uint16_t id)
{
    m_decoders[id] = nullptr;
    m_videoTexture[id] = TextureManager::INVALID_HANDLE_ID;
    m_audioStreams[id] = SoundRenderer::INVALID_HANDLE_ID;

    m_freeHandles.push_back(id);
}

}