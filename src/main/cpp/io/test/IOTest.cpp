#include <backwards/backward.hpp>

#include "ks/Logger.hpp"
#include "ks/Process_.h"
#include "ks/Random.h"
#include "ks/Settings.h"
#include "ks/SettingsCmdArgsLoader.h"
#include "ks/Thread.h"

#include "IOManager.h"

#include "IOSPTestNull.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("IOTest, build " __DATE__ " " __TIME__ "\n"
            "Usage: IOTest [settings --key value] ...\n");
        return -1;
    }

    auto signalHandler = new backward::SignalHandling();

    ks::Logger::Setup();
    ks::Random::Init();
    ks::Process::LogInfo();

    ks::Settings settings;
    ks::SettingsCmdArgsLoader loader(argc, argv);
    loader.Load(settings);

    auto ioProcessManager = new io::IOStateProcessorManager();

    // TODO add state processors, load from lua?
    ioProcessManager->Add(new io::IOSPTestNull());

    auto ioManager = new io::IOManager(nullptr, ioProcessManager);

    ioManager->Init(settings);
    ioManager->SetIOProcessors("test");

    KS_LOG_INFO("Running loop");

    while (true) {
        ioManager->StateProcess();

        if (ioManager->GetInputStates().GetDigital(0)) {
            printf("Input 0\n");
        }

        if (ioManager->GetInputStates().GetDigital(1)) {
            printf("Input 1\n");
        }

        if (ioManager->GetInputStates().GetDigital(2)) {
            break;
        }

        // reduce cpu load
        ks::Thread::SleepMs(1);
    }

    KS_LOG_INFO("Loop exit");

    ioManager->Shutdown();
    delete ioManager;

    ks::Random::Shutdown();
    ks::Logger::Shutdown();

    delete signalHandler;
}