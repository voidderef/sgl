#pragma once

#include <string>

namespace sgl {

class SGLSettings
{
public:
    static const std::string ms_gfxFullscreen;
    static const std::string ms_gfxResWidth;
    static const std::string ms_gfxResHeight;

    static const std::string ms_ispScripts;
    static const std::string ms_ospScripts;

    static const std::string ms_soundDriver;
    static const std::string ms_soundDevice;
    static const std::string ms_soundMasterVolume;
    static const std::string ms_soundSfxVolume;

    static const std::string ms_systemLogLevel;
    static const std::string ms_systemDataPath;
    static const std::string ms_systemSavePath;
    static const std::string ms_systemTmpPath;

private:
    SGLSettings() = default;
    ~SGLSettings() = default;
};

}
