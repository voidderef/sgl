#pragma once

namespace ks {

class System
{
public:
    static void Reboot();

    static void Shutdown();

private:
    System() = default;

    ~System() = default;

#if defined(KS_PLATFORM_WIN32)
    static void __ShutdownOrReboot(bool shutdown);
#endif
};

}
