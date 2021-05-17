#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
};

#include "ks/ThreadLoop.h"
#include "ks/Timer.h"

namespace sgl {

class VideoDecoder : public ks::ThreadLoop
{
public:
    enum PixelFormat
    {
        e_PixelFormatRGBA,
        e_PixelFormatARGB,
        e_PixelFormatBGRA,
        e_PixelFormatABGR
    };

    enum AudioSampleFormat
    {
        e_AudioSampleFormatUnknown,
        e_AudioSampleFormatU8,
        e_AudioSampleFormatU16,
        e_AudioSampleFormatU32,
        e_AudioSampleFormatU64,
        e_AudioSampleFormatFloat,
        e_AudioSampleFormatDouble,
    };

    enum VideoFrameState
    {
        e_VideoFrameStateNA = 0,
        e_VideoFrameStateQueueDelay = 1,
        e_VideoFrameStateOnTime = 2,
        e_VideoFrameStateSkip = 3,
    };

    explicit VideoDecoder(const std::string& path, bool decodeAudio = true);

    ~VideoDecoder() override;

    uint32_t GetVideoWidth() const {
        return (uint32_t) m_videoCodecCtx->width;
    }

    uint32_t GetVideoHeight() const {
        return (uint32_t) m_videoCodecCtx->height;
    }

    double GetVideoFramerate() const {
        return av_q2d(m_videoCodecCtx->framerate);
    }

    uint64_t GetVideoBitrate() const {
        return (uint64_t) m_videoCodecCtx->bit_rate;
    }

    uint64_t GetAudioBitrate() const {
        return static_cast<uint64_t>(m_audioCodecCtx ? m_audioCodecCtx->bit_rate : 0);
    }

    uint16_t GetAudioChannels() const {
        return static_cast<uint16_t>(m_audioCodecCtx ? m_audioCodecCtx->channels : 0);
    }

    uint32_t GetAudioSampleRate() const {
        return static_cast<uint32_t>(m_audioCodecCtx ? m_audioCodecCtx->sample_rate : 0);
    }

    uint32_t GetAudioSampleFormatSize() const {
        if (!m_audioCodecCtx) {
            return 0;
        }

        switch (m_audioCodecCtx->sample_fmt) {
            case AV_SAMPLE_FMT_U8:
            case AV_SAMPLE_FMT_U8P:
                return 1;

            case AV_SAMPLE_FMT_S16:
            case AV_SAMPLE_FMT_S16P:
                return 2;

            case AV_SAMPLE_FMT_S32:
            case AV_SAMPLE_FMT_S32P:
            case AV_SAMPLE_FMT_FLT:
            case AV_SAMPLE_FMT_FLTP:
                return 4;

            case AV_SAMPLE_FMT_DBL:
            case AV_SAMPLE_FMT_DBLP:
            case AV_SAMPLE_FMT_S64:
            case AV_SAMPLE_FMT_S64P:
                return 8;

            default:
                return 0;
        }
    }

    uint32_t GetAudioSampleSize() const {
        return GetAudioSampleFormatSize() * GetAudioChannels();
    }

    AudioSampleFormat GetAudioSampleFormat() const {
        if (!m_audioCodecCtx) {
            return e_AudioSampleFormatUnknown;
        }

        switch (m_audioCodecCtx->sample_fmt) {
            case AV_SAMPLE_FMT_U8:
            case AV_SAMPLE_FMT_U8P:
                return e_AudioSampleFormatU8;

            case AV_SAMPLE_FMT_S16:
            case AV_SAMPLE_FMT_S16P:
                return e_AudioSampleFormatU16;

            case AV_SAMPLE_FMT_S32:
            case AV_SAMPLE_FMT_S32P:
                return e_AudioSampleFormatU32;

            case AV_SAMPLE_FMT_FLT:
            case AV_SAMPLE_FMT_FLTP:
                return e_AudioSampleFormatFloat;

            case AV_SAMPLE_FMT_DBL:
            case AV_SAMPLE_FMT_DBLP:
                return e_AudioSampleFormatDouble;

            case AV_SAMPLE_FMT_S64:
            case AV_SAMPLE_FMT_S64P:
                return e_AudioSampleFormatU64;

            default:
                return e_AudioSampleFormatUnknown;
        }
    }

