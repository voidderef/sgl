#include "VideoDecoder.h"

#include "VideoDecoderException.h"

namespace sgl {

VideoDecoder::VideoDecoder(const std::string& path, bool decodeAudio) :
    ThreadLoop("VideoDecoder(" + path + ")"),
    m_path(path),
    m_decodeAudio(decodeAudio),
    m_loopFramePos(0),
    m_loopCount(0),
    m_loopState(0),
    m_decodingState(e_DecodingStateReadFrame),
    m_decodingCmd(e_DecodingCmdNone),
    m_avFormatCtx(nullptr),
    m_videoStreamIdx((uint32_t) -1),
    m_audioStreamIdx((uint32_t) -1),
    m_videoStreamParams(nullptr),
    m_audioStreamParams(nullptr),
    m_videoCodec(nullptr),
    m_audioCodec(nullptr),
    m_videoCodecCtx(nullptr),
    m_audioCodecCtx(nullptr),
    m_videoFrames(60),
    m_videoResizedFrame(nullptr),
    m_videoFramesPos(0),
    m_audioFrames(60),
    m_currentAudioFrameOffset(0),
    m_audioSamplesPlanar(false),
    m_audioSampleTimeSec(0.0),
    m_audioTimeSecPos(0.0),
    m_avPacket(nullptr)
{
    // register all codecs
    static bool initOnce = true;

    if (initOnce) {
        av_register_all();

        // disable all logging, remove to renable for debugging purpose
        av_log_set_level(AV_LOG_QUIET);
        initOnce = false;
    }

    try {
        __ParseContainer();
        __ParseStreams();
        __OpenVideoStream();
        __OpenAudioStream();

        m_avPacket = av_packet_alloc();

        if (!m_avPacket) {
            throw VideoDecoderException(this, "Allocating av packet failed");
        }

        m_videoResizedFrame = av_frame_alloc();

        if (!m_videoResizedFrame) {
            throw VideoDecoderException(this, "Allocating av frame video resized failed");
        }

    } catch (...) {
        // cleanup all resources on failure
        __Cleanup();

        throw;
    }

    if (m_audioStreamIdx != -1) {
        m_audioSampleTimeSec = 1.0 / GetAudioSampleRate();
    }
}

VideoDecoder::~VideoDecoder()
{
    __Cleanup();
}

void VideoDecoder::SetLoopStartPos(uint32_t framePos)
{
    m_loopFramePos.store(framePos, std::memory_order_relaxed);
}

void VideoDecoder::SetLoop(int32_t loopCount)
{
    m_loopCount.store(loopCount, std::memory_order_relaxed);
    m_loopState.store(loopCount, std::memory_order_relaxed);
}

bool VideoDecoder::IsEof() const
{
    if (!IsRunning()) {
        return true;
    }

    return m_decodingState.load(std::memory_order_relaxed) == e_DecodingStateEOF && m_audioFrames.IsEmpty() &&
        m_videoFrames.IsEmpty();
}

void VideoDecoder::Start()
{
    m_decodingCmd.store(e_DecodingCmdRun, std::memory_order_relaxed);

    __ResetVideo(true, 0);

    m_loopState.store(m_loopCount.load(std::memory_order_relaxed), std::memory_order_relaxed);

    // Start the decoding thread
    ks::ThreadLoop::Start();

    // wait a moment until the thread is running and has its buffers filled
    DecodingState decodingState;

    do {
        decodingState = m_decodingState.load(std::memory_order_relaxed);
        ks::Thread::SleepMs(1);
    } while (decodingState != e_DecodingStateReadFrame &&
        decodingState != e_DecodingStateFillBuffer &&
        decodingState != e_DecodingStateDrainVideoBuffers &&
        decodingState != e_DecodingStateDrainAudioBuffers);

    // we assume the video has more than a few frames at least
    // warm up queue, wait a moment
    while (m_videoFrames.GetCount() < 10 && m_audioFrames.GetCount() < 10) {
        ks::Thread::SleepMs(1);
    }

    m_audioTimeSecPos = 0;
    m_videoFramesPos = 0;
    m_currentAudioFrameOffset = 0;
}

void VideoDecoder::Stop()
{
    if (!IsRunning()) {
        return;
    }

    // wait until previous command is processed
    DecodingCmd cmd = e_DecodingCmdNone;

    while (!m_decodingCmd.compare_exchange_strong(cmd, e_DecodingCmdStop,
            std::memory_order_relaxed)) {
        cmd = e_DecodingCmdNone;
        ks::Thread::Yield();
    }

    ks::ThreadLoop::Stop();
}

void VideoDecoder::Pause(bool pause)
{
    if (!IsRunning()) {
        return;
    }

    // wait until previous command is processed
    DecodingCmd cmd = e_DecodingCmdNone;
    DecodingCmd cmdToSet = pause ? e_DecodingCmdPause : e_DecodingCmdRun;

    while (!m_decodingCmd.compare_exchange_strong(cmd, cmdToSet,
            std::memory_order_relaxed)) {
        cmd = e_DecodingCmdNone;
        ks::Thread::Yield();
    }
}

void VideoDecoder::Reset()
{
    if (!IsRunning()) {
        return;
    }

    m_loopState.store(m_loopCount.load(std::memory_order_relaxed), std::memory_order_relaxed);

    // wait until previous command is processed
    DecodingCmd cmd = e_DecodingCmdNone;

    while (!m_decodingCmd.compare_exchange_strong(cmd, e_DecodingCmdReset,
            std::memory_order_relaxed)) {
        cmd = e_DecodingCmdNone;
        ks::Thread::Yield();
    }

    m_audioTimeSecPos = 0;
    m_videoFramesPos = 0;
    m_currentAudioFrameOffset = 0;
}

VideoDecoder::VideoFrameState VideoDecoder::GetNextVideoFrame(void* mem, uint32_t width, uint32_t height,
        PixelFormat pf, double targetDisplayFPS)
{
    if (!IsRunning()) {
        return e_VideoFrameStateNA;
    }

    AVPixelFormat targetPf;
    AVFrame* curFrame = m_videoFrames.GetCurFront();

    if (!curFrame) {
        return e_VideoFrameStateQueueDelay;
    }

    // sync video to audio
    double targetFrameWindow = 1.0 / targetDisplayFPS;
    double targetFrameWindowThreshold = targetFrameWindow * 0.8;
    double curTime = m_audioTimeSecPos;
    double displayTime = m_videoFramesPos / GetVideoFramerate();

    m_videoFramesPos++;

    if (curTime - displayTime < targetFrameWindowThreshold) {
        // on time, continue
    } else {
        // too late, throw away
        m_videoFrames.PopFront();
        return e_VideoFrameStateSkip;
    }

    switch (pf) {
        case e_PixelFormatRGBA:
            targetPf = AV_PIX_FMT_RGBA;
            break;

        case e_PixelFormatARGB:
            targetPf = AV_PIX_FMT_ARGB;
            break;

        case e_PixelFormatBGRA:
            targetPf = AV_PIX_FMT_BGRA;
            break;

        case e_PixelFormatABGR:
            targetPf = AV_PIX_FMT_ABGR;
            break;

        default:
            targetPf = AV_PIX_FMT_RGBA;
            break;
    }

    av_image_fill_arrays(m_videoResizedFrame->data,
        m_videoResizedFrame->linesize, (const uint8_t *) mem, targetPf, width,
        height, 1);

    // create context for scaling using the current video frame
    SwsContext* swsCtx = sws_getContext(curFrame->width, curFrame->height,
        AVPixelFormat(curFrame->format),
        width, height, targetPf, SWS_BICUBIC, nullptr, nullptr, nullptr);

    if (!swsCtx) {
        throw VideoDecoderException(this, "Can't get scale context");
    }

    // scale the frame to the buffer directly
    sws_scale(swsCtx, (const uint8_t *const *) curFrame->data,
        curFrame->linesize, 0, curFrame->height, m_videoResizedFrame->data,
        m_videoResizedFrame->linesize);

    sws_freeContext(swsCtx);

    m_videoFrames.PopFront();

    return e_VideoFrameStateOnTime;
}

bool VideoDecoder::IsAudioDataAvailable() const
{
    if (!IsRunning()) {
        return false;
    }

    return static_cast<bool>(m_audioFrames.GetCurFront());
}

size_t VideoDecoder::CopyAudioData(void* buffer, size_t len)
{
    if (!IsRunning()) {
        return 0;
    }

    // assert audio buffer size
    if (len % GetAudioSampleSize()) {
        throw VideoDecoderException("Size of audio buffer must be a multiple of the sample size: %d % %d != 0",
            len, GetAudioSampleSize());
    }

    size_t bufferPos = 0;

    // create an audio stream from the decoded frames by coping data until no frames are left or buffer is full
    while (true) {
        AVFrame* curFrame = m_audioFrames.GetCurFront();

        if (!curFrame) {
            m_audioTimeSecPos += ((double) bufferPos / GetAudioSampleSize()) * m_audioSampleTimeSec;
            return bufferPos;
        }

        // we always return interleaved audio, interleave the samples from the separated channels if stored planar
        if (m_audioSamplesPlanar) {
            // for planar audio, the linesize is the total size of ALL channels together...srsly?
            // the doc doesn't tell you that clearly, just:
            // "For audio, only linesize[0] may be set. For planar audio, each channel plane must be the same size."
            if (m_currentAudioFrameOffset >= curFrame->linesize[0] / GetAudioChannels()) {
                // move to next frame
                m_currentAudioFrameOffset = 0;
                m_audioFrames.PopFront();
                continue;
            }

            // figure out how many interleaved samples we can store in the remaining buffer
            size_t numInterleavedSamples = len / GetAudioSampleSize();
            size_t bytesCopied = 0;

            auto buffer2 = static_cast<uint8_t*>(buffer);
            while (numInterleavedSamples > 0 && m_currentAudioFrameOffset < curFrame->linesize[0]) {
                // interleave samples of each channel
                for (uint8_t c = 0; c < GetAudioChannels(); c++) {
                    for (uint8_t s = 0; s < GetAudioSampleFormatSize(); s++) {
                        buffer2[bufferPos++] = curFrame->extended_data[c][m_currentAudioFrameOffset + s];
                        bytesCopied++;
                    }
                }

                numInterleavedSamples--;
                m_currentAudioFrameOffset += GetAudioSampleFormatSize();
            }

            // bytes copied
            len -= bytesCopied;

            // buffer full
            if (numInterleavedSamples == 0) {
                m_audioTimeSecPos += ((double) bufferPos / GetAudioSampleSize()) * m_audioSampleTimeSec;
                return bufferPos;
            }

            // still space left in buffer, continue with next frame
        } else {
            // total size of data in bytes for _ALL_ channels together (interleaved data)
            if (m_currentAudioFrameOffset >= curFrame->linesize[0]) {
                // move to next frame
                m_currentAudioFrameOffset = 0;
                m_audioFrames.PopFront();
                continue;
            }

            size_t bytesToCopy = len - bufferPos;

            if (bytesToCopy > curFrame->linesize[0] - m_currentAudioFrameOffset) {
                bytesToCopy = curFrame->linesize[0] - m_currentAudioFrameOffset;
            }

            auto buffer2 = static_cast<uint8_t*>(buffer);
            memcpy(&buffer2[bufferPos], &curFrame->extended_data[0][m_currentAudioFrameOffset], bytesToCopy);

            m_currentAudioFrameOffset += bytesToCopy;
            bufferPos += bytesToCopy;

            if (bufferPos == len) {
                m_audioTimeSecPos += ((double) bufferPos / GetAudioSampleSize()) * m_audioSampleTimeSec;
                return bufferPos;
            }
        }
    }
}

void VideoDecoder::_RunLoop()
{
    int ret;

    // process outstanding cmds
    switch (m_decodingCmd.exchange(e_DecodingCmdNone, std::memory_order_relaxed)) {
        case e_DecodingCmdNone:
        case e_DecodingCmdRun: {
            break;
        }

        case e_DecodingCmdPause: {
            ks::Thread::SleepMs(1);
            return;
        }

        case e_DecodingCmdReset: {
            __ResetVideo(true, 0);

            return;
        }

        case e_DecodingCmdStop: {
            // pause decoding for thread to shut down
            m_decodingState.store(e_DecodingStatePaused, std::memory_order_relaxed);
            return;
        }
    }

    // state machine for decoding
    switch (m_decodingState.load(std::memory_order_relaxed)) {
        case e_DecodingStateReadFrame: {
            // package processed or dropped
            av_packet_unref(m_avPacket);

            // read next frame from video file
            ret = av_read_frame(m_avFormatCtx, m_avPacket);

            if (ret < 0) {
                if (ret == AVERROR_EOF) {
                    m_decodingState.store(e_DecodingStateCheckLoop, std::memory_order_relaxed);
                    break;
                } else {
                    throw VideoDecoderException(this, "Reading next frame failed", ret);
                }
            }

            m_decodingState = e_DecodingStateFillBuffer;

            break;
        }

        case e_DecodingStateFillBuffer: {
            AVCodecContext* codecCtx;

            // decode audio/video frame
            if (m_avPacket->stream_index == m_videoStreamIdx) {
                codecCtx = m_videoCodecCtx;
            } else if (m_avPacket->stream_index == m_audioStreamIdx) {
                if (m_decodeAudio) {
                    codecCtx = m_audioCodecCtx;
                } else {
                    // throw away read audio frames
                    codecCtx = nullptr;
                }
            } else {
                codecCtx = nullptr;
            }

            if (codecCtx) {
                // this will always process the whole packet (according to doc)
                ret = avcodec_send_packet(codecCtx, m_avPacket);

                if (ret < 0) {
                    // internal input buffer is full, keep packet for later
                    if (ret == AVERROR(EAGAIN)) {
                        if (m_avPacket->stream_index == m_videoStreamIdx) {
                            m_decodingState.store(e_DecodingStateDrainVideoBuffers, std::memory_order_relaxed);
                        } else if (m_avPacket->stream_index == m_audioStreamIdx) {
                            m_decodingState.store(e_DecodingStateDrainAudioBuffers, std::memory_order_relaxed);
                        } else {
                            throw VideoDecoderException(this, "Invalid state, should not happen");
                        }

                        // TODO keep package for later?

                        // keep unprocessed package

                        break;
                    } else if (ret == AVERROR_EOF) {
                        // drop package
                        av_packet_unref(m_avPacket);
                        m_decodingState.store(e_DecodingStateCheckLoop, std::memory_order_relaxed);
                        break;
                    } else {
                        throw VideoDecoderException(this, "Send package to decode audio/video frame failed", ret);
                    }
                }
            }

            // decode audio/video frame
            if (m_avPacket->stream_index == m_videoStreamIdx) {
                m_decodingState.store(e_DecodingStateDrainVideoBuffers, std::memory_order_relaxed);
            } else if (m_avPacket->stream_index == m_audioStreamIdx) {
                m_decodingState.store(e_DecodingStateDrainAudioBuffers, std::memory_order_relaxed);
            } else {
                m_decodingState.store(e_DecodingStateReadFrame, std::memory_order_relaxed);
            }

            break;
        }

        case e_DecodingStateDrainVideoBuffers: {
            // check if there is space in the queue
            AVFrame* curFrame = m_videoFrames.GetCurBack();

            // no place in queue, retry
            if (!curFrame) {
                break;
            }

            ret = avcodec_receive_frame(m_videoCodecCtx, curFrame);

            if (ret < 0) {
                if (ret == AVERROR(EAGAIN)) {
                    // internal output buffer empty, read next frame
                } else if (ret == AVERROR_EOF) {
                    // fully flushed, no more frames to process
                    m_decodingState.store(e_DecodingStateCheckLoop, std::memory_order_relaxed);
                    break;
                } else {
                    throw VideoDecoderException(this, "Receive package of decoded video frame failed", ret);
                }
            } else {
                m_videoFrames.PushBack();
            }

            m_decodingState.store(e_DecodingStateReadFrame, std::memory_order_relaxed);

            break;
        }

        case e_DecodingStateDrainAudioBuffers: {
            AVFrame* curFrame;

            // a single encoded frame from file might contain one or multiple complete
            // decoded audio frames
            while (true) {
                // check if there is space in the queue
                curFrame = m_audioFrames.GetCurBack();

                // no place in queue, retry later
                if (!curFrame) {
                    break;
                }

                ret = avcodec_receive_frame(m_audioCodecCtx, curFrame);

                if (ret < 0) {
                    if (ret == AVERROR(EAGAIN)) {
                        // internal output buffer empty, read next frame
                        break;
                    } else if (ret == AVERROR_EOF) {
                        // fully flushed, no more frames to process
                        m_decodingState.store(e_DecodingStateCheckLoop, std::memory_order_relaxed);
                        break;
                    } else {
                        throw VideoDecoderException(this, "Receive package of decoded audio frame failed", ret);
                    }
                } else {
                    m_audioFrames.PushBack();
                }
            }

            if (curFrame) {
                m_decodingState.store(e_DecodingStateReadFrame, std::memory_order_relaxed);
            }

            break;
        }

        case e_DecodingStateCheckLoop: {
            while (true) {
                int32_t loopState = m_loopState.load(std::memory_order_relaxed);
                uint32_t loopStart = m_loopFramePos.load(std::memory_order_relaxed);

                // endless loop
                if (loopState <= -1) {
                    __ResetVideo(false, loopStart);
                    break;
                } else if (loopState == 0) {
                    m_decodingState.store(e_DecodingStateEOF, std::memory_order_relaxed);
                    break;
                } else {
                    if (m_loopState.compare_exchange_strong(loopState, loopState - 1, std::memory_order_relaxed)) {
                        __ResetVideo(false, loopStart);
                        break;
                    }
                }
            }

            return;
        }

        case e_DecodingStateEOF: {
            _SleepMs(1);
           return;
        }

        case e_DecodingStatePaused: {
            _SleepMs(10);
            return;
        }

        default: {
            throw VideoDecoderException(this, "Unhandled decoding state");
        }
    }

    // slow down a little to reduce CPU load
    _SleepUs(100);
}

void VideoDecoder::__ParseContainer()
{
    int averror;

    // open input video container file and create the format context for it
    // this only looks at the header of the file
    m_avFormatCtx = avformat_alloc_context();

    averror = avformat_open_input(&m_avFormatCtx, m_path.c_str(), nullptr,
        nullptr);
    if (averror < 0) {
        throw VideoDecoderException(this, "Cannot open file and create avformat context", averror);
    }

    // find stream info in container
    // this populates m_avFormatCtx->streams
    averror = avformat_find_stream_info(m_avFormatCtx, nullptr);
    if (averror < 0) {
        throw VideoDecoderException(this, "Can't find stream info", averror);
    }

    // dump info about streams in container to stdout, useful for debugging
    av_dump_format(m_avFormatCtx, 0, m_path.c_str(), false);
}

void VideoDecoder::__ParseStreams()
{
    // selecting video stream here
    // loop through all the streams in the container
    for (uint32_t i = 0; i < m_avFormatCtx->nb_streams; i++)
    {
        // select first video and audio stream found
        switch (m_avFormatCtx->streams[i]->codecpar->codec_type) {
            case AVMEDIA_TYPE_VIDEO:
                if (m_videoStreamIdx == -1) {
                    m_videoStreamIdx = i;
                }

                break;

            case AVMEDIA_TYPE_AUDIO:
                if (m_audioStreamIdx == -1) {
                    m_audioStreamIdx = i;
                }

                break;

            default:
                break;
        }
    }

    if (m_videoStreamIdx == -1) {
        throw VideoDecoderException(this, "Could not find video stream");
    }

    m_videoStreamParams = m_avFormatCtx->streams[m_videoStreamIdx]->codecpar;

    // video only is fine as well
    if (m_audioStreamIdx == -1) {
        m_audioStreamParams = nullptr;
    } else {
        m_audioStreamParams = m_avFormatCtx->streams[m_audioStreamIdx]->codecpar;
    }
}

void VideoDecoder::__OpenVideoStream()
{
    int averror;

    // find the decoder for the selected stream
    m_videoCodec = avcodec_find_decoder(m_videoStreamParams->codec_id);

    if (!m_videoCodec) {
        throw VideoDecoderException(this, "Could not find video decoder");
    }

    // create a context for decoding
    m_videoCodecCtx = avcodec_alloc_context3(m_videoCodec);

    if (!m_videoCodecCtx) {
        throw VideoDecoderException(this, "Could not allocate video codec ctx");
    }

    averror = avcodec_parameters_to_context(m_videoCodecCtx, m_videoStreamParams);

    if (averror < 0) {
        throw VideoDecoderException(this, "Fill video context with params failed",
            averror);
    }

    // Inform the codec that we can handle truncated bitstreams -- i.e.,
    // bitstreams where frame boundaries can fall in the middle of packets
    if (m_videoCodec->capabilities & AV_CODEC_CAP_TRUNCATED) {
        m_videoCodecCtx->flags |= AV_CODEC_FLAG_TRUNCATED;
    }

    // open the codec for the context
    averror = avcodec_open2(m_videoCodecCtx, m_videoCodec, nullptr);

    if (averror < 0) {
        throw VideoDecoderException(this, "Opening video codec for decoding ctx failed", averror);
    }
}

void VideoDecoder::__OpenAudioStream()
{
    int averror;

    // video only, don't need an audio decoder
    if (m_audioStreamIdx == -1) {
        return;
    }

    // find the decoder for the selected stream
    m_audioCodec = avcodec_find_decoder(m_audioStreamParams->codec_id);

    if (!m_audioCodec) {
        throw VideoDecoderException(this, "Could not find audio decoder");
    }

    // create a context for decoding
    m_audioCodecCtx = avcodec_alloc_context3(m_audioCodec);

    if (!m_audioCodecCtx) {
        throw VideoDecoderException(this, "Could not allocate audio codec ctx");
    }

    averror = avcodec_parameters_to_context(m_audioCodecCtx, m_audioStreamParams);

    if (averror < 0) {
        throw VideoDecoderException(this, "Fill audio context with params failed",
            averror);
    }

    // open the codec for the context
    averror = avcodec_open2(m_audioCodecCtx, m_audioCodec, nullptr);

    if (averror < 0) {
        throw VideoDecoderException(this, "Opening audio codec for decoding ctx failed", averror);
    }

    // necessary for converting to interleaved audio
    switch (m_audioCodecCtx->sample_fmt) {
        case AV_SAMPLE_FMT_U8:
        case AV_SAMPLE_FMT_S16:
        case AV_SAMPLE_FMT_S32:
        case AV_SAMPLE_FMT_FLT:
        case AV_SAMPLE_FMT_DBL:
        case AV_SAMPLE_FMT_S64:
            m_audioSamplesPlanar = false;
            break;

        case AV_SAMPLE_FMT_U8P:
        case AV_SAMPLE_FMT_S16P:
        case AV_SAMPLE_FMT_S32P:
        case AV_SAMPLE_FMT_FLTP:
        case AV_SAMPLE_FMT_DBLP:
        case AV_SAMPLE_FMT_S64P:
            m_audioSamplesPlanar = true;
            break;

        default:
            break;
    }
}

void VideoDecoder::__Cleanup()
{
    if (m_videoResizedFrame) {
        av_frame_free(&m_videoResizedFrame);
    }

    if (m_avPacket) {
        av_packet_free(&m_avPacket);
    }

    if (m_audioCodecCtx) {
        avcodec_free_context(&m_audioCodecCtx);
    }

    if (m_videoCodecCtx) {
        avcodec_free_context(&m_videoCodecCtx);
    }

    if (m_avFormatCtx) {
        avformat_close_input(&m_avFormatCtx);
    }
}

void VideoDecoder::__ResetVideo(bool clearQueues, uint32_t framePos)
{
    int averror;

    if (m_audioStreamIdx != -1) {
        // don't use AVSEEK_FLAG_FRAME or AVSEEK_FLAG_BYTE because these don't work on all decoders/file formats
        // the beginning can also be at a negative "timestamp". this should ensure that we seek to the start

        if (!framePos) {
            averror = av_seek_frame(m_avFormatCtx, m_audioStreamIdx, -9999999, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_ANY);
        } else {
            averror = av_seek_frame(m_avFormatCtx, m_audioStreamIdx, framePos, AVSEEK_FLAG_FRAME);
        }

        if (averror < 0) {
            throw VideoDecoderException(this, averror, "Resetting audio stream failed");
        }
    }

    if (!framePos) {
        averror = av_seek_frame(m_avFormatCtx, m_videoStreamIdx, -9999999, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_ANY);
    } else {
        averror = av_seek_frame(m_avFormatCtx, m_videoStreamIdx, framePos, AVSEEK_FLAG_FRAME);
    }

    if (averror < 0) {
        throw VideoDecoderException(this, averror, "Resetting video stream failed");
    }

    // dont forget to flush the buffers
    avcodec_flush_buffers(m_videoCodecCtx);

    if (m_audioCodecCtx) {
        avcodec_flush_buffers(m_audioCodecCtx);
    }

    if (clearQueues) {
        m_videoFrames.Reset();
        m_audioFrames.Reset();
    }

    m_decodingState.store(e_DecodingStateReadFrame, std::memory_order_relaxed);
}

VideoDecoder::FrameFifo::FrameFifo(size_t size) :
    m_size(size),
    m_front(0),
    m_back(0)
{
    m_buffer.resize(m_size);

    for (auto& i : m_buffer) {
        i = av_frame_alloc();
        if (!i) {
            throw VideoDecoderException("Allocating frame buffer failed");
        }
    }
}

VideoDecoder::FrameFifo::~FrameFifo()
{
    for (auto& i : m_buffer) {
        av_frame_free(&i);
    }
}

bool VideoDecoder::FrameFifo::IsEmpty() const
{
    size_t front = m_front.load(std::memory_order_relaxed) % m_size;
    size_t back = m_back.load(std::memory_order_relaxed) % m_size;

    return front == back;
}

size_t VideoDecoder::FrameFifo::GetBuffered() const
{
    size_t front = m_front.load(std::memory_order_relaxed) % m_size;
    size_t back = m_back.load(std::memory_order_relaxed) % m_size;

    if (back < front) {
        return (m_size - front) + back;
    } else {
        return back - front;
    }
}

AVFrame* VideoDecoder::FrameFifo::GetCurBack() const
{
    size_t front = m_front.load(std::memory_order_relaxed) % m_size;
    size_t back = m_back.load(std::memory_order_relaxed) % m_size;

    if ((back + 1) % m_size == front) {
        return nullptr;
    }

    return m_buffer[back];
}

void VideoDecoder::FrameFifo::PushBack()
{
    size_t front = m_front.load(std::memory_order_relaxed) % m_size;
    size_t back = m_back.load(std::memory_order_relaxed) % m_size;

    if ((back + 1) % m_size == front) {
        return;
    }

    m_back.fetch_add(1, std::memory_order_relaxed);
}

AVFrame* VideoDecoder::FrameFifo::GetCurFront() const
{
    size_t front = m_front.load(std::memory_order_relaxed) % m_size;
    size_t back = m_back.load(std::memory_order_relaxed) % m_size;

    if (back == front) {
        return nullptr;
    }

    return m_buffer[front];
}

void VideoDecoder::FrameFifo::PopFront()
{
    size_t front = m_front.load(std::memory_order_relaxed) % m_size;
    size_t back = m_back.load(std::memory_order_relaxed) % m_size;

    if (back == front) {
        return;
    }

    m_front.fetch_add(1, std::memory_order_relaxed);
}

void VideoDecoder::FrameFifo::Reset()
{
    m_front.store(0, std::memory_order_relaxed);
    m_back.store(0, std::memory_order_relaxed);
}

}