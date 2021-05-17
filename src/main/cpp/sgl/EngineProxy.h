#pragma once

#include "io/IOManager.h"

#include "ks/Settings.h"

#include "sgl/lua/LuaState.h"

#include "sgl/DataManager.h"
#include "sgl/FontManager.h"
#include "sgl/Renderer.h"
#include "sgl/SaveState.h"
#include "sgl/SFXManager.h"
#include "sgl/SoundRenderer.h"
#include "sgl/SpriteRenderer.h"
#include "sgl/TextureManager.h"
#include "sgl/VideoManager.h"
#include "sgl/WorldState.h"

namespace sgl {

class EngineProxy
{
public:
    EngineProxy(ks::Settings* refSettings, DataManager* refDataManager, io::IOManager* refIoManager,
            SoundRenderer* refSoundRenderer, SFXManager* refSFXManager, Renderer* refRenderer,
            FontManager* refFontManager, TextureManager* refTextureManager, SpriteRenderer* refSpriteRenderer,
            VideoManager* refVideoManager, WorldState* refWorldState, SaveState* refSaveState, LuaState* refLuaState) :
        m_refSettings(refSettings),
        m_refDataManager(refDataManager),
        m_refIoManager(refIoManager),
        m_refSoundRenderer(refSoundRenderer),
        m_refSFXManager(refSFXManager),
        m_refRenderer(refRenderer),
        m_refFontManager(refFontManager),
        m_refTextureManager(refTextureManager),
        m_refSpriteRenderer(refSpriteRenderer),
        m_refVideoManager(refVideoManager),
        m_refWorldState(refWorldState),
        m_refSaveState(refSaveState),
        m_refLuaState(refLuaState)
    {
    };

    ~EngineProxy() = default;

    ks::Settings& GetSettings() const {
        return *m_refSettings;
    }

    DataManager& GetDataManager() const {
        return *m_refDataManager;
    }

    io::IOManager& GetIoManager() const {
        return *m_refIoManager;
    }

    SoundRenderer& GetSoundRenderer() const {
        return *m_refSoundRenderer;
    }

    SFXManager& GetSFXManager() const {
        return *m_refSFXManager;
    }

    Renderer& GetRenderer() const {
        return *m_refRenderer;
    }

    FontManager& GetFontManager() const {
        return *m_refFontManager;
    }

    TextureManager& GetTextureManager() const {
        return *m_refTextureManager;
    }

    SpriteRenderer& GetSpriteRenderer() const {
        return *m_refSpriteRenderer;
    }

    VideoManager& GetVideoManager() const {
        return *m_refVideoManager;
    }

    WorldState& GetWorldState() const {
        return *m_refWorldState;
    }

    SaveState& GetSaveState() const {
        return *m_refSaveState;
    }

    LuaState& GetLuaState() const {
        return *m_refLuaState;
    }

private:
    ks::Settings* m_refSettings;

    DataManager* m_refDataManager;
    io::IOManager* m_refIoManager;
    SoundRenderer* m_refSoundRenderer;
    SFXManager* m_refSFXManager;
    Renderer* m_refRenderer;
    FontManager* m_refFontManager;
    TextureManager* m_refTextureManager;
    SpriteRenderer* m_refSpriteRenderer;
    VideoManager* m_refVideoManager;

    WorldState* m_refWorldState;

    SaveState* m_refSaveState;

    LuaState* m_refLuaState;
};

}
