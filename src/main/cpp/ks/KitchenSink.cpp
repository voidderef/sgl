#include "KitchenSink.h"

#include "Logger.hpp"
#include "Process.h"
#include "Random.h"

namespace ks {

backward::SignalHandling* KitchenSink::ms_signalHandler = nullptr;

void KitchenSink::Init(const std::string& logFile)
{
    ms_signalHandler = new backward::SignalHandling();

    Logger::Setup(logFile);

    KS_LOG_INFO("Initializing, log file %s...", logFile);

    Random::Init();

    Process::LogInfo();
}

void KitchenSink::Shutdown()
{
    KS_LOG_INFO("Shutdown...");

    Random::Shutdown();

    Logger::Shutdown();

    delete ms_signalHandler;
}

}