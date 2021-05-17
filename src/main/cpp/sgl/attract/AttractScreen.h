#pragma once

#include "Games.h"
#include "SaveState.h"
#include "Screen.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"

#include "sgl/attract/DefaultGameSelectorTimer.h"

namespace sgl {

class AttractScreen : public Screen
{
public:
    enum InputStateDigital {
        e_ISD_SelectNext = 0,
        e_ISD_SelectPrev = 1,
        e_ISD_SelectionConfirmed = 2,
        e_ISD_ExitApplication = 3,
        e_ISD_OperatorScreen = 4,
        e_ISD_Debug0 = 5,
        e_ISD_Debug1 = 6,
        e_ISD_Debug2 = 7,
        e_ISD_Debug3 = 8,
        e_ISD_Debug4 = 9,
        e_ISD_Debug5 = 10,
        e_ISD_Debug6 = 11,
        e_ISD_Debug7 = 12,
    };

    enum OutputStateDigital {
        e_OSD_NewEntrySelected = 0,
        e_OSD_EntrySelectionConfirmed = 1,
        e_OSD_ExitScreen = 2,
    };

    enum OutputStateText {
        e_OST_GameNameSelected = 0,
    };

public:
    static const std::string MS_NAME;

    AttractScreen();
    ~AttractScreen() override = default;

    bool Init() override;

    void Cleanup() override;

    void Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState) override;

private:
    static const std::string ms_settingDefaultGameTimeoutId;
    static const std::string ms_settingDefaultGameTimeoutDurationSec;

private:
    Games* m_refGames;

    DefaultGameSelectorTimer* m_defaultGameSelectorTimer;

    bool __IsAnyInputTriggered(const io::InputState& inputState);

private:
    enum State {
        e_StateFirst = 0,
        e_StateLoop = 1,
        e_StateSelected = 2,
        e_StateTransition = 3,
        e_StateExecute = 4,
        e_StateExitApplication = 5,
        e_StateEnterOperatorScreen = 6,
    };

    State m_currentState;

    size_t m_pos;

    void __SelectNextEntry(io::OutputState& outputState);
    void __SelectPreviousEntry(io::OutputState& outputState);
    void __SelectEntry(io::OutputState& outputState, size_t entryIdx);
};

}
