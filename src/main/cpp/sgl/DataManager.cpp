#include "DataManager.h"

#include "ks/Logger.hpp"

#include "ResourceException.h"

namespace sgl {

const std::string DataManager::ms_audio = "aud";
const std::string DataManager::ms_font = "font";
const std::string DataManager::ms_io = "io";
const std::string DataManager::ms_lua = "lua";
const std::string DataManager::ms_mov = "mov";
const std::string DataManager::ms_sfx = "sfx";
const std::string DataManager::ms_shell = "shell";
const std::string DataManager::ms_tex = "tex";

const std::string DataManager::ms_games = "games.lua";

DataManager::DataManager(ks::FileSystem* fs) :
    m_fsData(fs),
    m_fsAudio(new ks::FileSystem(ms_audio, m_fsData, "aud")),
    m_fsFont(new ks::FileSystem(ms_font, m_fsData, "font")),
    m_fsIo(new ks::FileSystem(ms_io, m_fsData, "io")),
    m_fsLua(new ks::FileSystem(ms_lua, m_fsData, "lua")),
    m_fsMov(new ks::FileSystem(ms_mov, m_fsData, "mov")),
    m_fsSfx(new ks::FileSystem(ms_sfx, m_fsData, "sfx")),
    m_fsShell(new ks::FileSystem(ms_shell, m_fsData, "shell")),
    m_fsTex(new ks::FileSystem(ms_tex, m_fsData, "tex")),
    m_games(m_fsData->GetAbsolutePath(ms_games))
{
    KS_LOG_INFO("Initialing data manager, data dir: %s", m_fsData->GetRootPath());

    __AssertFs(m_fsData);

    if (!m_fsData->Exists(ms_games)) {
        throw ResourceException("Missing %s in data dir", ms_games);
    }

    __AssertFs(m_fsAudio);
    __AssertFs(m_fsFont);
    __AssertFs(m_fsIo);
    __AssertFs(m_fsLua);
    __AssertFs(m_fsMov);
    __AssertFs(m_fsSfx);
    __AssertFs(m_fsShell);
    __AssertFs(m_fsTex);
}

DataManager::~DataManager()
{
    delete m_fsData;

    delete m_fsAudio;
    delete m_fsFont;
    delete m_fsIo;
    delete m_fsLua;
    delete m_fsMov;
    delete m_fsSfx;
    delete m_fsShell;
    delete m_fsTex;
}

void DataManager::__AssertFs(ks::FileSystem* refFs)
{
    if (!refFs->Exists()) {
        throw ResourceException(refFs, "Path does not exist");
    }
}

}