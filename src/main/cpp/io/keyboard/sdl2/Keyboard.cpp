#include "io/IOException.h"

#include "io/keyboard/sdl2/Keyboard.h"
#include "io/keyboard/sdl2/Mappings.h"

#include "io/sdl2/EventProxy.h"

namespace io {
namespace keyboard {
namespace sdl2 {

Keyboard::Keyboard(const io::IOManagerCtx& ioManagerCtx) :
    IODevice(DEVICE_IDENT)
{
    auto eventProxy = ioManagerCtx.Get<io::sdl2::EventProxy*>("sdl2-event-proxy");
    eventProxy->AddKeyboardEventListener(this);

    memset(m_keyStatesCritical, 0, sizeof(m_keyStatesCritical));
    memset(m_keyStates, 0, sizeof(m_keyStates));
}

Keyboard::~Keyboard()
{
    // empty
}

void Keyboard::ProcessKeyboardEvent(const SDL_KeyboardEvent& event)
{
    auto digitalInput = Mappings::Get(event.keysym);
    bool state = false;
    bool setEvent = false;

    if (event.state == SDL_PRESSED) {
        state = true;
        setEvent = true;
    } else if (event.state == SDL_RELEASED) {
        state = false;
        setEvent = true;
    } else if (event.repeat > 0){
        state = true;
        setEvent = false;
    }

    if (setEvent) {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_keyStatesCritical[digitalInput] = state;
    }
}

void Keyboard::Open(const ks::Settings& settings)
{
    // empty
}

void Keyboard::Close()
{
    // empty
}

void Keyboard::Update()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    memcpy(m_keyStates, m_keyStatesCritical, sizeof(m_keyStates));
}

void Keyboard::ProcessInputStates(InputState& inputStates)
{
    for (uint16_t i = 0; i < e_DI_Count; i++) {
        inputStates.SetDigital(i, m_keyStates[i]);
    }
}

}
}
}