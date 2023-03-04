#include "SGLSettings.h"

#include "ks/Settings.h"

namespace sgl {

const std::string SGLSettings::ms_gfxFullscreen = "gfx" + ks::Settings::ms_delim + "fullscreen";
const std::string SGLSettings::ms_gfxResWidth = "gfx" + ks::Settings::ms_delim + "res_width";
const std::string SGLSettings::ms_gfxResHeight = "gfx" + ks::Settings::ms_delim + "res_height";

const std::string SGLSettings::ms_ispScripts = "io" + ks::Settings::ms_delim + "isp_scripts";
const std::string SGLSettings::ms_ospScripts = "io" + ks::Settings::ms_delim + "osp_scripts";

const std::string SGLSettings::ms_soundDriver = "sound" + ks::Settings::ms_delim + "driver";
const std::string SGLSettings::ms_soundDevice = "sound" + ks::Settings::ms_delim + "device";
const std::string SGLSettings::ms_soundMasterVolume = "sound" + ks::Settings::ms_delim + "master_volume";
const std::string SGLSettings::ms_soundSfxVolume = "sound" + ks::Settings::ms_delim + "sfx_volume";

const std::string SGLSettings::ms_systemDataPath = "system" + ks::Settings::ms_delim + "data_path";
const std::string SGLSettings::ms_systemSavePath = "system" + ks::Settings::ms_delim + "save_path";
const std::string SGLSettings::ms_systemTmpPath = "system" + ks::Settings::ms_delim + "tmp_path";

}