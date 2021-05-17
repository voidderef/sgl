#pragma once

#include "ks/Exception.h"
#include "VideoDecoder.h"

namespace sgl {

class VideoDecoderException : public ks::Exception
{
public:
    template<typename... Args>
    explicit VideoDecoderException(const std::string& format, Args... args) :
        Exception(format, args...)
    {}

    template<typename... Args>
    explicit VideoDecoderException(const VideoDecoder* refVideoDecoder, const std::string& format, Args... args) :
        Exception("[%s] " + format, refVideoDecoder->GetPath(), args...)
    {}

    template<typename... Args>
    explicit VideoDecoderException(const VideoDecoder* refVideoDecoder, int averror, const std::string& format,
            Args... args) :
        Exception("[%s] " + format + ": %s", refVideoDecoder->GetPath(), args..., av_make_error_string(m_errBuffer,
            AV_ERROR_MAX_STRING_SIZE, averror))
    {}

    ~VideoDecoderException() override = default;

private:
    char m_errBuffer[AV_ERROR_MAX_STRING_SIZE] {};
};

}
