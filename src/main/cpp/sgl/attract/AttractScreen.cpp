#include "AttractScreen.h"

#include "sgl/operator/OperatorScreen.h"

namespace sgl {

const std::string AttractScreen::MS_NAME = "attract";

const std::string AttractScreen::ms_settingDefaultGameTimeoutId = "screen" + ks::Settings::ms_delim +
    "attract" + ks::Settings::ms_delim + "default_game_timeout_id";
const std::string AttractScreen::ms_settingDefaultGameTimeoutDurationSec = "screen" + ks::Settings::ms_delim +
    "attract" + ks::Settings::ms_delim + "default_game_timeout_duration_sec";

AttractScreen::AttractScreen() :
    Screen(MS_NAME),
    m_defaultGameSelectorTimer(nullptr),
    m_currentState(e_StateFirst),
    m_pos(0)
{

}

bool AttractScreen::Init()
{
    m_refGames = _GetEngineProxy()->GetWorldState().Get<Games>("games");

    m_refGames->SortById();

    auto defaultGameTimeoutId = _GetEngineProxy()->GetSettings().GetValue<uint32_t>(ms_settingDefaultGameTimeoutId);
    auto defaultGameTimeoutDurationSec =
        _GetEngineProxy()->GetSettings().GetValue<uint32_t>(ms_settingDefaultGameTimeoutDurationSec);

    if (defaultGameTimeoutId >= m_refGames->GetEntries().size()) {
        KS_LOG_ERROR("Invalid game ID defined for default_game_timeout_id, timeout feature disabled: %d >= %d",
            defaultGameTimeoutId,
            m_refGames->GetEntries().size());
        defaultGameTimeoutDurationSec = 0;
    }

    m_defaultGameSelectorTimer = new DefaultGameSelectorTimer(defaultGameTimeoutId, defaultGameTimeoutDurationSec);
    m_defaultGameSelectorTimer->Init(_GetEngineProxy());

    // If a default game to start on timeout is activated, ignore this to have the default game on screen
    // This avoids weird looking transitions when going from the idle animation to the selected animation
    if (defaultGameTimeoutDurationSec > 0) {
        m_pos = defaultGameTimeoutId;
    } else {
        m_pos = _GetEngineProxy()->GetSaveState().GetGameEntrySelected();
    }

    // check if position is still valid -> game list has changed
    if (m_pos >= m_refGames->GetEntries().size()) {
        m_pos = 0;
    }

    m_currentState = e_StateFirst;

    return true;
}

void AttractScreen::Cleanup()
{
    m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnDisable();
    m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUnload();

    m_refGames = nullptr;

    m_defaultGameSelectorTimer->Cleanup(_GetEngineProxy());
    delete m_defaultGameSelectorTimer;
    m_defaultGameSelectorTimer = nullptr;
}

void AttractScreen::Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState)
{
    // current state execution
    switch (m_currentState) {
        case e_StateFirst: {
            KS_LOG_DEBUG("Initial game selected: %s", *m_refGames->GetEntries()[m_pos]);

            if (!m_refGames->GetEntries()[m_pos]->GetAttractBindings().LLoad(
                    m_refGames->GetEntries()[m_pos]->GetLuaRefResources(),
                    m_refGames->GetEntries()[m_pos]->GetLuaRefArgs())) {
                KS_LOG_ERROR("Loading resources of entry %s failed", *m_refGames->GetEntries()[m_pos]);
            }

             m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnEnable();

            // new entry selected event
            outputState.SetDigital(e_OSD_NewEntrySelected, true);

            // set text of item selected
            outputState.SetText(e_OST_GameNameSelected, m_refGames->GetEntries()[m_pos]->GetName());

            m_currentState = e_StateLoop;

            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUpdate(prevDeltaSec, inputState);

            break;
        }

        case e_StateLoop: {
            m_defaultGameSelectorTimer->Update(_GetEngineProxy());

            if (__IsAnyInputTriggered(inputState)) {
                m_defaultGameSelectorTimer->InputTriggered();
            }

            // Check first, if the default game select timeout is triggered before evaluating any inputs
            if (m_defaultGameSelectorTimer->IsTimeout()) {
                m_currentState = e_StateSelected;
            } else if (inputState.GetDigital(e_ISD_SelectNext)) {
                __SelectNextEntry(outputState);
            } else if (inputState.GetDigital(e_ISD_SelectPrev)) {
                __SelectPreviousEntry(outputState);
            } else if (inputState.GetDigital(e_ISD_SelectionConfirmed)) {
                m_currentState = e_StateSelected;
            } else if (inputState.GetDigital(e_ISD_ExitApplication)) {
                m_currentState = e_StateExitApplication;
            } else if (inputState.GetDigital(e_ISD_OperatorScreen)) {
                m_currentState = e_StateEnterOperatorScreen;
            }

            // set text of item selected
            outputState.SetText(e_OST_GameNameSelected, m_refGames->GetEntries()[m_pos]->GetName());

            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUpdate(prevDeltaSec, inputState);

            break;
        }

        case e_StateSelected: {
            bool confirmationOk = m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnConfirm();

            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUpdate(prevDeltaSec, inputState);

            // the script can nak the confirmation triggered by the user if, for example, the current
            // state of the script does not allow a transition to the selected state
            if (confirmationOk) {
                KS_LOG_DEBUG("Entry, now selected: %s", m_refGames->GetEntries()[m_pos]->GetName());

                _GetEngineProxy()->GetSFXManager().Play(SFXManager::e_EffectAttractConfirm);

                // set text of item selected
                outputState.SetText(e_OST_GameNameSelected, m_refGames->GetEntries()[m_pos]->GetName());

                // entry selected, decision event
                outputState.SetDigital(e_OSD_EntrySelectionConfirmed, true);

                m_currentState = e_StateTransition;
            } else {
                KS_LOG_DEBUG("NAK by script on entry selected: %s", m_refGames->GetEntries()[m_pos]->GetName());
                m_currentState = e_StateLoop;
            }

            break;
        }

        case e_StateTransition: {
            // wait for the script to confirm that it finished with its transition and
            // for the confirm sound to finish playing
            if (m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnTransition() &&
                    !_GetEngineProxy()->GetSFXManager().IsPlaying(SFXManager::e_EffectAttractConfirm)) {
                KS_LOG_DEBUG("Transition finished");
                m_currentState = e_StateExecute;
            }

            // set text of item selected
            outputState.SetText(e_OST_GameNameSelected, m_refGames->GetEntries()[m_pos]->GetName());

            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUpdate(prevDeltaSec, inputState);

            break;
        }

        case e_StateExecute: {
            outputState.SetDigital(e_OSD_ExitScreen, true);


            // one last update call to script
            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUpdate(prevDeltaSec, inputState);
            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnDisable();

            // force trigger out of main loop update. this should never be called anywhere outside of the main loop
            // however, the situation here is different because we execute a shell script and (hopefully) never
            // return
            _GetEngineProxy()->GetIoManager().StateProcess();
            _GetEngineProxy()->GetIoManager().FlushOutputStates();

            // write save state
            _GetEngineProxy()->GetSaveState().Save();

            // this call should never return
            m_refGames->GetEntries()[m_pos]->ExecuteShellScript();

            // otherwise reset
            m_currentState = e_StateLoop;
            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnEnable();

            break;
        }

        case e_StateExitApplication: {
            outputState.SetDigital(e_OSD_ExitScreen, true);

            // one last update call to script
            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUpdate(prevDeltaSec, inputState);
            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnDisable();

            _ExitApplication();

            KS_LOG_DEBUG("User quit");

            break;
        }

        case e_StateEnterOperatorScreen: {
            outputState.SetDigital(e_OSD_ExitScreen, true);

            // one last update call to script
            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUpdate(prevDeltaSec, inputState);
            m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnDisable();

            _SetNextScreen(OperatorScreen::MS_NAME);

            break;
        }

        default:
            break;
    }
}

