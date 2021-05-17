#include "io/IOException.h"

#include "io/joystick/sdl2/Joystick.h"

#include "io/sdl2/EventProxy.h"

namespace io {
namespace joystick {
namespace sdl2 {

Joystick::Joystick(const io::IOManagerCtx& ioManagerCtx) :
    IODevice(DEVICE_IDENT)
{
    auto eventProxy = ioManagerCtx.Get<io::sdl2::EventProxy*>("sdl2-event-proxy");
    eventProxy->AddJoystickEventListener(this);

    memset(m_buttonStatesCritical, 0, sizeof(m_buttonStatesCritical));
    memset(m_buttonStates, 0, sizeof(m_buttonStates));
}

Joystick::~Joystick()
{
    // empty
}

void Joystick::ProcessJoystickButtonEvent(const SDL_JoyButtonEvent& event)
{
    bool state = false;
    bool setEvent = false;

    // Hardcoded limit of 32 buttons of a single joystick
    if (event.button >= 32) {
        return;
    }

    // Hardcoded limit of 4 joysticks
    if (event.which >= 4) {
        return;
    }

    if (event.state == SDL_PRESSED) {
        state = true;
        setEvent = true;
    } else if (event.state == SDL_RELEASED) {
        state = false;
        setEvent = true;
    }

    if (setEvent) {
        std::lock_guard<std::mutex> lock(m_mutex);

        // Matches what's defined as enum DigitalInputs
        m_buttonStatesCritical[event.which * 32 + event.button] = state;
    }
}

void Joystick::Open(const ks::Settings& settings)
{
    // empty
}

void Joystick::Close()
{
    // empty
}

void Joystick::Update()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    memcpy(m_buttonStates, m_buttonStatesCritical, sizeof(m_buttonStates));
}

void Joystick::ProcessInputStates(InputState& inputStates)
{
    for (uint16_t i = 0; i < e_DI_Count; i++) {
        inputStates.SetDigital(i, m_buttonStates[i]);
    }
}

}
}
}