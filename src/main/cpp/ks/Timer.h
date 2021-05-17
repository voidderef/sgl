#pragma once

#ifdef KS_PLATFORM_WIN32
#include <cstdint>
#include <windows.h>
#else
#include <chrono>
#endif

namespace ks {

class Timer
{
public:
    Timer() :
        m_running(false),
        m_start(),
        m_end()
    {};

    ~Timer() = default;

    bool IsRunning() const {
        return m_running;
    }

    void Clear() {
        m_running = false;
        m_end = m_start;
    }

    void Start() {
        m_running = true;

#ifdef KS_PLATFORM_WIN32
        m_start = __NowNs();
#elif defined(KS_PLATFORM_LINUX)
        m_start = std::chrono::high_resolution_clock::now();
#endif
    }

    void StartOffsetNs(double ns) {
        Start();

#ifdef KS_PLATFORM_WIN32
        m_start += ns;
#elif defined(KS_PLATFORM_LINUX)
        m_start += std::chrono::nanoseconds(static_cast<int64_t>(ns));
#endif
    }

    void StartOffsetUs(double us) {
        Start();

#ifdef KS_PLATFORM_WIN32
        m_start += us * 1000;
#elif defined(KS_PLATFORM_LINUX)
        m_start += std::chrono::microseconds(static_cast<int64_t>(us * 1000));
#endif
    }

    void StartOffsetMs(double ms) {
        Start();

#ifdef KS_PLATFORM_WIN32
        m_start += ms * 1000 * 1000;
#elif defined(KS_PLATFORM_LINUX)
        m_start += std::chrono::milliseconds(static_cast<int64_t>(ms * 1000 * 1000));
#endif
    }

    void StartOffsetSec(double sec) {
        Start();

#ifdef KS_PLATFORM_WIN32
        m_start += sec * 1000 * 1000 * 1000;
#elif defined(KS_PLATFORM_LINUX)
        m_start += std::chrono::seconds(static_cast<int64_t>(sec * 1000 * 1000 * 1000));
#endif
    }

    void Stop() {
        m_running = true;

#ifdef KS_PLATFORM_WIN32
        m_end = __NowNs();
#elif defined(KS_PLATFORM_LINUX)
        m_end = std::chrono::high_resolution_clock::now();
#endif
    }

    double GetSec() {
#ifdef KS_PLATFORM_WIN32
        if (m_running) {
            m_end = __NowNs();
        }

        return (m_end - m_start) / 1000.0 / 1000.0 / 1000.0;
#elif defined(KS_PLATFORM_LINUX)
        if (m_running) {
            m_end = std::chrono::high_resolution_clock::now();
        }

        std::chrono::duration<double, std::nano> delta(m_end - m_start);
        return delta.count() / 1000.0 / 1000.0 / 1000.0;
#endif
    }

    double GetMs() {
#ifdef KS_PLATFORM_WIN32
        if (m_running) {
            m_end = __NowNs();
        }

        return (m_end - m_start) / 1000.0 / 1000.0;
#elif defined(KS_PLATFORM_LINUX)
        if (m_running) {
            m_end = std::chrono::high_resolution_clock::now();
        }

        std::chrono::duration<double, std::nano> delta(m_end - m_start);
        return delta.count() / 1000.0 / 1000.0;
#endif
    }

    double GetUs() {
#ifdef KS_PLATFORM_WIN32
        if (m_running) {
            m_end = __NowNs();
        }

        return (m_end - m_start) / 1000.0;
#elif defined(KS_PLATFORM_LINUX)
        if (m_running) {
            m_end = std::chrono::high_resolution_clock::now();
        }

        std::chrono::duration<double, std::nano> delta(m_end - m_start);
        return delta.count() / 1000.0;
#endif
    }

    double GetNs() {
#ifdef KS_PLATFORM_WIN32
        if (m_running) {
            m_end = __NowNs();
        }

        return m_end - m_start;
#elif defined(KS_PLATFORM_LINUX)
        if (m_running) {
            m_end = std::chrono::high_resolution_clock::now();
        }

        std::chrono::duration<double, std::nano> delta(m_end - m_start);
        return delta.count();
#endif
    }

private:
    bool m_running;

    // Oh windows...
#ifdef KS_PLATFORM_WIN32
    uint64_t m_start;
    uint64_t m_end;

    static inline uint64_t __NowNs() {
        LARGE_INTEGER time;
        LARGE_INTEGER frequency;

        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&time);

        return ((1000 * 1000 * time.QuadPart) / frequency.QuadPart) * 1000;
    }
#elif defined(KS_PLATFORM_LINUX)
    std::chrono::high_resolution_clock::time_point m_start;
    std::chrono::high_resolution_clock::time_point m_end;
#else
#error "No platform defined"
#endif
};

}
