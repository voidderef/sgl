#include "BemanitoolsGlue.h"

#include <cstdarg>

#include "ks/Logger.hpp"

namespace io {
namespace bt {

int BemanitoolsGlue::m_threadId = 0;
std::unordered_map<int, std::thread*> BemanitoolsGlue::m_threads;

void BemanitoolsGlue::LogFormatterMisc(const char* module, const char* fmt, ...)
{
    // 64k in bemanitools to allow large logs like IO buffer dumps
    char buffer[65536];

    va_list args;
    va_start(args, fmt);

    vsprintf(buffer, fmt, args);

    va_end(args);

    KS_LOG_DEBUG("[%s] %s", module, buffer);
}

void BemanitoolsGlue::LogFormatterInfo(const char* module, const char* fmt, ...)
{
    // 64k in bemanitools to allow large logs like IO buffer dumps
    char buffer[65536];

    va_list args;
    va_start(args, fmt);

    vsprintf(buffer, fmt, args);

    va_end(args);

    KS_LOG_INFO("[%s] %s", module, buffer);
}

void BemanitoolsGlue::LogFormatterWarning(const char* module, const char* fmt, ...)
{
    // 64k in bemanitools to allow large logs like IO buffer dumps
    char buffer[65536];

    va_list args;
    va_start(args, fmt);

    vsprintf(buffer, fmt, args);

    va_end(args);

    KS_LOG_WARN("[%s] %s", module, buffer);
}

void BemanitoolsGlue::LogFormatterFatal(const char* module, const char* fmt, ...)
{
    // 64k in bemanitools to allow large logs like IO buffer dumps
    char buffer[65536];

    va_list args;
    va_start(args, fmt);

    vsprintf(buffer, fmt, args);

    va_end(args);

    KS_LOG_ERROR("[%s] %s", module, buffer);
}

int BemanitoolsGlue::ThreadCreate(int (*proc)(void*), void* ctx, uint32_t stackSize, unsigned int priority)
{
    int threadId = m_threadId++;

    std::thread* thread = new std::thread(proc, ctx);

    m_threads[threadId] = thread;

    return threadId;
}

void BemanitoolsGlue::ThreadJoin(int threadId, int* result)
{
    std::thread* thread = m_threads[threadId];
    thread->join();

    *result = 0;
}

void BemanitoolsGlue::ThreadDestroy(int threadId)
{
    std::thread* thread = m_threads[threadId];
    m_threads[threadId] = nullptr;

    delete thread;
}

}
}