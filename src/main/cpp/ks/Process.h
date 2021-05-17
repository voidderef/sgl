#pragma once

#include <string>

namespace ks {

class Process
{
public:
    static void LogInfo();

private:
    Process() {};
    ~Process() {};

    static void __LogCurrentProcess();
    static void __LogPathExecutable();
    static void __LogCurrentWorkingDirectory();
    static void __LogUser();
    static void __LogCmdLineArgs();
    static void __LogEnvironmentVariables();

#ifdef KS_PLATFORM_LINUX
    static std::string __ReadEntriesFromProcFile(const char* path, int* count_out);
#endif
};

}