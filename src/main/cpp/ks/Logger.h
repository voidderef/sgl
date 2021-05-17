#pragma once

#include <spdlog/spdlog.h>

namespace ks {

class Logger
{
public:
    static void Setup(const std::string& logFile = "");

    static void Shutdown();

    static std::shared_ptr<spdlog::logger>& GetLogger() {
        return m_logger;
    }

private:
    Logger() = default;

    ~Logger() = default;

    static std::shared_ptr<spdlog::logger> m_logger;
};

}
