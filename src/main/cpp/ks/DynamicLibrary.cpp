#include "DynamicLibrary.h"

#ifdef KS_PLATFORM_LINUX
#include <dlfcn.h>
#endif

#include "Exception.h"

namespace ks {

DynamicLibrary::DynamicLibrary(const std::string &path) :
    m_name(path),
#ifdef KS_PLATFORM_LINUX
    m_handle(nullptr)
#elif defined(KS_PLATFORM_WIN32)
    m_handle(NULL)
#endif
{
#ifdef KS_PLATFORM_LINUX
    m_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);

    if (m_handle == nullptr) {
        throw Exception("Could not load library " + path + ": " + std::string(dlerror()));
    }
#elif defined(KS_PLATFORM_WIN32)
    m_handle = LoadLibraryA(path.c_str());

    if (m_handle == nullptr) {
        throw Exception("Could not load library " + path + ": " + std::to_string(GetLastError()));
    }
#endif
}

DynamicLibrary::~DynamicLibrary()
{
#ifdef KS_PLATFORM_LINUX
    dlclose(m_handle);
#elif defined(KS_PLATFORM_WIN32)
    FreeLibrary(m_handle);
#endif
}

void* DynamicLibrary::GetFuncAddr(const std::string& funcName)
{
#ifdef KS_PLATFORM_LINUX
    void* ret = dlsym(m_handle, funcName.c_str());

    if (ret == nullptr) {
        throw Exception("Could not find func " + funcName + " in library " + m_name);
    }

    return ret;
#elif defined(KS_PLATFORM_WIN32)
    FARPROC address = GetProcAddress(m_handle, funcName.c_str());

    if (address == nullptr) {
        throw Exception("Could not find func " + funcName + " in library " + m_name);
    }

    return (void*) address;
#endif
}

}