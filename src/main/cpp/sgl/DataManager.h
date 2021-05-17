#pragma once

#include "ks/FileSystem.h"

namespace sgl {

class DataManager
{
public:
    static const std::string ms_audio;
    static const std::string ms_font;
    static const std::string ms_io;
    static const std::string ms_lua;
    static const std::string ms_mov;
    static const std::string ms_sfx;
    static const std::string ms_shell;
    static const std::string ms_tex;

    static const std::string ms_games;

public:
    explicit DataManager(ks::FileSystem* fs);

    ~DataManager();

    ks::FileSystem* GetRefFsData() const {
        return m_fsData;
    }

    ks::FileSystem* GetRefFsAudio() const {
        return m_fsAudio;
    }

    ks::FileSystem* GetRefFsFont() const {
        return m_fsFont;
    }

    ks::FileSystem* GetRefFsIo() const {
        return m_fsIo;
    }

    ks::FileSystem* GetRefFsLua() const {
        return m_fsLua;
    }

    ks::FileSystem* GetRefFsMov() const {
        return m_fsMov;
    }

    ks::FileSystem* GetRefFsSfx() const {
        return m_fsSfx;
    }

    ks::FileSystem* GetRefFsShell() const {
        return m_fsShell;
    }

    ks::FileSystem* GetRefFsTex() const {
        return m_fsTex;
    }

    const std::string& GetGamesScriptPath() const {
        return m_games;
    }

private:
    ks::FileSystem* m_fsData;

    ks::FileSystem* m_fsAudio;
    ks::FileSystem* m_fsFont;
    ks::FileSystem* m_fsIo;
    ks::FileSystem* m_fsLua;
    ks::FileSystem* m_fsMov;
    ks::FileSystem* m_fsSfx;
    ks::FileSystem* m_fsShell;
    ks::FileSystem* m_fsTex;

    const std::string m_games;

private:
    void __AssertFs(ks::FileSystem* refFs);
};

}
