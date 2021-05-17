#pragma once

#include <map>
#include <mutex>

#include "io/IODevice.h"
#include "io/IOManagerCtx.h"

#include "io/keyboard/DigitalInputs.h"

#include "io/sdl2/KeyboardEventListener.h"

namespace io {
namespace keyboard {
namespace sdl2 {

static constexpr const char* DEVICE_IDENT = "keyboard-sdl2";

class Keyboard : public io::IODevice, public io::sdl2::KeyboardEventListener
{
public:
    explicit Keyboard(const io::IOManagerCtx& ioManagerCtx);

    ~Keyboard() override;

    void ProcessKeyboardEvent(const SDL_KeyboardEvent& event) override;

    void Open(const ks::Settings& settings) override;

    void Close() override;

    void Update() override;

    void ProcessInputStates(InputState& inputStates) override;

private:
    std::mutex m_mutex;
    bool m_keyStatesCritical[e_DI_Count];

    bool m_keyStates[e_DI_Count];
};

}
}
}