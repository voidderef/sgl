#pragma once

#include "ks/FileSystem.h"

#include "SoundRenderer.h"

namespace sgl {

class SFXManager
{
public:
    enum Effect {
        e_EffectAttractSelect = 0,
        e_EffectAttractConfirm = 1,
        e_EffectCount = 2,
    };

public:
    SFXManager(ks::FileSystem* fs, SoundRenderer* refSoundRenderer, float sfxMasterVolume);

    ~SFXManager();

    void Play(Effect effect);

    bool IsPlaying(Effect effect);

private:
    static const std::string ms_fileAttractSelect;
    static const std::string ms_fileAttractConfirm;

private:
    SoundRenderer* m_refSoundRenderer;
    const float m_sfxMasterVolume;

    std::vector<uint16_t> m_effects;
};

}