    bool WithAudio() {
        return m_audioStreamIdx != -1;
    }

    const std::string& GetPath() const {
        return m_path;
    }

    void SetLoopStartPos(uint32_t framePos);

    void SetLoop(int32_t loopCount);

    bool IsEof() const;

    void Start() override;

    void Stop() override;

    void Pause(bool pause);

    void Reset();

    VideoFrameState GetNextVideoFrame(void* mem, uint32_t width, uint32_t height, PixelFormat pf,
        double targetDisplayFPS);

    bool IsAudioDataAvailable() const;

    // this always copies audio data with interleaved samples
    // stream based: tries to fill up the provided buffer if possible
    size_t CopyAudioData(void* buffer, size_t len);

    friend std::ostream& operator<<(std::ostream& os, const VideoDecoder& o)
    {
        return os << o.m_path << " video: " << o.GetVideoWidth() << "x" << o.GetVideoHeight() << ", bitrate " <<
            o.GetVideoBitrate() << " audio: " << o.GetAudioChannels() << " chan, " << o.GetAudioBitrate() <<
            " bitrate, " << o.GetAudioSampleRate() << " khz, sample format " << o.GetAudioSampleFormat();
    }

protected:
    void _RunLoop() override;

private:
    enum DecodingState {
        e_DecodingStateReadFrame = 0,
        e_DecodingStateFillBuffer = 1,
        e_DecodingStateDrainVideoBuffers = 2,
        e_DecodingStateDrainAudioBuffers = 3,
        e_DecodingStateCheckLoop = 4,
        e_DecodingStateEOF = 5,
        e_DecodingStatePaused = 6,
    };

    enum DecodingCmd {
        e_DecodingCmdNone = -1,
        e_DecodingCmdRun = 0,
        e_DecodingCmdPause = 1,
        e_DecodingCmdReset = 2,
        e_DecodingCmdStop = 3,
    };

private:
    // lock free 1:1 producer/consumer queue
    class FrameFifo
    {
    public:
        explicit FrameFifo(size_t size);
        ~FrameFifo();

        size_t GetSize() const {
            return m_size;
        }

        bool IsEmpty() const;

        size_t GetBuffered() const;

        AVFrame* GetCurBack() const;

        void PushBack();

        AVFrame* GetCurFront() const;

        void PopFront();

        void Reset();

        size_t GetCount() const {
            size_t front = m_front.load(std::memory_order_relaxed) % m_size;
            size_t back = m_back.load(std::memory_order_relaxed) % m_size;

            if (back >= front) {
                return back - front;
            } else {
                return (m_size - front) + back;
            }
        }

    private:
        const size_t m_size;
        std::vector<AVFrame*> m_buffer;
        std::atomic<size_t> m_front;
        std::atomic<size_t> m_back;
    };

private:
    const std::string m_path;
    const bool m_decodeAudio;

    std::atomic<uint32_t> m_loopFramePos;
    std::atomic<int32_t> m_loopCount;
    std::atomic<int32_t> m_loopState;

    std::atomic<DecodingState> m_decodingState;
    std::atomic<DecodingCmd> m_decodingCmd;

    AVFormatContext* m_avFormatCtx;

    uint32_t m_videoStreamIdx;
    uint32_t m_audioStreamIdx;

    AVCodecParameters* m_videoStreamParams;
    AVCodecParameters* m_audioStreamParams;

    AVCodec* m_videoCodec;
    AVCodec* m_audioCodec;

    AVCodecContext* m_videoCodecCtx;
    AVCodecContext* m_audioCodecCtx;

    FrameFifo m_videoFrames;
    AVFrame* m_videoResizedFrame;
    size_t m_videoFramesPos;

    FrameFifo m_audioFrames;
    size_t m_currentAudioFrameOffset;
    bool m_audioSamplesPlanar;
    double m_audioSampleTimeSec;
    double m_audioTimeSecPos;

    AVPacket* m_avPacket;

private:
    void __ParseContainer();
    void __ParseStreams();
    void __OpenVideoStream();
    void __OpenAudioStream();

    void __Cleanup();
    void __ResetVideo(bool clearQueues, uint32_t framePos);
};

}
