//
// Created by on 4/23/18.
//

#include "System.h"

#ifdef KS_PLATFORM_LINUX
#include <errno.h>
#include <unistd.h>

#include <linux/reboot.h>
#include <sys/reboot.h>
#elif defined(KS_PLATFORM_WIN32)
#include <windows.h>
#endif

#include "SystemException.h"

namespace ks {

void System::Reboot()
{
#ifdef KS_PLATFORM_LINUX
    sync();
    reboot(RB_AUTOBOOT);

    throw ks::SystemException(errno, "Reboot failed");
#elif defined(KS_PLATFORM_WIN32)
    __ShutdownOrReboot(false);
#endif
}

void System::Shutdown()
{
#ifdef KS_PLATFORM_LINUX
    sync();
    reboot(LINUX_REBOOT_CMD_POWER_OFF);

    throw ks::SystemException(errno, "Shutdown failed");
#elif defined(KS_PLATFORM_WIN32)
    __ShutdownOrReboot(true);
#endif
}

#if defined(KS_PLATFORM_WIN32)
void System::__ShutdownOrReboot(bool shutdown)
{
    // source: https://msdn.microsoft.com/en-us/library/windows/desktop/aa376871(v=vs.85).aspx
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // Get a token for this process.

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        throw ks::SystemException("Getting process token failed");
    }

    // Get the LUID for the shutdown privilege.

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

    // one privilege to set
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process.

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS) {
        throw ks::SystemException("Adjust token privileges failed");
    }

    // Shut down the system and force all applications to close.

    UINT flags = shutdown ? EWX_SHUTDOWN : EWX_REBOOT;

    if (!ExitWindowsEx(flags | EWX_FORCE,
            SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) {
        throw ks::SystemException(shutdown ? "Shutting down system failed" : "Rebooting system failed");
    }
}
#endif

}