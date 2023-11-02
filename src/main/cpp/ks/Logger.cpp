#include "Logger.h"

namespace ks {

std::shared_ptr<spdlog::logger> Logger::m_logger;

void Logger::Setup(const std::string& logFile)
{
    std::vector<spdlog::sink_ptr> sinks;

#ifdef KS_PLATFORM_LINUX
    auto colorSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();

    colorSink->set_color(spdlog::level::trace, colorSink->white);
    colorSink->set_color(spdlog::level::debug, colorSink->green);
    colorSink->set_color(spdlog::level::info, colorSink->bold + colorSink->blue);
    colorSink->set_color(spdlog::level::warn, colorSink->bold + colorSink->yellow);
    colorSink->set_color(spdlog::level::err, colorSink->bold + colorSink->red);
    colorSink->set_color(spdlog::level::critical, colorSink->bold + colorSink->on_red);
    colorSink->set_color(spdlog::level::off, colorSink->reset);
#elif defined(KS_PLATFORM_WIN32)
    auto colorSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#endif

    sinks.push_back(colorSink);

    if (!logFile.empty()) {
        sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>(logFile, true));
    }

    m_logger.reset();
    m_logger = std::make_shared<spdlog::logger>("sgl", begin(sinks), end(sinks));
    m_logger->set_pattern("[%L][%D][%T.%e][thread-%t]%v");

    m_logger->set_level(spdlog::level::trace);

    m_logger->flush_on(spdlog::level::info);
}

void Logger::Shutdown()
{
    m_logger.reset();
}

}