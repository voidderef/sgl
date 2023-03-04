#ifdef KS_PLATFORM_LINUX
#include <errno.h>
#include <linux/limits.h>
#include <unistd.h>
#endif

#include "Logger.hpp"
#include "Process_.h"

namespace ks {

void Process::LogInfo()
{
    KS_LOG_INFO("================================  Current process information ================================");

    __LogCurrentProcess();
    __LogPathExecutable();
    __LogCurrentWorkingDirectory();
    __LogUser();
    __LogCmdLineArgs();
    __LogEnvironmentVariables();

    KS_LOG_INFO("================================================================================================");
}

void Process::__LogCurrentProcess()
{
#ifdef KS_PLATFORM_LINUX
    KS_LOG_INFO("PID: %d", getpid());
#endif
}

void Process::__LogPathExecutable()
{
#ifdef KS_PLATFORM_LINUX
    char buffer[PATH_MAX];

    // readlink does not null terminate
    memset(buffer, 0, sizeof(buffer));

    if (readlink("/proc/self/exe", buffer, sizeof(buffer)) != -1) {
        KS_LOG_INFO("Executable: %s", buffer);
    } else {
        KS_LOG_ERROR("Getting path of executable failed");
    }
#endif
}

void Process::__LogCurrentWorkingDirectory()
{
#ifdef KS_PLATFORM_LINUX
    char tmp[PATH_MAX];

    if (!getcwd(tmp, sizeof(tmp))) {
        KS_LOG_ERROR("Getting current working directory failed");
    } else {
        KS_LOG_INFO("Current working directory: %s", tmp);
    }
#endif
}

void Process::__LogUser()
{
#ifdef KS_PLATFORM_LINUX
    KS_LOG_INFO("User: %d", getuid());
#endif
}

void Process::__LogCmdLineArgs()
{
#ifdef KS_PLATFORM_LINUX
    int count;
    std::string str = __ReadEntriesFromProcFile("/proc/self/cmdline", &count);

    KS_LOG_INFO("Cmd arguments (%d):\n%s", count, str);
#endif
}

void Process::__LogEnvironmentVariables()
{
#ifdef KS_PLATFORM_LINUX
    int count;
    std::string str = __ReadEntriesFromProcFile("/proc/self/environ", &count);

    KS_LOG_INFO("Environment variables (%d):\n%s", count, str);
#endif
}

#ifdef KS_PLATFORM_LINUX
std::string Process::__ReadEntriesFromProcFile(const char* path, int* count_out)
{
    std::string str;

    FILE* cmdline = fopen(path, "rb");

    if (!cmdline) {
        KS_LOG_ERROR("Opening %s failed: %s", path, strerror(errno));
        return str;
    }

    char* arg = 0;
    size_t size = 0;
    int count = 0;

    while (getdelim(&arg, &size, 0, cmdline) != -1) {
       if (!str.empty()) {
            str += '\n';
        }

        str += std::string(arg);
        count++;
    }

    free(arg);
    fclose(cmdline);

    if (count_out) {
        *count_out = count;
    }

    return str;
}
#endif

}