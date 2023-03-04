#include "GameLauncher.h"

#include "ks/FileSystem.h"
#include "ks/FileSystemException.h"
#include "ks/Logger.hpp"

namespace sgl {

#ifdef SGL_PLATFORM_LINUX
const std::string GameLauncher::ms_tmpGameLauncherScriptName = "sgl_launch_game";
#elif defined(SGL_PLATFORM_WIN32)
const std::string GameLauncher::ms_tmpGameLauncherScriptName = "sgl_launch_game.bat";
#else
#error "No valid platform defined"
#endif

GameLauncher::GameLauncher(ks::FileSystem* fs) :
    m_fs(fs)
{

}

GameLauncher::~GameLauncher()
{
    delete m_fs;
}

void GameLauncher::Init()
{
    if (!m_fs->Exists()) {
        throw new ks::FileSystemException("Path %s for game launcher scripts does not exist", m_fs->GetRootPath());
    }

    if (m_fs->Exists(ms_tmpGameLauncherScriptName)) {
        m_fs->RemoveFile(ms_tmpGameLauncherScriptName);
    }

    KS_LOG_DEBUG("Prepared game launcher temporary path %s", m_fs->GetRootPath());
}

std::string GameLauncher::Prepare(const Games::Entry& entryToLaunch)
{
    // Notes about executing another program/process from here:
    // don't use fork, execve, posix_spawn on Linux (and CreateProcess on
    // windows). This causes issues if the target application wants to use the
    // same IO hardware the loader is using (hardware not responding when
    // running target application)

    KS_LOG_DEBUG("Preparing temporary exec script for %s in path %s", entryToLaunch, m_fs->GetRootPath());

    std::string script = __createGameLaunchScriptContents(entryToLaunch);

    m_fs->SaveFile(ms_tmpGameLauncherScriptName, script.c_str(), script.length());

    std::string tmpLauncherScriptPath = m_fs->GetAbsolutePath(ms_tmpGameLauncherScriptName);
#ifdef SGL_PLATFORM_LINUX
    // mark it exectuable
    chmod(tmpLauncherScriptPath.c_str(), S_IRWXU);
#endif

    KS_LOG_INFO("Temporary exec script %s created", tmpLauncherScriptPath);

    return tmpLauncherScriptPath;
}

std::string GameLauncher::__createGameLaunchScriptContents(const Games::Entry& entryToLaunch)
{
#ifdef SGL_PLATFORM_LINUX
    return "#!/bin/sh\n\n" + entryToLaunch.GetPathStartScript();
#elif defined(SGL_PLATFORM_WIN32)
    return "@echo off\n\n" + entryToLaunch.GetPathStartScript();
#else
#error "No valid platform defined"
#endif
}

}