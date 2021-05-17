#pragma once

#ifdef KS_PLATFORM_WIN32
#include <cstdint>
#include <windows.h>
#else
#include <chrono>
#endif

namespace ks {

class Time
{
public:
    static double NowSec() {
        return NowNs() / 1000.0 / 1000.0 / 1000.0;
    }

    static double NowMs() {
        return NowNs() / 1000.0 / 1000.0;
    }

    static double NowUs() {
        return NowNs() / 1000.0;
    }

    static double NowNs() {
        // Oh windows...
#ifdef KS_PLATFORM_WIN32
        LARGE_INTEGER time;
        LARGE_INTEGER frequency;

        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&time);

        return ((1000 * 1000 * time.QuadPart) / frequency.QuadPart) * 1000;
#elif defined(KS_PLATFORM_LINUX)
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> duration(now.time_since_epoch());

        return duration.count();
#endif
    };

private:
    Time() {};
    ~Time() {};
};

}
