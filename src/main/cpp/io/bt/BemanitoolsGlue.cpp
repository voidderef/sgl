#include "BemanitoolsGlue.h"

#include <cstdarg>

#include "ks/Logger.hpp"

namespace io {
namespace bt {

int BemanitoolsGlue::m_threadId = 0;
std::unordered_map<int, std::thread*> BemanitoolsGlue::m_threads;

void BemanitoolsGlue::LogFormatterMisc(const char* module, const char* fmt, ...)
{
    // TODO incomplete
    KS_LOG_DEBUG("[%s] %s", module, fmt);
}

void BemanitoolsGlue::LogFormatterInfo(const char* module, const char* fmt, ...)
{
    // TODO incomplete
    KS_LOG_INFO("[%s] %s", module, fmt);
}

void BemanitoolsGlue::LogFormatterWarning(const char* module, const char* fmt, ...)
{
    // TODO incomplete
    KS_LOG_WARN("[%s] %s", module, fmt);
}

void BemanitoolsGlue::LogFormatterFatal(const char* module, const char* fmt, ...)
{
    // TODO incomplete
    KS_LOG_ERROR("[%s] %s", module, fmt);
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