#pragma once

#include <string>

#ifdef KS_PLATFORM_WIN32
#include <windows.h>
#endif

namespace ks {

class DynamicLibrary
{
public:
    explicit DynamicLibrary(const std::string& path);

    ~DynamicLibrary();

    const std::string& GetName() const {
        return m_name;
    }

    void* GetFuncAddr(const std::string& funcName);

private:
    const std::string m_name;

#ifdef KS_PLATFORM_LINUX
    void* m_handle;
#elif defined(KS_PLATFORM_WIN32)
    HMODULE m_handle;
#endif
};

}
