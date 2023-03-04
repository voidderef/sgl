#pragma once

#include <string>

#include "ks/FileSystem.h"
#include "ks/Object.h"

#include "sgl/Games.h"

namespace sgl {

class GameLauncher : public ks::Object
{
public:
    explicit GameLauncher(ks::FileSystem* fs);

    ~GameLauncher();

    void Init();

    std::string Prepare(const Games::Entry& entryToLaunch);

private:
    static const std::string ms_tmpGameLauncherScriptName;

    ks::FileSystem* m_fs;

    std::string __createGameLaunchScriptContents(const Games::Entry& entryToLaunch);
};

}
