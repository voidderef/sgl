#pragma once

// we have to use the C api of fmod because their compiled .a file
// doesn't use mangled names and thus won't link with mingw
#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>

#include "ks/Object.h"
#include "ks/Settings.h"
#include "ks/ThreadLoop.h"

#include "VideoDecoder.h"

namespace sgl {

class SoundRenderer : public ks::Object
{
public:
    static const uint16_t INVALID_HANDLE_ID = 0xFFFF;

    SoundRenderer();

    ~SoundRenderer() override = default;

    void ApplySettings(const ks::Settings& settings);

    void Init();

    void SetMasterVolume(float volume);

    uint16_t CreateSound(const std::string& path, bool streamed = false);

    uint16_t CreateVideoSoundStream(VideoDecoder* videoDecoder);

    void DestorySound(uint16_t& id);

    void SetVolume(uint16_t id, float volume);

    void SetLoop(uint16_t id, int32_t loop);

    bool IsPlaying(uint16_t id);

    void Play(uint16_t id);

    void Stop(uint16_t id);

    void Pause(uint16_t id, bool pause = true);

    void Shutdown();

private:
    struct SoundState {
        FMOD_SOUND* m_sound;
        FMOD_CHANNEL* m_channel;
        int32_t m_loop;
        float m_volume;
    };

private:
    FMOD_SYSTEM* m_system;

    FMOD_OUTPUTTYPE m_driver;
    std::string m_device;
    float m_masterVolume;

    std::vector<SoundState> m_sounds;

    uint16_t m_handleCount;
    std::vector<uint16_t> m_freeHandles;

    void __ApplySettingsDriver(const ks::Settings& settings);

    void __FmodCreate();
    void __FmodLogVersion();
    void __FmodSetOutputType();
    void __FmodSetDriver();
    void __FmodSetSpeakerMode();
    void __FmodInit();

    uint16_t __Alloc();
    void __Free(uint16_t id);

    static inline float __NormVolume(float volume)
    {
        if (volume < 0.0f) {
            return 0.0f;
        } else if (volume >= 1.0f) {
            return 1.0f;
        } else {
            return volume;
        }
    }

    static FMOD_RESULT F_CALLBACK __VideoSoundReadCallback(FMOD_SOUND* sound, void* data, unsigned int datalen);
    static FMOD_RESULT F_CALLBACK __VideoSoundSetPosCallback(FMOD_SOUND* sound, int subsound, unsigned int position,
        FMOD_TIMEUNIT postype);
};

}
