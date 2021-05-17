#include "SFXManager.h"

#include "ks/Logger.hpp"

namespace sgl {

const std::string SFXManager::ms_fileAttractSelect = "select.wav";
const std::string SFXManager::ms_fileAttractConfirm = "confirm.wav";

SFXManager::SFXManager(ks::FileSystem* refFs, SoundRenderer* refSoundRenderer, float sfxMasterVolume) :
    m_refSoundRenderer(refSoundRenderer),
    m_sfxMasterVolume(sfxMasterVolume),
    m_effects()
{
    uint16_t tmp = SoundRenderer::INVALID_HANDLE_ID;
    m_effects.resize(e_EffectCount, tmp);

    if (refFs->Exists(ms_fileAttractSelect)) {
        m_effects[e_EffectAttractSelect] = m_refSoundRenderer->CreateSound(
            refFs->GetAbsolutePath(ms_fileAttractSelect));
        m_refSoundRenderer->SetVolume(m_effects[e_EffectAttractSelect], m_sfxMasterVolume);
    } else {
        KS_LOG_WARN("Could not find select sound in %s", *refFs);
    }

    if (refFs->Exists(ms_fileAttractConfirm)) {
        m_effects[e_EffectAttractConfirm] = m_refSoundRenderer->CreateSound(
            refFs->GetAbsolutePath(ms_fileAttractConfirm));
        m_refSoundRenderer->SetVolume(m_effects[e_EffectAttractConfirm], m_sfxMasterVolume);
    } else {
        KS_LOG_WARN("Could not find confirm sound in %s", *refFs);
    }
}

SFXManager::~SFXManager()
{
    for (auto& it : m_effects) {
        if (it != SoundRenderer::INVALID_HANDLE_ID) {
            m_refSoundRenderer->DestorySound(it);
        }
    }
}

void SFXManager::Play(Effect effect)
{
    if (m_effects[effect] != SoundRenderer::INVALID_HANDLE_ID) {
        m_refSoundRenderer->Play(m_effects[effect]);
    }
}

bool SFXManager::IsPlaying(Effect effect)
{
    if (m_effects[effect] != SoundRenderer::INVALID_HANDLE_ID) {
        return m_refSoundRenderer->IsPlaying(m_effects[effect]);
    } else {
        return false;
    }
}

}