bool AttractScreen::__IsAnyInputTriggered(const io::InputState& inputState)
{
    return inputState.GetDigital(e_ISD_SelectNext) ||
        inputState.GetDigital(e_ISD_SelectPrev) ||
        inputState.GetDigital(e_ISD_SelectionConfirmed) ||
        inputState.GetDigital(e_ISD_ExitApplication) ||
        inputState.GetDigital(e_ISD_OperatorScreen) ||
        inputState.GetDigital(e_ISD_Debug0) ||
        inputState.GetDigital(e_ISD_Debug1) ||
        inputState.GetDigital(e_ISD_Debug2) ||
        inputState.GetDigital(e_ISD_Debug3) ||
        inputState.GetDigital(e_ISD_Debug4) ||
        inputState.GetDigital(e_ISD_Debug5) ||
        inputState.GetDigital(e_ISD_Debug6) ||
        inputState.GetDigital(e_ISD_Debug7);
}

void AttractScreen::__SelectNextEntry(io::OutputState& outputState)
{
    size_t newPos = m_pos;

    newPos++;

    if (newPos >= m_refGames->GetNumEntries()) {
        newPos = 0;
    }

    __SelectEntry(outputState, newPos);
}

void AttractScreen::__SelectPreviousEntry(io::OutputState& outputState)
{
    size_t newPos = m_pos;

    if (newPos == 0) {
        newPos = m_refGames->GetNumEntries() - 1;
    } else {
        newPos--;
    }

    __SelectEntry(outputState, newPos);
}

void AttractScreen::__SelectEntry(io::OutputState& outputState, size_t newPos)
{
    // Unload current
    m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnDisable();
    m_refGames->GetEntries()[m_pos]->GetAttractBindings().LUnload();

    m_pos = newPos;

    _GetEngineProxy()->GetSaveState().SetGameEntrySelected(m_pos);

    KS_LOG_DEBUG("Now selected: %s", m_refGames->GetEntries()[m_pos]->GetName());

    if (!m_refGames->GetEntries()[m_pos]->GetAttractBindings().LLoad(
            m_refGames->GetEntries()[m_pos]->GetLuaRefResources(),
            m_refGames->GetEntries()[m_pos]->GetLuaRefArgs())) {
        KS_LOG_ERROR("Loading resources of entry %s failed", *m_refGames->GetEntries()[m_pos]);
    }

    m_refGames->GetEntries()[m_pos]->GetAttractBindings().LOnEnable();

    _GetEngineProxy()->GetSFXManager().Play(SFXManager::e_EffectAttractSelect);

    // new entry selected event
    outputState.SetDigital(e_OSD_NewEntrySelected, true);

    // set text of item selected
    outputState.SetText(e_OST_GameNameSelected, m_refGames->GetEntries()[m_pos]->GetName());
}

}