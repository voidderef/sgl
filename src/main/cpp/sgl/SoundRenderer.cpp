#include "sgl/SGLSettings.h"
#include "sgl/SoundException.h"
#include "sgl/SoundRenderer.h"

namespace sgl {

SoundRenderer::SoundRenderer() :
    m_system(nullptr),
    m_driver(FMOD_OUTPUTTYPE_AUTODETECT),
    m_device(""),
    m_masterVolume(1.0f),
    m_sounds(),
    m_handleCount(0),
    m_freeHandles()
{

}

void SoundRenderer::ApplySettings(const ks::Settings& settings)
{
    SetMasterVolume(settings.GetValue<float>(SGLSettings::ms_soundMasterVolume));
    __ApplySettingsDriver(settings);
    m_device = settings.GetValue<const std::string&>(SGLSettings::ms_soundDevice);
}

void SoundRenderer::Init()
{
    KS_LOG_INFO("Initialize");

    __FmodCreate();
    __FmodLogVersion();
    __FmodSetOutputType();
    __FmodSetDriver();
    __FmodSetSpeakerMode();
    __FmodInit();
}

void SoundRenderer::SetMasterVolume(float volume)
{
    m_masterVolume = __NormVolume(volume);
}

uint16_t SoundRenderer::CreateSound(const std::string& path, bool streamed)
{
    uint16_t id = __Alloc();

    KS_LOG_DEBUG("Creating sound (%d): %s %d", id, path, streamed);

    FMOD_RESULT res = FMOD_System_CreateSound(m_system, path.c_str(), streamed ? FMOD_CREATESTREAM : FMOD_DEFAULT,
        nullptr, &m_sounds[id].m_sound);

    if (res != FMOD_OK) {
        __Free(id);
        KS_LOG_ERROR("Creating sound of file '%s' failed: %s", path, FMOD_ErrorString(res));
        return SoundRenderer::INVALID_HANDLE_ID;
    }

    m_sounds[id].m_channel = nullptr;
    m_sounds[id].m_loop = 0;
    m_sounds[id].m_volume = 1.0f;

    return id;
}

uint16_t SoundRenderer::CreateVideoSoundStream(VideoDecoder* videoDecoder)
{
    uint16_t id = __Alloc();

    KS_LOG_DEBUG("Creating video sound stream (%d): %s", id, *videoDecoder);

    FMOD_CREATESOUNDEXINFO exinfo = {};
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    // use a huge buffer to avoid blocking the decoding thread
    // if we don't grab the decoded audio data fast enough, decoding of the video
    // will stutter resulting in video render stutter
    // 1024 seems to be a fine size which doesn't cause any stuttering
    exinfo.decodebuffersize = static_cast<unsigned int>(
        256 * videoDecoder->GetAudioChannels() * videoDecoder->GetAudioSampleFormatSize());                                       /* Chunk size of stream update in samples.  This will be the amount of data passed to the user callback. */
    // don't set length to 0. we set the stream to loop so file size doesn't matter. set this to buffer size
    exinfo.length = exinfo.decodebuffersize;
    exinfo.numchannels = videoDecoder->GetAudioChannels();
    exinfo.defaultfrequency = videoDecoder->GetAudioSampleRate();

    switch (videoDecoder->GetAudioSampleFormat()) {
        case VideoDecoder::e_AudioSampleFormatU8:
            exinfo.format = FMOD_SOUND_FORMAT_PCM8;
            break;

        case VideoDecoder::e_AudioSampleFormatU16:
            exinfo.format = FMOD_SOUND_FORMAT_PCM16;
            break;

        case VideoDecoder::e_AudioSampleFormatU32:
            exinfo.format = FMOD_SOUND_FORMAT_PCM32;
            break;

        case VideoDecoder::e_AudioSampleFormatFloat:
            exinfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;
            break;

        case VideoDecoder::e_AudioSampleFormatUnknown:
            throw SoundException("Unknown audio sample format for video: %s", videoDecoder->GetPath());

        case VideoDecoder::e_AudioSampleFormatU64:
        case VideoDecoder::e_AudioSampleFormatDouble:
            throw SoundException("Unsupported audio sample format %d for video: %s",
                videoDecoder->GetAudioSampleFormat(), videoDecoder->GetPath());
    }

    exinfo.pcmreadcallback = __VideoSoundReadCallback;
    exinfo.pcmsetposcallback = __VideoSoundSetPosCallback;
    exinfo.userdata = videoDecoder;

    KS_LOG_DEBUG("Creating sound stream for video: %d %d %d %d", exinfo.decodebuffersize, exinfo.numchannels,
        exinfo.defaultfrequency, exinfo.format);

    FMOD_RESULT res = FMOD_System_CreateStream(m_system, nullptr, FMOD_OPENUSER, &exinfo, &m_sounds[id].m_sound);

    if (res != FMOD_OK) {
        KS_LOG_ERROR("Creating sound stream for video failed: %s", FMOD_ErrorString(res));
    }

    m_sounds[id].m_channel = nullptr;
    // loop endless to keep streaming
    m_sounds[id].m_loop = -1;
    m_sounds[id].m_volume = 1.0f;

    return id;
}

void SoundRenderer::DestorySound(uint16_t& id)
{
    KS_LOG_DEBUG("Destroying sound %d", id);

    if (m_sounds[id].m_channel) {
        FMOD_Channel_Stop(m_sounds[id].m_channel);
        m_sounds[id].m_channel = nullptr;
    }

    if (m_sounds[id].m_sound) {
        FMOD_Sound_Release(m_sounds[id].m_sound);
        m_sounds[id].m_sound = nullptr;
    }

    __Free(id);
    id = INVALID_HANDLE_ID;
}

void SoundRenderer::SetVolume(uint16_t id, float volume)
{
    m_sounds[id].m_volume = __NormVolume(volume);

    if (m_sounds[id].m_channel) {
        FMOD_Channel_SetVolume(m_sounds[id].m_channel, m_masterVolume * volume);
    }
}

void SoundRenderer::SetLoop(uint16_t id, int32_t loop)
{
    m_sounds[id].m_loop = loop;

    if (m_sounds[id].m_channel) {
        FMOD_Channel_SetLoopCount(m_sounds[id].m_channel, loop);
    }
}

bool SoundRenderer::IsPlaying(uint16_t id)
{
    if (m_sounds[id].m_channel) {
        FMOD_BOOL playing;

        FMOD_Channel_IsPlaying(m_sounds[id].m_channel, &playing);

        return static_cast<bool>(playing);
    }

    return false;
}

void SoundRenderer::Play(uint16_t id)
{
    if (m_sounds[id].m_channel) {
        FMOD_Channel_Stop(m_sounds[id].m_channel);
    }

    FMOD_RESULT res = FMOD_System_PlaySound(m_system, FMOD_CHANNEL_FREE, m_sounds[id].m_sound, true,
        &m_sounds[id].m_channel);

    if (res != FMOD_OK) {
        throw SoundException("Playing sound failed: %s", FMOD_ErrorString(res));
    }

    FMOD_Channel_SetMode(m_sounds[id].m_channel, FMOD_LOOP_NORMAL);
    FMOD_Channel_SetPosition(m_sounds[id].m_channel, 0, FMOD_TIMEUNIT_MS);
    FMOD_Channel_SetLoopCount(m_sounds[id].m_channel, m_sounds[id].m_loop);
    FMOD_Channel_SetVolume(m_sounds[id].m_channel, m_masterVolume * m_sounds[id].m_volume);
    FMOD_Channel_SetPaused(m_sounds[id].m_channel, false);
}

void SoundRenderer::Stop(uint16_t id)
{
    if (m_sounds[id].m_channel) {
        FMOD_Channel_Stop(m_sounds[id].m_channel);
        m_sounds[id].m_channel = nullptr;
    }
}

void SoundRenderer::Pause(uint16_t id, bool pause)
{
    if (m_sounds[id].m_channel) {
        FMOD_Channel_SetPaused(m_sounds[id].m_channel, pause);
    }
}

void SoundRenderer::Shutdown()
{
    KS_LOG_INFO("Shutdown");

    // stop and cleanup all sounds
    for (uint16_t i = 0; i < m_handleCount; i++) {
        FMOD_Channel_Stop(m_sounds[i].m_channel);
        FMOD_Sound_Release(m_sounds[i].m_sound);
        __Free(i);
    }

    FMOD_System_Release(m_system);
    m_system = nullptr;
}


void SoundRenderer::__ApplySettingsDriver(const ks::Settings& settings)
{
    std::string tmp;

    tmp = settings.GetValue<const std::string&>(SGLSettings::ms_soundDriver);

    if (tmp == "auto" || tmp.empty()) {
        m_driver = FMOD_OUTPUTTYPE_AUTODETECT;
        tmp = "auto";
    } else if (tmp == "alsa") {
        m_driver = FMOD_OUTPUTTYPE_ALSA;
    } else if (tmp == "pulse") {
        m_driver = FMOD_OUTPUTTYPE_PULSEAUDIO;
    } else if (tmp == "oss") {
        m_driver = FMOD_OUTPUTTYPE_OSS;
    } else if (tmp == "esd") {
        m_driver = FMOD_OUTPUTTYPE_ESD;
    } else if (tmp == "dsound") {
        m_driver = FMOD_OUTPUTTYPE_DSOUND;
    } else if (tmp == "winmm") {
        m_driver = FMOD_OUTPUTTYPE_WINMM;
    } else if (tmp == "asio") {
        m_driver = FMOD_OUTPUTTYPE_ASIO;
    } else if (tmp == "wsapi") {
        m_driver = FMOD_OUTPUTTYPE_WASAPI;
    } else {
        KS_LOG_ERROR(
            "Invalid sound driver specified %s. Fallback to 'auto'\n"
            "Autodetect/default: auto\n"
            "Available on Linux: alsa, pulse, oss, esd\n"
            "Available on Windows: dsound, winmm, asio, wsapi",
            tmp);
        m_driver = FMOD_OUTPUTTYPE_AUTODETECT;
        tmp = "auto";
    }

    KS_LOG_INFO("Driver selected: %s", tmp);
}

void SoundRenderer::__FmodCreate()
{
    FMOD_RESULT res = FMOD_System_Create(&m_system);

    if (res != FMOD_OK) {
        throw SoundException("Creating FMOD system failed: %s", FMOD_ErrorString(res));
    }
}

void SoundRenderer::__FmodLogVersion()
{
    FMOD_RESULT res;
    unsigned int version;

    res = FMOD_System_GetVersion(m_system, &version);

    if (res != FMOD_OK) {
        throw SoundException("FMOD_System_GetVersion failed %d: %s", res, FMOD_ErrorString((FMOD_RESULT) res));
    }

    KS_LOG_INFO("FMODEX version: %d.%d.%d", (version >> 16) & 0xFFFF, (version >> 8) & 0xFF, version & 0xFF);
}

void SoundRenderer::__FmodSetOutputType()
{
    FMOD_RESULT res;
    FMOD_OUTPUTTYPE outputType;

    res = FMOD_System_SetOutput(m_system, m_driver);

    if (res != FMOD_OK) {
        throw SoundException("FMOD_System_SetOutput failed %d: %s", res, FMOD_ErrorString((FMOD_RESULT) res));
    }

    res = FMOD_System_GetOutput(m_system, &outputType);

    if (res != FMOD_OK) {
        throw SoundException("FMOD_System_GetOutput failed %d: %s", res, FMOD_ErrorString((FMOD_RESULT) res));
    }

    KS_LOG_DEBUG("Output type: %d", outputType);
}

void SoundRenderer::__FmodSetDriver()
{
    FMOD_RESULT res;
    int num_drivers;

    char buffer[4096];
    FMOD_GUID guid;
    int selected_driver = -1;

    res = FMOD_System_GetNumDrivers(m_system, &num_drivers);

    if (res != FMOD_OK) {
        throw SoundException("FMOD_System_GetNumDrivers failed %d: %s", res, FMOD_ErrorString((FMOD_RESULT) res));
    }

    KS_LOG_INFO("Num available devices %d", num_drivers);

    for (int i = 0; i < num_drivers; i++) {
        res = FMOD_System_GetDriverInfo(m_system, i, buffer, sizeof(buffer), &guid);

        if (res != FMOD_OK) {
            throw SoundException("FMOD_System_GetDriverInfo failed %d: %s", res, FMOD_ErrorString((FMOD_RESULT) res));
        }

        KS_LOG_DEBUG("Device %d: %s", i, buffer);

        if (!m_device.empty() && m_device == std::string(buffer)) {
            selected_driver = i;
        }
    }

    if (selected_driver == -1) {
        KS_LOG_INFO("Selecting default device (0) because either the selected driver was not found or no driver was "
            "selected");
        selected_driver = 0;
    } else {
        KS_LOG_INFO("Found selected driver '%s', id %d", m_device, selected_driver);
    }

    res = FMOD_System_SetDriver(m_system, selected_driver);

    if (res != FMOD_OK) {
        throw SoundException("FMOD_System_SetDriver failed %d: %s", res, FMOD_ErrorString((FMOD_RESULT) res));
    }
}

void SoundRenderer::__FmodSetSpeakerMode()
{
    FMOD_RESULT res = FMOD_System_SetSpeakerMode(m_system, FMOD_SPEAKERMODE_STEREO);

    if (res != FMOD_OK) {
        throw SoundException("FMOD_System_SetSpeakerMode failed %d: %s", res, FMOD_ErrorString((FMOD_RESULT) res));
    }
}

void SoundRenderer::__FmodInit()
{
    // fmod runs its own audio thread for driving the output
    FMOD_RESULT res = FMOD_System_Init(m_system, 32, FMOD_INIT_NORMAL, nullptr);

    if (res != FMOD_OK) {
        throw SoundException("Initializing FMOD system failed: %s", FMOD_ErrorString(res));
    }
}

uint16_t SoundRenderer::__Alloc()
{
    uint16_t id;

    if (m_handleCount + 1 == INVALID_HANDLE_ID) {
        throw SoundException("Out of handles");
    }

    if (m_freeHandles.empty()) {
        id = m_handleCount++;

        m_sounds.resize(m_handleCount);
    } else {
        id = m_freeHandles.back();
        m_freeHandles.pop_back();
    }

    return id;
}

void SoundRenderer::__Free(uint16_t id)
{
    m_sounds[id].m_sound = nullptr;
    m_sounds[id].m_channel = nullptr;
    m_sounds[id].m_loop = 0;
    m_sounds[id].m_volume = 1.0f;

    m_freeHandles.push_back(id);
}

FMOD_RESULT F_CALLBACK SoundRenderer::__VideoSoundReadCallback(FMOD_SOUND* sound, void* data, unsigned int datalen)
{
    VideoDecoder* videoDecoder;
    FMOD_Sound_GetUserData(sound, (void**) &videoDecoder);

    // don't wait for any data if non is available (e.g. eof or stopped)
    if (videoDecoder->IsAudioDataAvailable()) {
        // we have to fill up the whole buffer. it seems like blocking here is fine if no data is available
        // even the documentation doesn't state that. otherwise, if we don't fill up the provided buffer, we most
        // likely get stuttering during playback because there is no way to tell fmod that the buffer isn't filled
        // completely
        size_t dataPos = 0;

        while (datalen > 0) {
            size_t read = videoDecoder->CopyAudioData((void*) ((uintptr_t) data + dataPos), datalen);

            // no data currently available
            if (read == 0) {
                break;
            }

            datalen -= read;
            dataPos += read;
        }

        // fill with silence, this should only happen for the remaining data of the very last frame ever yielded
        // by CopyAudioData. otherwise, we get stuttering which can be solved by adjusting the size of the frame buffer
        // or ensuring that enough audio data is pre-buffered by the video decoder before starting playback
        if (datalen > 0) {
            memset((void*) ((uintptr_t) data + dataPos), 0, datalen);
        }
    } else {
        // fill with silence
        memset(data, 0, datalen);
    }

    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK SoundRenderer::__VideoSoundSetPosCallback(FMOD_SOUND* sound, int subsound, unsigned int position,
        FMOD_TIMEUNIT postype)
{
    // no seeking supported
    return FMOD_OK;
}

}