#include "ShellScript.h"

#include "ks/FileSystem.h"
#include "ks/Logger.hpp"

namespace sgl {

ShellScript::ShellScript(const std::string& path) :
    m_path(path)
{

}

void ShellScript::Execute()
{
    // note:
    // don't use fork, execve, posix_spawn on Linux (and CreateProcess on
    // windows). This causes issues if the target application wants to use the
    // same IO hardware the loader is using (hardware not responding when
    // running target application)

    KS_LOG_DEBUG("Executing %s", m_path);

#ifdef SGL_PLATFORM_LINUX
    std::string script = "#!/bin/bash\n\n" + m_path;

    ks::FileSystem fs("/tmp");
    // +1 to write the null terminator as well
    fs.SaveFile("sgl_exec.sh", script.c_str(), script.length() + 1);

    // mark it exectuable
    chmod("/tmp/sgl_exec.sh", S_IRWXU);

    KS_LOG_INFO("Temporary exec script /tmp/sgl_exec.sh created");

    exit(0);
#elif defined(SGL_PLATFORM_WIN32)
    std::string script = m_path;

    ks::FileSystem fs("C:/Windows/Temp");
    // +1 to write the null terminator as well
    fs.SaveFile("sgl_exec.bat", script.c_str(), script.length() + 1);

    KS_LOG_INFO("Temporary exec script C:/Windows/Temp created");

    exit(0);
#else
#error "No valid platform defined"
#endif
}

}