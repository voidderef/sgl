#pragma once

#include <thread>
#include <unordered_map>

extern "C" {
#include "bt/api/glue.h"
}

namespace io {
namespace bt {

class BemanitoolsGlue
{
public:
    static void LogFormatterMisc(const char* module, const char* fmt, ...);

    static void LogFormatterInfo(const char* module, const char* fmt, ...);

    static void LogFormatterWarning(const char* module, const char* fmt, ...);

    static void LogFormatterFatal(const char* module, const char* fmt, ...);

    static int ThreadCreate(int (*proc)(void*), void* ctx, uint32_t stackSize, unsigned int priority);

    static void ThreadJoin(int threadId, int* result);

    static void ThreadDestroy(int threadId);

private:
    BemanitoolsGlue() = default;
    ~BemanitoolsGlue() = default;

    static int m_threadId;
    static std::unordered_map<int, std::thread*> m_threads;
};

}
}
