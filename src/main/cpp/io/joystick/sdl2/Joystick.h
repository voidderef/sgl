#pragma once

#include <map>
#include <mutex>

#include "io/IODevice.h"
#include "io/IOManagerCtx.h"

#include "io/joystick/DigitalInputs.h"

#include "io/sdl2/JoystickEventListener.h"

namespace io {
namespace joystick {
namespace sdl2 {

static constexpr const char* DEVICE_IDENT = "joystick-sdl2";

class Joystick : public io::IODevice, public io::sdl2::JoystickEventListener
{
public:
    explicit Joystick(const io::IOManagerCtx& ioManagerCtx);

    ~Joystick() override;

    void ProcessJoystickButtonEvent(const SDL_JoyButtonEvent& event) override;

    void Open(const ks::Settings& settings) override;

    void Close() override;

    void Update() override;

    void ProcessInputStates(InputState& inputStates) override;

private:
    std::mutex m_mutex;
    bool m_buttonStatesCritical[e_DI_Count];

    bool m_buttonStates[e_DI_Count];
};

}
}
}